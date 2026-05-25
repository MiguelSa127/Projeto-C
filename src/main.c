#include <stdio.h>
#include <mysql.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "config.h"
#include "funcoes.h"

int main()
{
    SetConsoleTitle("PowerSell");
    system("mode con: cols=80 lines=30");
    int confirmacao;
    MYSQL *conn = mysql_init(NULL);

    if (conn == NULL)
    {
        txterro();
        printf("Erro ao inicializar Mysql\n");
        txtnrm();
        system("pause");
        return 1;
    }

    if (mysql_real_connect(conn, DB_HOST, DB_USER, DB_PASSWORD, DB_NAME, atoi(DB_PORT), NULL, 0) == NULL)
    {
        txterro();
        printf("Erro ao Conectar: %s \n", mysql_error(conn));
        txtnrm();
        mysql_close(conn);
        system("pause");
        return 1;
    }
    txtconfirm();
    printf("Conectou com sucesso!\n");
    txtnrm();
    system("pause");

    const char *sql_create =
        "CREATE TABLE IF NOT EXISTS produtos ("
        "id INT AUTO_INCREMENT PRIMARY KEY,"
        "nome VARCHAR(255),"
        "valor DECIMAL(10,2),"
        "estoque INT"
        ");";

    if (mysql_query(conn, sql_create) != 0)
    {
        txterro();
        printf("Erro ao criar tabela: %s \n", mysql_error(conn));
        txtnrm();
        system("pause");
        return 1;
    }

    menu(conn);

    mysql_close(conn);
    txtconfirm();
    printf("Conexao Encerrada\n");
    txtnrm();
    system("pause");
    return 0;
}
