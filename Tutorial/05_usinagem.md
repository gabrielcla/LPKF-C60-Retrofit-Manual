# Como Usinar a partir de G-codes

## Preparação
- Ligue a máquina na ordem correta
- Faça Homing
- O spindle precisa estar parado. Antes da execução de qualquer gcode ele precisa estar parado.
- Ja tenha todos os gcodes da placa que quer usinar.

### Testagem dos G-codes e Posicionamento
> Para ter certeza que o gcode esta como deveria, execute o código dele SEM TER NADA NA MESA DA MÁQUINA!
> Aproveite também pra marcar a posição que a placa deve ficar na base da máquina

Para garantir que seu gcode vai ligar o spindle, baixar o solenoide e fazer todos os movimentos corretos,
além de tambem verificar se o projeto no kicad definiu corretamente as posições de origem, executa-se os gcodes sem ter placa na máquina.  

Garanta que ela esta fazendo as seguintes ações:
    Ligar o spindle e atingir a velocidade final antes de baixar o solenoide
    Se mover dentro da area da máquina sem ativar os interruptores de fim de curso



## Processo de Usinagem:
O Processo consiste de usinar primeiros as trilhas, depois furos e caso necessário, os contornos

### 1. Usinar as Trilhas
1. Trocar Ferramenta: Usar Fresa
2. Prenda a pcb no local correto e alinhada
3. Ajustar Limitador de Profundidade: Para Fresar
4. OPCIONAL: Usar script pra testar se a profundidade ta boa
5. Executar o G-code com as Trilhas e esperar acabar

### 2. Usinar os Furos
1. Trocar Ferramenta: Usar Broca
2. Ajustar Limitador de Profundidade: Para Furar
3. Executar o G-code com os Furos e esperar acabar


### 3. [Não Recomendo] Usinar Contorno
> Caso seja necessário cortar o conorno da placa, recomendo fortemente fazer isso manualmente, pois **é bem fácil quebrar a fresa**.

O processo é igual ao de usinar trilhas, só que inves ser bem superficial precisa ir 
executando o gcode dele varias vezes e ir incrementando a profundidade até atravessar e separar
> Ter pressa e ajustar a profundidade muito fundo vai **quebrar a fresa!!!**

1. Trocar Ferramenta: Usar Fresa
2. Ajustar Limitador de Profundidade: Para Fresar
3. Executar o G-code com o Contorno e esperar acabar
4. Ajustar o Limitador de Profundidade um pouco mais fundo do que está
5. Executar o mesmo G-code com o Contorno e esperar acabar
6. Enquanto a placa com o contorno não tiver sido separada repita os passos 4 e 5


### 4. Finalização
1. Mover a máquina pra posição S
2. OPCIONAL: Testar continuidade nas trilhas pra ver se separou bem
3. Desgrudar a pcb da máquina e limpa-la



---

[⬅ Voltar para Tutoriais](../README.md#tutoriais)

---
> Licença: MIT — consulte [`LICENSE`](../LICENSE).
