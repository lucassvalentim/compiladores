# Nome dos participantes:

    1) Lucas Henrique Valentim Rocha

    2) Isabelle Cristine do Carmo Matos

# Analisador Léxico e Sintático — Linguagem P

Este projeto implementa o **analisador léxico e sintático** de um compilador para a **linguagem P**.
O analisador é responsável por identificar e classificar os tokens presentes no código-fonte, servindo como a primeira etapa do processo de compilação.

---

## Como executar

### 1. Compilação

Para compilar o projeto, basta executar o comando:

```bash
make
```

Esse comando irá gerar um executável chamado **`main`** na pasta raiz do projeto.

---

### 2. Execução

Para executar o analisador léxico, utilize o seguinte formato:

```bash
./main inputs/<nome_do_arquivo>.p
```

O resultado da análise será impresso diretamente no **terminal**.

---

### 3. Salvando a saída em arquivo

Caso queira salvar a saída em um arquivo `.txt` dentro da pasta `outputs`, execute o comando:

```bash
./main inputs/<nome_do_arquivo>.p > outputs/<nome_arquivo_saida>.txt
```

---

## 📁 Estrutura de diretórios

O projeto já contém exemplos prontos para teste:

```
inputs/   → arquivos de entrada (.p)
outputs/  → arquivos de saída (.txt)
```

Para cada arquivo de entrada na pasta `inputs`, há um arquivo de saída correspondente na pasta `outputs`, contendo o resultado gerado pelo analisador.

**Exemplo:**

```
inputs/soma.p  →  outputs/soma.txt
```

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

- Compilador **C++** compatível com o padrão **C++11** ou superior
- **make** instalado no sistema

---

## 📄 Licença

Este projeto é de uso acadêmico e foi desenvolvido para fins de estudo e demonstração de técnicas de análise léxica.
