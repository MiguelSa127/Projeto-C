#include <stdio.h>
#include <windows.h>
#include <mysql.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"

void limparBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void txtconfirm()
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h, FOREGROUND_GREEN);
}

void txterro()
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h, FOREGROUND_RED);
}

void txtnrm()
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h, 7);
}

void compra(MYSQL *conn)
{
    char confirmacao;
    int id[100];
    int quantidade[100];
    float subtotal[100];
    int estoque;
    float finaltotal;
    int lista = 0;
    char query[256];
    do
    {

        system("cls");

        printf("Realizar Compra:\n");
        printf("Insira a ID do produto:\n");
        scanf(" %d", &id[lista]);

        printf("Insira a Quantidade do produto:\n");
        scanf("%d", &quantidade[lista]);

        sprintf(query, "SELECT * FROM produtos WHERE id=%d", id[lista]);

        if (mysql_query(conn, query) != 0)
        {
            txterro();
            printf("Erro ao ler Produto: %s\n", mysql_error(conn));
            txtnrm();
            system("pause");
            return;
        }

        MYSQL_RES *result = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(result);

        float valor = atof(row[2]);
        subtotal[lista] = valor * quantidade[lista];
        int estoquebanco = atoi(row[3]);
        estoque = estoquebanco - quantidade[lista];

        mysql_free_result(result);

        finaltotal += subtotal[lista];

        sprintf(query, "UPDATE produtos SET estoque='%d' WHERE id=%d", estoque, id[lista]);

        if (mysql_query(conn, query) != 0)
        {
            txterro();
            printf("Erro ao Atualizar Estoque: %s\n", mysql_error(conn));
            txtnrm();
            system("pause");
            return;
        }

        printf("Deseja adicionar algum novo produto?(s/n)\n");
        lista++;

        scanf(" %c", &confirmacao);
    } while (confirmacao == 's' || confirmacao == 'S');

    printf("%-30s | %-10s | %-10s\n", "Nome", "Quantidade", "Subtotal");
    printf("------------------------------------------------------\n");

    for (int i = 0; i < lista; i++)
    {
        sprintf(query, "SELECT nome FROM produtos WHERE id=%d", id[i]);

        if (mysql_query(conn, query) != 0)
        {
            txterro();
            printf("Erro ao Listar: %s\n", mysql_error(conn));
            txtnrm();
            system("pause");
            return;
        }

        MYSQL_RES *result = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(result);

        printf("%-30s | %-10d | R$%-10.2f\n", row[0], quantidade[i], subtotal[i]);

        mysql_free_result(result);
    }
    printf("------------------------------------------------------\n");
    txtconfirm();
    printf("TOTAL: R$%.2f\n", finaltotal);
    txtnrm();
    system("pause");
}

void excluir(MYSQL *conn)
{
    int id;

    system("cls");

    printf("Digite a ID do produto a ser deletado\n");
    scanf("%d", &id);
    limparBuffer();

    char query[256];
    snprintf(query, sizeof(query), "DELETE FROM produtos WHERE id=%d;", id);

    if (mysql_query(conn, query) != 0)
    {
        txterro();
        printf("Erro ao Excluir Produto: %s\n", mysql_error(conn));
        txtnrm();
        system("pause");
        return;
    }
    txtconfirm();
    printf("Produto excluido com sucesso!\n");
    txtnrm();
    system("pause");
    listar(conn);
}

void editar(MYSQL *conn)
{
    int id;
    char nome[100];
    float valor;
    int estoque;

    system("cls");

    printf("Digite a ID do Produto a ser editado:\n");
    scanf("%d", &id);
    limparBuffer();

    printf("Digite o novo nome do Produto:\n");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = 0;

    printf("Digite o novo valor do Produto\n");
    scanf("%f", &valor);

    printf("Digite a nova quantidade no Estoque\n");
    scanf("%d", &estoque);

    char query[256];
    sprintf(query, "UPDATE produtos SET nome='%s', valor='%.2f', estoque='%d' WHERE id=%d", nome, valor, estoque, id);

    if (mysql_query(conn, query) != 0)
    {
        txterro();
        printf("Erro ao editar: %s\n", mysql_error(conn));
        txtnrm();
        system("pause");
        return;
    }
    txtconfirm();
    printf("Editado com Sucesso!\n");
    txtnrm();
    system("pause");
}

void listar(MYSQL *conn)
{
    if (mysql_query(conn, "SELECT * FROM produtos") != 0)
    {
        txterro();
        printf("Erro de leitura: %s\n", mysql_error(conn));
        txtnrm();
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    MYSQL_ROW row;
    int opcao2;

    system("cls");

    printf("\n%-5s | %-30s | %10s | %-10s\n", "ID", "Nome", "Preco", "Estoque");
    printf("------------------------------------------------------\n");

    while ((row = mysql_fetch_row(result)) != NULL)
    {
        printf("%-5s | %-30s | %10s | %-10s\n", row[0], row[1], row[2], row[3]);
    }

    mysql_free_result(result);

    printf("------------------------------------------------------\n");

    do
    {
        printf("[1] - Editar Produto | [2] - Excluir Produto | [0] - Sair\n");
        printf("Escolha sua Opcao:\n");
        scanf("%d", &opcao2);

        switch (opcao2)
        {
        case 1:
            limparBuffer();
            editar(conn);
            listar(conn);
            return;
        case 2:
            limparBuffer();
            excluir(conn);
            return;
        case 0:
            printf("Retornando...\n");
            system("pause");
            break;
            txterro();
        default:
            printf("OPCAO INVALIDA!\n");
            txtnrm();
        }
    } while (opcao2 != 0);
}

void adicionar(MYSQL *conn)
{
    char confirmacao;
    char nome[100];
    float valor;
    int estoque;

    system("cls");

    printf("--ADICIONAR UM NOVO PRODUTO--\n");

    printf("Insira o nome do Produto:\n");
    scanf(" %[^\n]", nome);

    printf("Insira o preco do Produto %s:\n", nome);
    scanf("%f", &valor);

    printf("Insira a quantidade do Produto %s no estoque:\n", nome);
    scanf("%d", &estoque);

    char query[256];
    sprintf(query, "INSERT INTO produtos (nome, valor, estoque) VALUES ('%s', '%.2f', '%d')", nome, valor, estoque);

    if (mysql_query(conn, query) != 0)
    {
        txterro();
        printf("Erro ao adicionar novo produto: %s\n", mysql_error(conn));
        txtnrm();
        return;
    }
    txtconfirm();
    printf("Produto adicionado com sucesso!\n");
    txtnrm();
    printf("Deseja adicionar outro Produto(s/n)?\n");
    scanf(" %c", &confirmacao);
    if (confirmacao == 's' || confirmacao == 'S')
    {
        adicionar(conn);
    }
}

void menu(MYSQL *conn)
{
    int opcao;
    do
    {

        system("cls");

        printf("\n---------POWER-SELL---------\n");
        printf("--------MENU INICIAL--------\n");
        printf("[1] - Adicionar novo Produto\n");
        printf("[2] - Listar Produtos\n");
        printf("[3] - Modo Caixa\n");
        printf("[0] - Sair\n");
        printf("----------------------------\n");
        printf("Insira sua opcao:\n");
        scanf(" %d", &opcao);

        switch (opcao)
        {
        case 1:
            limparBuffer();
            adicionar(conn);
            break;
        case 2:
            limparBuffer();
            listar(conn);
            break;
        case 3:
            limparBuffer();
            compra(conn);
            break;
        case 0:
            limparBuffer();
            printf("Saindo...\n");
            break;
            txterro();
        default:
            printf("OPCAO INVALIDA!\n");
            txtnrm();
        }
    } while (opcao != 0);
}
