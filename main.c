#include <stdio.h>
#include <mysql.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "funcoes.h"

int main(){
    int confirmacao;
    MYSQL *conn = mysql_init(NULL);

    if (conn == NULL) {
        printf("Erro ao inicializar Mysql\n");
        system("pause");
        return 1;
    }

    if(mysql_real_connect(conn, DB_HOST, DB_USER, DB_PASSWORD, DB_NAME, atoi(DB_PORT), NULL, 0) == NULL) {
        printf("Erro ao Conectar: %s \n", mysql_error(conn));
        mysql_close(conn);
        system("pause");
        return 1;
    }

    printf("Conectou com sucesso!\n");
    system("pause");

    const char *sql_create =
    "CREATE TABLE IF NOT EXISTS produtos ("
    "id INT AUTO_INCREMENT PRIMARY KEY,"
    "nome VARCHAR(255),"
    "valor DECIMAL(10,2),"
    "estoque INT"
    ");";

    if (mysql_query(conn, sql_create) != 0) {
        printf("Erro ao criar tabela: %s \n", mysql_error(conn));
        system("pause");
        return 1;
    }
        
        menu(conn);

    mysql_close(conn);
    printf("Conexao Encerrada\n");
    system("pause");
    return 0;
}
