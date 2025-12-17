# 🛠️ LPKF ProtoMat C60 (Retrofit Educacional) — Guia de Uso e Documentação Técnica

Repositório educacional com **tutoriais em Markdown e códigos** utilizados na **operação, configuração e retrofit** de uma fresadora de PCBs **LPKF ProtoMat C60**, desenvolvidos no contexto de um **Trabalho de Conclusão de Curso (TCC)**.

O objetivo é servir como **guia prático** para **alunos, professores e técnicos**, documentando o fluxo real utilizado para produzir placas de circuito impresso — do preparo dos arquivos à usinagem final. As soluções apresentadas refletem o contexto e os recursos disponíveis no projeto e **podem (e devem) ser aprimoradas** ao longo do tempo.

---
## 📘 Tutoriais

Os tutoriais estão organizados na pasta `Tutorial/` e devem ser lidos **preferencialmente em ordem**:

[**01 - Conhecendo a máquina e o firmware FluidNC**](Tutorial/01_primeiros_passos.md)  
[**02 - Operações básicas com a máquina**](Tutorial/02_operacoes_basicas.md)  
[**03 - Controlando a máquina com G-codes**](Tutorial/03_gcodes_basico.md)  
[**04 - Gerando G-codes a partir de um projeto CAD**](Tutorial/04_gcodes_cad.md)  
[**05 - Usinando a partir dos G-codes**](Tutorial/05_usinagem.md)  


---

## 💻 Códigos e Configurações

A pasta `Retrofit/` contém:

* Arquivo de configuração do Firmware FluidNC
* Código para o Arduino Uno funcionar como ESC do spindle
* Mais informações sobre o retrofit

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
│   ├── 03_gcodes_basico.md
│   ├── 04_gcodes_cad.md
│   ├── 05_usinagem.md
├── Retrofit/
│   ├── ESC - Arduino Uno/
│   ├── FluidNC - MKS DLC32/
├── Projeto Exemplo/
│   ├── KiCAD
│   ├── FlatCAM
│   ├── G-codes
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
## 🎓 Contexto Acadêmico
Este repositório foi desenvolvido como parte de um **Trabalho de Conclusão de Curso (TCC)** em Engenharia da Computação, com foco na recuperação, retrofit e documentação de uma fresadora de PCBs para uso didático em laboratório.
O objetivo é facilitar a **transferência de conhecimento** e permitir que futuros alunos, professores e técnicos possam operar, manter e evoluir a máquina.

## 📜 Licença

Este projeto é distribuído sob a licença **MIT**.

Apesar de ter sido desenvolvido com **finalidade educacional**, a licença permite o **uso, modificação e redistribuição**, inclusive para outros fins, desde que mantido o aviso de copyright.

Consulte o arquivo [`LICENSE`](./LICENSE) para os termos completos.
