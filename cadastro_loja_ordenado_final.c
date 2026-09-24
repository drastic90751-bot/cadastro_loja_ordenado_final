#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define VERDE   "\033[32m"
#define VERM    "\033[31m"
#define AZUL    "\033[36m"
#define AMARELO "\033[33m"

typedef struct {
    int id;
    char produto[50];
    char categoria[30];
    float preco;
    int quantidade;
    char fornecedor[50];
} Produto;

/* === Cabecalhos === */
void criarArquivo();
void listarProdutos();
void ordenarProdutos(int modo);
int carregarProdutos(Produto lista[]);
void exibirTabela(Produto lista[], int total);
void salvarOrdenado(Produto lista[], int total);
void limparTela();

/* === Funcoes basicas === */
void limparTela() {
    system("clear || cls");
}

/* Cria o arquivo inicial com dados de exemplo */
void criarArquivo() {
    FILE *fp = fopen("cadastro_loja.txt", "w");
    if (!fp) { printf(VERM "Erro ao criar arquivo!\n" RESET); return; }

    Produto itens[] = {
        {1, "Camiseta Basica",  "Vestuario",       49.90, 30, "Moda Brasil"},
        {2, "Mouse Optico USB", "Informatica",     59.99, 15, "TecnoShop"},
        {3, "Shampoo 300ml",    "Higiene Pessoal", 22.50, 40, "Beleza Pura"},
        {4, "Cafe 500g",        "Alimentos",       18.00, 25, "Graos do Sul"},
        {5, "Notebook X100",    "Informatica",    3200.00, 8, "TecnoShop"},
        {6, "Tenis Esportivo",  "Vestuario",       199.90, 12, "Sport Brasil"},
        {7, "Sabonete Neutro",  "Higiene Pessoal",   5.99, 80, "Beleza Pura"}
    };

    for (int i = 0; i < 7; i++) {
        fprintf(fp, "%d;%s;%s;%.2f;%d;%s\n",
                itens[i].id, itens[i].produto, itens[i].categoria,
                itens[i].preco, itens[i].quantidade, itens[i].fornecedor);
    }

    fclose(fp);
    printf(VERDE "Arquivo criado com sucesso!\n" RESET);
}

/* Le o arquivo e carrega os dados no vetor de estruturas */
int carregarProdutos(Produto lista[]) {
    FILE *fp = fopen("cadastro_loja.txt", "r");
    if (!fp) {
        printf(VERM "Arquivo nao encontrado! Use a opcao 1 para cria-lo.\n" RESET);
        return 0;
    }

    char linha[300];
    int count = 0;
    while (fgets(linha, sizeof(linha), fp)) {
        sscanf(linha, "%d;%49[^;];%29[^;];%f;%d;%49[^\n]",
               &lista[count].id, lista[count].produto, lista[count].categoria,
               &lista[count].preco, &lista[count].quantidade, lista[count].fornecedor);
        count++;
    }

    fclose(fp);
    return count;
}

/* Exibe os produtos em formato de tabela alinhada */
void exibirTabela(Produto lista[], int total) {
    printf(BOLD AZUL "\n%-5s %-22s %-15s %-10s %-10s %-20s\n" RESET,
           "ID", "Produto", "Categoria", "Preco", "Qtde", "Fornecedor");
    printf(AZUL "-----------------------------------------------------------------------------\n" RESET);

    for (int i = 0; i < total; i++) {
        printf("%-5d %-22s %-15s %-10.2f %-10d %-20s\n",
               lista[i].id, lista[i].produto, lista[i].categoria,
               lista[i].preco, lista[i].quantidade, lista[i].fornecedor);
    }
}

/* Pergunta ao usuario se deseja gravar a lista ordenada de volta no arquivo */
void salvarOrdenado(Produto lista[], int total) {
    char opcao;
    printf(AMARELO "\nDeseja salvar esta lista ordenada no arquivo (S/N)? " RESET);
    scanf(" %c", &opcao);

    if (toupper(opcao) == 'S') {
        FILE *fp = fopen("cadastro_loja.txt", "w");
        for (int i = 0; i < total; i++) {
            fprintf(fp, "%d;%s;%s;%.2f;%d;%s\n",
                    lista[i].id, lista[i].produto, lista[i].categoria,
                    lista[i].preco, lista[i].quantidade, lista[i].fornecedor);
        }
        fclose(fp);
        printf(VERDE "\nArquivo atualizado com sucesso!\n" RESET);
    } else {
        printf(VERM "\nAlteracoes nao salvas.\n" RESET);
    }
}

/* Ordena os produtos usando Bubble Sort, conforme o modo escolhido */
void ordenarProdutos(int modo) {
    Produto lista[200];
    int total = carregarProdutos(lista);
    if (total == 0) return;

    Produto temp;

    switch (modo) {
        case 1: /* Nome A-Z */
            for (int i = 0; i < total - 1; i++)
                for (int j = 0; j < total - i - 1; j++)
                    if (strcmp(lista[j].produto, lista[j + 1].produto) > 0) {
                        temp = lista[j]; lista[j] = lista[j + 1]; lista[j + 1] = temp;
                    }
            printf(VERDE "\nProdutos ordenados por nome (A-Z):\n" RESET);
            break;

        case 2: /* Nome Z-A */
            for (int i = 0; i < total - 1; i++)
                for (int j = 0; j < total - i - 1; j++)
                    if (strcmp(lista[j].produto, lista[j + 1].produto) < 0) {
                        temp = lista[j]; lista[j] = lista[j + 1]; lista[j + 1] = temp;
                    }
            printf(VERDE "\nProdutos ordenados por nome (Z-A):\n" RESET);
            break;

        case 3: /* Preco crescente */
            for (int i = 0; i < total - 1; i++)
                for (int j = 0; j < total - i - 1; j++)
                    if (lista[j].preco > lista[j + 1].preco) {
                        temp = lista[j]; lista[j] = lista[j + 1]; lista[j + 1] = temp;
                    }
            printf(VERDE "\nProdutos ordenados por preco (menor -> maior):\n" RESET);
            break;

        case 4: /* Preco decrescente */
            for (int i = 0; i < total - 1; i++)
                for (int j = 0; j < total - i - 1; j++)
                    if (lista[j].preco < lista[j + 1].preco) {
                        temp = lista[j]; lista[j] = lista[j + 1]; lista[j + 1] = temp;
                    }
            printf(VERDE "\nProdutos ordenados por preco (maior -> menor):\n" RESET);
            break;

        case 5: /* Categoria + Nome */
            for (int i = 0; i < total - 1; i++)
                for (int j = 0; j < total - i - 1; j++) {
                    int cmp = strcmp(lista[j].categoria, lista[j + 1].categoria);
                    if (cmp > 0 || (cmp == 0 && strcmp(lista[j].produto, lista[j + 1].produto) > 0)) {
                        temp = lista[j];
                        lista[j] = lista[j + 1];
                        lista[j + 1] = temp;
                    }
                }
            printf(VERDE "\nProdutos ordenados por categoria (A-Z) e nome (A-Z):\n" RESET);
            break;

        default:
            printf(VERM "Modo invalido!\n" RESET);
            return;
    }

    exibirTabela(lista, total);
    salvarOrdenado(lista, total);
}

/* Le o arquivo e mostra a tabela sem reordenar */
void listarProdutos() {
    Produto lista[200];
    int total = carregarProdutos(lista);
    if (total > 0) exibirTabela(lista, total);
}

/* === Menu principal === */
int main() {
    int opcao;

    do {
        limparTela();
        printf(BOLD AMARELO "\n========== CADASTRO DE LOJA ==========\n" RESET);
        printf("1  - Criar arquivo inicial\n");
        printf("2  - Listar produtos\n");
        printf("8  - Ordenar produtos por nome (A-Z)\n");
        printf("9  - Ordenar produtos por nome (Z-A)\n");
        printf("10 - Ordenar produtos por preco (menor -> maior)\n");
        printf("11 - Ordenar produtos por preco (maior -> menor)\n");
        printf("12 - Ordenar por categoria e nome (A-Z)\n");
        printf("0  - Sair\n");
        printf(AZUL "---------------------------------------\n" RESET);
        printf("Escolha: ");
        scanf("%d", &opcao);
        limparTela();

        switch (opcao) {
            case 1:  criarArquivo(); break;
            case 2:  listarProdutos(); break;
            case 8:  ordenarProdutos(1); break;
            case 9:  ordenarProdutos(2); break;
            case 10: ordenarProdutos(3); break;
            case 11: ordenarProdutos(4); break;
            case 12: ordenarProdutos(5); break;
            case 0:  printf(AMARELO "Encerrando programa...\n" RESET); break;
            default: printf(VERM "Opcao invalida!\n" RESET);
        }

        if (opcao != 0) {
            printf("\nPressione ENTER para continuar...");
            getchar();
            getchar();
        }

    } while (opcao != 0);

    return 0;
}
