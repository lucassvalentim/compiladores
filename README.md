# Nome dos participantes:

1) **Lucas Henrique Valentim Rocha**\
2) **Isabelle Cristine do Carmo Matos**

# Front-end do Compilador --- Linguagem P

Este projeto implementa o **front-end completo** de um compilador para a
**linguagem P**, incluindo:

- **Analisador Léxico**
- **Analisador Sintático**
- **Analisador Semântico** (verifica regras semânticas
  que envolvem declarações)
- **Gerador de Código Intermediário**, representado por uma **AST
  (Abstract Syntax Tree)**


---

## Como executar

### 1. Compilação

Para compilar o projeto, basta executar:

```bash
make
```

Isso irá gerar um executável chamado **`main`** na pasta raiz do
projeto.

---

### 2. Execução

Para rodar o compilador (front-end) sobre um arquivo `.p`, utilize:

```bash
./main inputs/<nome_do_arquivo>.p
```

O resultado da análise léxica, sintática, semântica e a representação da
**AST** será impresso diretamente no **terminal**.

---

### 3. Salvando a saída em arquivo

Para salvar toda a saída (tokens, árvore sintática e mensagens
semânticas) em um arquivo `.txt`, execute:

```bash
./main inputs/<nome_do_arquivo>.p > outputs/<nome_arquivo_saida>.txt
```

---

## 📁 Estrutura de diretórios

O projeto contém arquivos de exemplo já prontos para teste:

    inputs/   → arquivos de entrada (.p)
    outputs/  → arquivos de saída (.txt)

Para cada arquivo dentro de `inputs`, há uma saída correspondente em
`outputs`, contendo:

- Tokens identificados
- Estrutura sintática
- Verificações semânticas
- AST resultante

**Exemplo:**

    inputs/soma.p  →  outputs/soma.txt

---

## Exemplo de uso

```bash
./main inputs/soma.p
```

ou salvando o resultado:

```bash
./main inputs/soma.p > outputs/soma.txt
```

---

## Requisitos

- Compilador **C++** compatível com o padrão **C++11** ou superior\
- **make** instalado no sistema

---

## 📄 Licença

Este projeto foi desenvolvido para fins acadêmicos como demonstração
prática do funcionamento do front-end de um compilador.
