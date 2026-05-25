#include <mysql.h>

void limparBuffer();
void menu(MYSQL *conn);
void adicionar(MYSQL *conn);
void listar(MYSQL *conn);
void editar(MYSQL *conn);
void excluir(MYSQL *conn);
void compra(MYSQL *conn);
void txterro();
void txtconfirm();
void txtnrm();