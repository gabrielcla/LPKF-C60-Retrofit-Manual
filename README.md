# 🛠️ Fresadora PCB LPKF ProtoMat C60 – Guia de Uso e Retrofit Educacional

Repositório educacional com **tutoriais em Markdown e códigos** utilizados na **operação, configuração e retrofit** de uma fresadora de PCBs **LPKF ProtoMat C60**, desenvolvidos no contexto de um **Trabalho de Conclusão de Curso (TCC)**.

O objetivo é servir como **guia prático** para **alunos, professores e técnicos**, documentando o fluxo real utilizado para produzir placas de circuito impresso — do preparo dos arquivos à usinagem final. As soluções apresentadas refletem o contexto e os recursos disponíveis no projeto e **podem (e devem) ser aprimoradas** ao longo do tempo.



## 📖 Visão Geral

Este repositório documenta a recuperação e o uso prático de uma fresadora de PCBs **LPKF ProtoMat C60**, incluindo instruções operacionais, boas práticas de laboratório e códigos utilizados no processo de retrofit.

O material foi pensado para facilitar a continuidade do uso da máquina em ambiente acadêmico, reduzindo a dependência de conhecimento informal e centralizando a documentação técnica.

---
## 📘 Tutoriais

Os tutoriais estão organizados na pasta `Tutorial/` e devem ser lidos **preferencialmente em ordem**:

[**1 - Primeiros Passos**](Tutorial/01_primeiros_passos.md) - Conhecendo a máquina e o firmware FluidNC  
[**2 - Operações Básicas - Como Operar a Fresadora**](Tutorial/02_operacoes_basicas.md) - Operações básicas com a máquina  
[**3 - Usando GCodes e Controlando a Máquina**](Tutorial/03_usando_gcodes.md) - Criando as instruções para a usinagem  
[**4 - Usinando a partir de G-Codes**](Tutorial/04_usinagem.md) - Como usinar uma PCB  
[**5 - Problemas Comuns e Soluções**](Tutorial/05_problemas_comuns.md) - Principais problemas 


---

## 💻 Códigos e Configurações

A pasta `Retrofit/` contém:

* Arquivo de configuração do Fiwmware
* Código para o arduino uno funcionar como ESC do spindle

Esses códigos fazem parte do processo de retrofit e estão disponíveis para **consulta, estudo e melhoria**.

---


## 📂 Estrutura do Repositório

```text
.
├── README.md
├── Tutorial/
│   ├── assets/
│   ├── 01_primeiros_passos.md
│   ├── 02_operacoes_basicas.md
│   ├── 03_gcodes.md
│   ├── 04_usinagem.md
│   └── 05_problemas_comuns.md
├── Retrofit/
│   ├── ESC - Arduino Uno/
│   ├── FluidNC - MKS DLC32/
├── Projeto Exemplo/
├── G-Codes/
└── LICENSE
```

---


## ⚠️ Segurança

> ⚠️ **Atenção:** A fresadora é um equipamento potencialmente perigoso.

* Utilize sempre **óculos de proteção**
* Não toque na ferramenta com a máquina ligada
* Prenda cabelos longos e não use camisa de mangas compridas
* Nunca opere a máquina sem supervisão
* Siga rigorosamente os procedimentos descritos nos tutoriais

O autor **não se responsabiliza por danos materiais ou físicos** decorrentes do uso inadequado das informações aqui apresentadas.

---

## 🤝 Contribuições

Sugestões de melhoria são bem-vindas.

Caso queira contribuir:

* Abra uma *issue*
* Envie um *pull request*
* Ou faça um *fork* do projeto

A ideia é que este repositório evolua e se torne um **manual de referência** para uso contínuo da máquina.

---

## 📜 Licença

Este projeto é disponibilizado para fins **educacionais**.

Consulte o arquivo `LICENSE` para mais detalhes sobre uso, modificação e distribuição.
