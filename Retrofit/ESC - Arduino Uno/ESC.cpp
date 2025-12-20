// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Gabriel Couto

/*###############################################################################################*/
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

bool Ligado;
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

// Valores do ADC calibrados para o MKS DLC32 que fornece o sinal ao Arduino
uint16_t adc_10k = 93;      // Valor do ADC que equivale a 10 mil RPM (min)
uint16_t adc_60k = 664;     // Valor do ADC que equivale a 60 mil RPM (max)
uint16_t leitura_anterior;  // Armazena a leitura anterior do ADC

/* DECLARA AS FUNÇÕES */
void adc_init(void);
long mapFast(long x, long in_min, long in_max, long out_min, long out_max);
void usart_init(void);
void usart_tx_char(char c);
void usart_tx_string(const char *s);
void usart_tx_uint16(uint16_t val);
void ligarSpindleVel(long freq_final);
void desligarSpindle(void);
void partidaVel(long freq_final);
uint16_t adc_read_stable();

/*###############################################################################################*/
/*#########################################| PROGRAMA |##########################################*/

//FAZ PARTIR ATÉ A VELOCIDADE ESPECIFICADA MAS DEPOIS NÃO MUDA A VELOCIDADE, SÓ DESLIGA
int main(void) { 
    // Inicia spindle Desligado
    desligarSpindle();
    Ligado = false; 
    // Inicia o ADC pra ler o pino A1
    adc_init();
    // Inicia UART pra comunicação Serial
    usart_init();
    usart_tx_string("ESC inicia desligado.\r\n");

    // Sem o loop o PWM para de atualizar o tword e o motor para
    while (1) {  
        // Só muda os dados 1 vez a cada 100 ciclos (senão gasta tempo demais fazendo isso)
        if (repets == 100) {
            uint16_t leitura = adc_read_stable(); 
            // Se a leitura tiver constante Liga/Desliga o motor conforme solicitado
            if (leitura == leitura_anterior) {
                // Se o motor tiver ligado e a leitura for menor que a mínima -> desliga ele
                if (Ligado == true) {
                    // Se a leitura for menor que a vel min de 10k rpm -> desligar
                    if (leitura < adc_10k) {
                        usart_tx_string("DESLIGANDO...\r\n");
                        desligarSpindle();
                        Ligado = false;
                    }
                }
                
                // Se o motor tiver desligado
                if (Ligado == false) {
                    // Se a leitura for maior igual que 10k Inicia o motor
                    if (leitura >= adc_10k) {
                        Ligado = true; // Define como ligado
                        // Calcula a freq equivalente
                        dfreq = mapFast(leitura, (long)adc_10k, (long)adc_60k, fMin, fMax);
                        
                        // Exibe a frequencia desejada
                        usart_tx_string("ADC=");
                        usart_tx_uint16(leitura);
                        usart_tx_string(" LIGANDO ATÉ ");
                        usart_tx_uint16((uint16_t)dfreq*60);
                        usart_tx_string("Hz \r\n");
                        
                        // Liga o spindle e dá a partida até a velocidade desejada
                        ligarSpindleVel(dfreq);
                    }
                }
                // // ===== Envia info pela serial ================================
                // usart_tx_string("ADC=");
                // usart_tx_uint16(leitura);
                // usart_tx_string(" F=");
                // usart_tx_uint16((uint16_t)dfreq);
                // usart_tx_string("\r\n");
                // // =============================================================
            }
            leitura_anterior = leitura; // Guarda o valor da leitura
            repets = 0;
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


/*######################################| LIGAR e DESLIGAR |#####################################*/

// ===============================================================
// DESLIGA COMPLETAMENTE O SPINDLE
// Para o IR2132 (entradas HIN/LIN invertidas: ativo-NÍVEL BAIXO)
//
// Estratégia segura:
// 1) Desabilitar PWM nos timers   -> pinos voltam a ser GPIO comuns
// 2) Forçar TODOS os pinos HIGH   -> HIGH = desativado no IR2132
// 3) Evitar qualquer possibilidade de shoot-through
// ===============================================================
void desligarSpindle(void) {
    cli();  // Travamos interrupções para garantir desligamento atômico
    // --- 1. Desativa os modos PWM (pinos deixam de ser OCnx e viram GPIO) ---
    TCCR0A = 0;    // Timer0 modo normal
    TCCR0B = 0;
    TCCR1A = 0;    // Timer1 modo normal
    TCCR1B = 0;
    TCCR2A = 0;    // Timer2 modo normal
    TCCR2B = 0;
    // --- 2. Força todos os pinos usados nos drivers HIGH ---
    //     IMPORTANTE: no IR2132 invertido, HIGH = desligado
    PORTB |= (1<<PB1) | (1<<PB2) | (1<<PB3);  // pinos 9, 10, 11 = HIGH
    PORTD |= (1<<PD3) | (1<<PD5) | (1<<PD6);  // pinos 3, 5, 6  = HIGH
    // O spindle agora está 100% desligado e todas as meias pontes do IR2132
    // foram colocadas em estado seguro (nenhum MOSFET conduz).
    sei();  // Libera interrupções novamente
}

// Liga o Spindle e dá a partida até a velocidade desejada
void ligarSpindleVel(long freq_final) {
    cli(); // trava interrupções durante a configuração dos timers

    // 1) Habilita a interrupção pelo timer2 quando houver overflow
    TIMSK2 = (1 << TOIE2); // Seta só o TOIE2 como 1 e o resto como 0
    sei(); // Habilita interrupções globais para as interrupções funcionarem

    // 2) Configura os pinos como saída
    DDRB = ((1 << PB1) | (1 << PB2) | (1 << PB3));
    DDRD = ((1<< PD3) | (1 << PD5) | (1 << PD6));

    /* 3) CONFIGURAÇÃO DOS TIMERS */
    // Timer0 - MODO PWM FASE CORRIGIDA DE 8 BITS
    TCCR0A = ((1<<COM0A1) | (0<<COM0A0) | (1<<COM0B1) | (1<<COM0B0) | (1<<WGM00) | (1<<WGM02) | (0<<WGM01)); 
    TCCR0B = (0<<CS02) | (1<<CS01) | (0<<CS00); //liga T/C0 8 prescaler
    // Timer1 - MODO PWM FASE CORRIGIDA DE 8 BITS
    TCCR1A = (1<<COM1A1) | (0<<COM1A0) | (1<<COM1B1) | (1<<COM1B0) | (1<<WGM10);
    TCCR1B = (0<<CS12) | (1<<CS11) | (0<<CS10); //liga T/C0 8 prescaler
    // Timer2 - MODO PWM FASE CORRIGIDA DE 8 BITS
    TCCR2A = (1<<COM2A1) | (0<<COM2A0) | (1<<COM2B1) | (1<<COM2B0) | (1<<WGM20); 
    TCCR2B = (0<<CS22) | (1<<CS21) | (0<<CS20); //liga T/C2 com prescaler de 8

    // 4) Zerar variáveis de estado do DDS
    phaccu = 0;
    tword_m = 0;
    duty = 100;

    // 5) Partida Suave até a freq desejada
    partidaVel(freq_final);
}


/*##########################################| PARTIDA |##########################################*/
  /*
  Vai aumentando 10Hz a cada ms
  10k = 170         ifinal=11  = 1.1s + 2s = 3.2s
  60k = 1000        ifinal=94  = 9.4s + 2s = 11.4s
  */
// Recebe a freq final e parte o motor até a frequencia desejada
void partidaVel(long freq_final) {
  // Inicia com 1hz e vai aumentando por 2s até chegar a 60hz
  dfreq = 1;
  for(i = 1; i < 20; i++) {
    dfreq = 3*i;
    tword_m = fma(pow(2,32),(dfreq/3921.6),0); // calulate DDS new tuning word
    _delay_ms(100);
  }

  // Vê quantas vezes precisa aumentar
  int IFinal = (freq_final-60)/10;
  // Inicia com 60hz e vai aumentando 10Hz a cada 0.1s até chegar a freq desejada
  for(i = 0; i <= IFinal; i++) {
    dfreq = 60 + 10*i;
    tword_m = fma(pow(2,32),(dfreq/3921.6),0); // calulate DDS new tuning word
    _delay_ms(100);
  }
}

/*###########################################| UART |############################################*/
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
  while (!(UCSR0A & (1 << UDRE0))) {
    // espera buffer de transmissão esvaziar
    // Timer2 ainda pode gerar interrupção aqui
  }
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
  // Converte para dígitos ao contrário
  while (val > 0 && i < 5) {
    buf[i++] = '0' + (val % 10);
    val /= 10;
  }
  // Envia na ordem correta
  while (i > 0) {
    usart_tx_char(buf[--i]);
  }
}

/*##########################################| ADC |##########################################*/

// === Configuração inicial do ADC ===
void adc_init() {
  ADMUX = (1 << REFS0) | (1 << MUX0); 
  // REFS0 = usa AVcc como referência
  // MUX0 = canal 1 (A1)
  // Habilita o ADC e configura prescaler 128 → ADC clock = 125 kHz
  ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); 
}

// === Leitura mais estável do ADC ===
uint16_t adc_read_stable() {
    uint32_t acc = 0;
    // 1) Descartar a primeira leitura (ruidosa)
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    // 2) Fazer 16 leituras válidas e tirar a média
    for(uint8_t n = 0; n < 16; n++) {
        ADCSRA |= (1 << ADSC);
        while (ADCSRA & (1 << ADSC));
        acc += ADC;
    }
    return (uint16_t)(acc >> 4);  // divide por 16 (média)
}

// === Mapeamento rápido (substitui o map() da Arduino) ===
long mapFast(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
