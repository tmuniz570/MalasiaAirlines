# Sistema de Reservas - Malaysia Airlines

---

Este projeto é um sistema de gerenciamento de reservas de passagens aéreas para a **Malaysia Airlines**, desenvolvido em C como parte de um trabalho acadêmico (P2) da UFAL. Ele permite cadastrar aviões, fazer e consultar reservas, e buscar passageiros.

## Visão Geral

O sistema simula funcionalidades básicas de uma companhia aérea, utilizando estruturas de dados como árvores AVL para os aviões e listas duplamente encadeadas para as reservas de passageiros em cada voo. Os dados são persistidos em arquivos binários (`voos` e `reservas`).

## Funcionalidades

* **Cadastro de Aviões**: Permite adicionar novos aviões ao sistema, especificando o número do voo e a capacidade total de poltronas (entre 1 e 60). O sistema verifica se o avião já está cadastrado.
* **Reserva de Passagens**: Permite reservar uma passagem para um passageiro em um voo específico. O sistema verifica a existência do voo e a disponibilidade de poltronas.
* **Consultas**:
    * **Aviões Cadastrados**: Lista todos os aviões no sistema, mostrando o número do voo e a capacidade total de poltronas.
    * **Dados do Avião**: Exibe todas as reservas (passageiros) de um voo específico, ordenadas alfabeticamente pelo nome do passageiro, além do total de reservas e poltronas livres.
    * **Por Nome do Passageiro**: Busca e lista todos os voos em que um determinado passageiro possui reserva.
* **Persistência de Dados**: Os dados de aviões e reservas são salvos em arquivos binários, permitindo que o estado do sistema seja mantido entre as execuções.
* **Balanceamento de Árvore (AVL)**: A estrutura de árvores AVL é utilizada para armazenar os aviões, garantindo que as operações de busca, inserção e remoção sejam eficientes, mantendo a árvore balanceada automaticamente através de rotações (esquerda e direita).
* **Ordenação de Reservas**: As reservas de cada voo são ordenadas alfabeticamente pelo nome do passageiro ao serem exibidas, utilizando o algoritmo Quick Sort.

## Estruturas de Dados

* **`AVIAO` (Árvore AVL)**:
    * `num`: Número do voo (identificador único do avião).
    * `reservas`: Ponteiro para o início da lista duplamente encadeada de reservas associadas a este avião.
    * `poltronas_total`: Capacidade total de poltronas do avião.
    * `poltronas_livre`: Número de poltronas ainda disponíveis.
    * `alte`, `altd`: Alturas das subárvores esquerda e direita, usadas para o balanceamento AVL.
    * `esq`, `dir`: Ponteiros para os filhos esquerdo e direito na árvore AVL.
    * `pai`: Ponteiro para o nó pai na árvore AVL.
* **`RESERVA` (Lista Duplamente Encadeada)**:
    * `nome`: Nome do passageiro.
    * `num_voo`: Número do voo ao qual a reserva pertence.
    * `prox`, `ant`: Ponteiros para o próximo e anterior elemento na lista de reservas.

## Como Compilar e Rodar

1.  **Compilação**: Use um compilador C (como GCC) para compilar o código-fonte.
    ```bash
    gcc main.c -o reservas
    ```
2.  **Execução**:
    * **Windows**:
        ```bash
        reservas.exe
        ```
    * **Linux**:
        ```bash
        ./reservas
        ```

## Requisitos do Sistema

* Compilador C (GCC ou similar).
* Sistema operacional Windows ou Linux.

## Autores

* **Kin Torres**
* **Thiago Brandão**

---
