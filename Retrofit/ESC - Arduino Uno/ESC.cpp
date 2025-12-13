#include <avr/io.h>        // acesso direto aos registradores (PORT, DDR, TCCR, etc.)
#include <avr/interrupt.h> // controle de interrupções (sei(), cli(), ISR)
#include <avr/pgmspace.h>  // para usar PROGMEM e pgm_read_byte_near()
#include <util/delay.h>    // para usar _delay_ms() no lugar de delay()

/*#########################################| DEFINIÇÕES |########################################*/

// Frequência de rotação do motor
#define fMax      1000 // 60k RPM (Valor escrito no spindle)
#define fMin       170 // 10k RPM

// Baud rate UART (115200 bps @ 16 MHz → UBRR0 = 8)
#define UBRR0_VALUE 8

// tabela do seno - 256 valores de 0 a 255
const unsigned char seno[] PROGMEM = {127, 130, 133, 136, 140, 143, 146, 149, 152, 155, 158, 161, 164, 167, 170, 173, 176, 179, 
182, 185, 187, 190, 193, 195, 198, 201, 203, 206, 208, 211, 213, 215, 217, 220, 222, 224, 226, 228, 230, 232, 233, 235, 237, 238,
240, 241, 242, 244, 245, 246, 247, 248, 249, 250, 251, 252, 252, 253, 253, 254, 254, 254, 254, 255, 255, 254, 254, 254, 254, 253, 
253, 252, 252, 251, 250, 250, 249, 248, 247, 246, 244, 243, 242, 240, 239, 237, 236, 234, 232, 231, 229, 227, 225, 223, 221, 219, 
216, 214, 212, 209, 207, 204, 202, 199, 197, 194, 191, 189, 186, 183, 180, 177, 175, 172, 169, 166, 163, 160, 157, 154, 150, 147, 
144, 141, 138, 135, 132, 129, 125, 122, 119, 116, 113, 110, 107, 104, 100, 97, 94, 91,88, 85, 82, 79, 77, 74, 71, 68, 65, 63, 60, 
57, 55, 52, 50, 47, 45, 42, 40, 38, 35, 33, 31, 29, 27, 25, 23, 22, 20, 18, 17, 15, 14, 12, 11, 10, 8, 7, 6, 5, 4, 4, 3, 2, 2, 1, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 13, 14, 16, 17, 19, 21, 22, 24, 26, 28, 30, 32, 34, 37, 
39, 41, 43, 46, 48, 51, 53, 56, 59, 61, 64, 67, 69, 72, 75, 78, 81, 84, 87, 90, 93, 96,99,102, 105, 108, 111, 114, 118, 121, 124,
127};

double dfreq;
unsigned int i, repets;

// variaveis usadas dentro da interrupção do timer declaradas como volatile
volatile char current_count;
volatile unsigned char duty; // duty - razão cíclica
volatile unsigned long phaccu; // acumulador de fase
volatile unsigned long tword_m; // DDS tuning word m

// 120 graus de defasagem entre as fases
int offset_1 = 85; 
int offset_2 = 170;

// Precisa declarar as funções antes de chama-las
void partida();
void adc_init(void);
uint16_t adc_read(void);
long mapFast(long x, long in_min, long in_max, long out_min, long out_max);

// ==== UART ====
void usart_init(void);
void usart_tx_char(char c);
void usart_tx_string(const char *s);
void usart_tx_uint16(uint16_t val);

/*#########################################| PROGRAMA |##########################################*/

int main(void) { 
  // Habilita a interrupção pelo timer2 quando houver overflow
  TIMSK2 = (1 << TOIE2); // Seta só o TOIE2 como 1 e o resto como 0
  sei(); // Habilita interrupções globais para as interrupções funcionarem

  // Define os pinos como output
  DDRB = ((1 << PB1) | (1 << PB2) | (1 << PB3));  // Define os pinos 9, 10 e 11 como output
  DDRD = ((1<< PD3) | (1 << PD5) | (1 << PD6));   // Define os pinos 3, 5 e 6 como output

  // (Opcional) garante PD1 como saída de TX (não é estritamente necessário)
  DDRD |= (1 << PD1);  // pino digital 1 (TX) como saída

  /* CONFIGURAÇÃO DOS TIMERS */
  // Timer0 - MODO PWM FASE CORRIGIDA DE 8 BITS
  TCCR0A = ((1<<COM0A1) | (0<<COM0A0) | (1<<COM0B1) | (1<<COM0B0) | (1<<WGM00) | (1<<WGM02) | (0<<WGM01)); 
  TCCR0B = (0<<CS02) | (1<<CS01) | (0<<CS00); // liga T/C0 8 prescaler
  // Timer1 - MODO PWM FASE CORRIGIDA DE 8 BITS
  TCCR1A = (1<<COM1A1) | (0<<COM1A0) | (1<<COM1B1) | (1<<COM1B0) | (1<<WGM10);
  TCCR1B = (0<<CS12) | (1<<CS11) | (0<<CS10); // liga T/C1 8 prescaler
  // Timer2 - MODO PWM FASE CORRIGIDA DE 8 BITS
  TCCR2A = (1<<COM2A1) | (0<<COM2A0) | (1<<COM2B1) | (1<<COM2B0) | (1<<WGM20); 
  TCCR2B = (0<<CS22) | (1<<CS21) | (0<<CS20); // liga T/C2 com prescaler de 8

  // Inicia o ADC pra ler o pino A1
  adc_init();

  // Inicia UART (NOVO)
  usart_init();
  usart_tx_string("ESC iniciado.\r\n");

  // Dá a partida no motor aos poucos
  partida();

  // Sem o loop o PWM para de atualizar o tword e o motor para
  while (1) { 
      
    // Só muda os dados 1 vez a cada 100 ciclos (senão gasta tempo demais fazendo isso)
    if (repets == 100) {
        uint16_t leitura = adc_read(); // Pega o valor do pino
        // Como o esp32 é 3,3V e o arduino 5V, o valor maximo q o arduino vai marcar é 675
        dfreq = mapFast(leitura, 0, 675, fMin, fMax); // Calcula a freq equivalente
        tword_m = fma(pow(2,32),(dfreq/3921.6),0); // calulate DDS new tuning word
        repets = 0;
        // ===== Envia info pela serial ========
        usart_tx_string("ADC=");
        usart_tx_uint16(leitura);
        usart_tx_string(" F=");
        usart_tx_uint16((uint16_t)dfreq);
        usart_tx_string("\r\n");
    }
    repets++;
  }   

}

/*########################################| Interrupções |#######################################*/

// Função chamada ao ocorrer interrupção
ISR(TIMER2_OVF_vect) { // interrupção do timer para leitura da tabela de senos   
    phaccu=phaccu+tword_m; // Adiciona tuning M word ao acumulador de fase anterior
    current_count=phaccu >> 24; // Pega os 8 bits mais significativos do acumulador de fase (phaccu)  
    duty=100;
    OCR2A=((pgm_read_byte_near(seno + (uint8_t)(current_count)))*duty)/100;             // Pino 11
    OCR2B=((pgm_read_byte_near(seno + (uint8_t)(current_count)))*duty)/100;             // Pino 3
    OCR1A=((pgm_read_byte_near(seno + (uint8_t)(current_count + offset_1)))*duty)/100;  // Pino 9
    OCR1B=((pgm_read_byte_near(seno + (uint8_t)(current_count + offset_1)))*duty)/100;  // Pino 10
    OCR0A=((pgm_read_byte_near(seno + (uint8_t)(current_count + offset_2)))*duty)/100;  // Pino 6
    OCR0B=((pgm_read_byte_near(seno + (uint8_t)(current_count + offset_2)))*duty)/100;  // Pino 5
}

/*##########################################| FUNÇÕES |##########################################*/

// Usado pra partir o motor
void partida(void) {
  // Inicia com 1hz e vai aumentando por 2s até chegar a 60hz
  dfreq = 1;
  for(i = 1; i < 20; i++) {
    dfreq = 3*i;
    tword_m = fma(pow(2,32),(dfreq/3921.6),0); // calulate DDS new tuning word
    _delay_ms(100);
  }
  // Inicia com 60hz e vai aumentando por 2,2s até chegar a 10 mil RPM
  for(i = 0; i < 11; i++) {
    dfreq = 60 + 10*i;
    tword_m = fma(pow(2,32),(dfreq/3921.6),0); // calulate DDS new tuning word
    _delay_ms(200);
  }
}

/*#############################################| UART |##########################################*/
// Inicializa USART0 para TX em 115200 bps, 8N1, só transmissão
void usart_init(void) {
  // Configura baud rate
  UBRR0H = (uint8_t)(UBRR0_VALUE >> 8);
  UBRR0L = (uint8_t)(UBRR0_VALUE & 0xFF);
  UCSR0A = 0x00;              // modo normal, sem multiprocessador
  UCSR0B = (1 << TXEN0);      // habilita só transmissor (sem RX, sem interrupções)
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8 bits, 1 stop, sem paridade
}
// Envia 1 caractere (bloqueante, mas com interrupções habilitadas)
void usart_tx_char(char c) {
  while (!(UCSR0A & (1 << UDRE0))) {}
  UDR0 = c;
}
// Envia string terminada em '\0'
void usart_tx_string(const char *s) {
  while (*s) {
    usart_tx_char(*s++);
  }
}
// Envia um inteiro 16 bits em decimal (0–65535)
void usart_tx_uint16(uint16_t val) {
  char buf[6]; // até "65535" + '\0' se quisesse, mas aqui mando invertido
  uint8_t i = 0;
  if (val == 0) {
    usart_tx_char('0');
    return;
  }
  while (val > 0 && i < 5) { // Converte para dígitos ao contrário
    buf[i++] = '0' + (val % 10);
    val /= 10;
  }
  while (i > 0) { // Envia na ordem correta
    usart_tx_char(buf[--i]); 
  }
}

/*##########################################| ADC |##########################################*/

// === Configuração inicial do ADC ===
void adc_init() {
  ADMUX = (1 << REFS0) | (1 << MUX0); 
  ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); 
}
// === Leitura rápida do ADC ===
uint16_t adc_read() {
  ADCSRA |= (1 << ADSC);            // inicia conversão
  while (ADCSRA & (1 << ADSC));     // espera terminar
  return ADC;                       // retorna o valor de 10 bits
}
// === Mapeamento rápido (substitui o map() da Arduino) ===
long mapFast(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
