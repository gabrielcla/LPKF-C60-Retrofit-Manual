# Como Usinar a partir de G-Codes

## Para começar:
- Ligue a máquina na ordem correta
- Faça Homing
- Ja tenha os gcodes que quer imprimir no sdcard
- O spindle precisa estar parado


## Processo de Usinagem:  
1. Trocar Ferramenta: Usar Fresa
2. Prenda a pcb no local correto e alinhada
3. Ajustar Limitador de Profundidade: Para Fresar
4. Ligar Spindle na velocidade certa
5. OPCIONAL: Usar script pra testar se a profundidade ta boa
6. Executar o G-code com as Trilhas e esperar acabar
7. Desligar Spindle
8. Trocar Ferramenta: Usar Broca
9. Ajustar Limitador de Profundidade: Para Furar
10. Ligar Spindle na velocidade certa
11. Executar o G-code com os Furos e esperar acabar
12. Desligar Spindle
13. Mover a máquina pra posição S
14. OPCIONAL: Testar continuidade nas trilhas pra ver se separou bem
14. Desgrudar a pcb da máquina



## Ordem de Usinagem

1. Trilhas
2. Furos
3. Borda da placa

> Recomendo fortemente cortar a borda da placa manualmente pois precisa ajustar a profundidade e repetir a fresagem do contorno várias vezes
---


➡️ Próximo tutorial:  
[05 - Problemas Comuns e Soluções](05_problemas_comuns.md)  

