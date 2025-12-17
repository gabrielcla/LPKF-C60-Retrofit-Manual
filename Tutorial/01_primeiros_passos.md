# Primeiros Passos 

## 1) Conhecendo a Máquina

#### Principais Partes da Máquina
- Eixos X e Y - Controlados por motores de passo
- Eixo Z - Controlado por solenoide
- Spindle - Motor efetivamente realiza a usinagem
- Limitador de Profundidade - Regula o quão profundo a ferramenta entra na placa
    
#### Orientação e Posicionamento

<img src="assets/manual/orientacoes.png" width="300"><br>
- Posições:
    - O: Origem da Máquina (0,0)
    - T: Posição para troca de ferramenta
    - S: Posição para liberar aréa da placa
- Limites
    - Eixo X: de -100.0 até 225.0
    - Eixo Y: de -102.5 até 102.5



## 2) Conhecendo o Firmware FluidNC
### 2.1) Como se comunicar com o FluidNC
Ao ligar a placa mks vai aparecer a rede FLuidNC de senha 12345678, conecte-se a ela.  
Após isso, ao abrir o navegador ele ja deve redirecionar para a página com a interface web, mas se não redirecionar acesse fluidnc.local ou 192.168.0.1 para acessa-la.

### 2.2) Interface WEB
<img src="assets/WebUI/Web.png" width="500">

A interface WEB é dividida nas seguintes regiões:

- Região A – Menu Superior: Fornece acesso às seções de configuração geral do FluidNC, permitindo visualizar e editar o arquivo config.yaml, no qual estão definidas todas as parametrizações do firmware.
- Região B – Controle de Posicionamento: Apresenta, em tempo real, as coordenadas atuais dos eixos e disponibiliza controles manuais para movimentação da máquina.
- Região C – Controle de Estado: Responsável pelo gerenciamento do estado operacional. O FluidNC inicia, por padrão, no modo Alarm, impedindo qualquer movimento por segurança. Nessa região, é possível destravar a máquina, alterando o estado para Idle.
- Região D – Gerenciamento de Arquivos: Exibe o conteúdo armazenado no cartão SD e permite enviar, organizar e executar arquivos G-code diretamente pela interface.
- Região E – Console de Comunicação: Mostra o log de comunicação entre o firmware e o host, permitindo o envio manual de comandos em G-code e o monitoramento da execução em tempo real.


---

> **Aviso sobre material de terceiros:**  
> Algumas imagens utilizadas neste tutorial foram extraídas do manual original da fresadora **LPKF ProtoMat C60**, pertencente à **LPKF Laser & Electronics AG**.  
> O uso dessas imagens tem finalidade **exclusivamente educacional**, sem fins comerciais.  
> Todos os direitos pertencem aos seus respectivos autores.  
---

➡️ Próximo tutorial:  
[02 - Operações Básicas - Como Operar a Fresadora](02_operacoes_basicas.md)

[⬅ Voltar para Tutoriais](../README.md#📘-tutoriais)


