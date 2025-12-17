# Controlando a Máquina com G-codes

## Executando G-codes no FluidNC
Existem 2 modos de executar G-codes no FluidNC:
- A partir do comando no console
- A partir de um arquivo no sdcard


## Controlando a Máquina

### Movendo X e Y:
#### Homing e Ir pra Home
```
$H          ; Fazer Homing
G90         ; Modo Absoluto
G0 X0 Y0    ; Vai pra a Home (0,0)
M30         ; Finaliza o arquivo (se for um arquivo.nc)
```

#### Posicionar - Troca de Ferramenta
```
G90                 ; Modo Absoluto
G0 X-100.0 Y-102.5  ; Vai pra posição de trocar a ferramenta (-100.0, -102.5)
```

#### Posicionar - Liberar Área da Placa
```
G90                 ; Modo Absoluto
G0 X225.0 Y102.5    ; Vai pra posição de liberar área da placa (225.0, 102.5)
```



### Movendo Z
Como o eixo Z é um solenoide, ele fica normal quando z>=0 e abaixa quando z<0.

#### Abaixar e Levantar Solenoide
```
G90     ; Modo Absoluto
G0 Z-1  ; Abaixa o solenoide
G4 P1   ; Aguarda 1s
G0 Z0   ; Levanta o solenoide
```


### Controlando Spindle
O comando `M3` liga o spindle com a velocidade fornecida enquanto o comando `M5` desliga<br>
OBS: A fresadora só aceita velocidades de 10 mil a 60 mil RPM.
```
M3 S50000   ; Liga o spindle a 50.000 RPM
G4 P10      ; Aguarda 10 segundos (P em segundos no FluidNC)
M5          ; Desliga o spindle
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
