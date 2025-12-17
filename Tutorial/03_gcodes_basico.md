# Controlando a Máquina com G-codes

## Executando G-codes no FluidNC
Existem 2 modos de executar G-codes no FluidNC:
- A partir do comando no console
- A partir de um arquivo no sdcard

<br>

---
## Principais Comandos G-code

| Comando | Descrição | Exemplo |
|-------|----------|--------|
| `G0` | Movimento rápido (sem usinagem) até a posição definida | `G0 X10 Y20` |
| `G1` | Movimento linear com avanço (usinagem) | `G1 X50 Y30 F300` |
| `G2` | Interpolação circular no sentido horário (CW) | `G2 X40 Y20 I10 J0` |
| `G3` | Interpolação circular no sentido anti-horário (CCW) | `G3 X40 Y20 I-10 J0` |
| `G4` | Pausa (dwell) por um tempo definido | `G4 P2` |
| `G21` | Define unidades em milímetros | `G21` |
| `G28` | Move para a posição de referência (home padrão da máquina) | `G28` |
| `G90` | Modo de posicionamento absoluto | `G90` |
| `G91` | Modo de posicionamento incremental (relativo) | `G91` |
| `G92` | Define a posição atual como uma nova origem | `G92 X0 Y0 Z0` |
| `M0` | Pausa o programa até intervenção do operador | `M0` |
| `M2` | Finaliza o programa | `M2` |
| `M3` | Liga o spindle no sentido horário | `M3 S12000` |
| `M5` | Desliga o spindle | `M5` |
| `F` | Define a velocidade de avanço (feed rate) | `F200` |
| `S` | Define a rotação do spindle | `S10000` |


> ⚠️ **Observação:**  
> - A maioria dos G-codes gerados pelo FlatCAM utiliza `G90` (posicionamento absoluto).  
> - O comando `G92` **não altera fisicamente a posição da máquina**, apenas redefine a origem lógica.  




## Controlando a Máquina

### Movendo X e Y:
#### Fazer Homing e Ir pra Home
```gcode
$H           ; (Faz Homing)
G90 G0 X0 Y0 ; Modo Absoluto e Vai pra a Home (0,0)
```



#### Posicionar - Troca de Ferramenta
```gcode
G90 G0 X-100.0 Y-102.5 ; Modo Absoluto e vai pra (-100.0, -102.5)
```

#### Posicionar - Liberar Área da Placa
```gcode
G90 G0 X225.0 Y102.5 ; Modo Absoluto e vai pra  (225.0, 102.5)
```



### Movendo Z
Como o eixo Z é um solenoide, ele fica normal quando z>=0 e abaixa quando z<0.

#### Baixar e Levantar Solenoide
```gcode
G90     (Modo Absoluto)
G0 Z-1  (Abaixa o solenoide)
G4 P1   (Aguarda 1s)
G0 Z0   (Levanta o solenoide)
```


### Controlando Spindle
O comando `M3` liga o spindle com a velocidade fornecida enquanto o comando `M5` desliga<br>
OBS: A fresadora só aceita velocidades de 10 mil a 60 mil RPM.
```gcode
M3 S50000   (Liga o spindle a 50.000 RPM)
G4 P10      (Aguarda 10 segundos)
M5          (Desliga o spindle)
```




<br><br>

---
<p>
  <a href="../README.md#tutoriais">⬅ Voltar para Tutoriais</a>
  <span style="float:right">
    <a href="04_gcodes_cad.md">Próximo Tutorial ➡️</a>
  </span>

---
> Licença: MIT — consulte [`LICENSE`](../LICENSE).
