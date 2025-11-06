#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Contato {
    char nome[50];
    char telefone[15];
   struct Contato* esquerda;
   struct Contato* direita;
} Contato;

Contato* inserir(Contato* raiz, Contato*novo);
Contato* buscar(Contato* raiz, const char* nome);
Contato* remover(Contato* raiz, const char* nome);  
Contato* EncontrarMinimo(Contato* raiz);
void ImprimirArvoreDeContatos(Contato* raiz);
void menu();

// função Comparação de  nomes
int compararNomes(char* nome1, char* nome2) {
    return strcmp(nome1, nome2);
}

// função principal
int main () {
    Contato* raiz = NULL;
    int opcao;

    do {
        menu();
        scanf("%d", &opcao);
        getchar(); // Limpar o buffer do teclado

        switch (opcao) {
            case 1: {
                Contato* novo = (Contato*)malloc(sizeof(Contato));
                printf("Digite o nome do contato: ");
                fgets(novo->nome, sizeof(novo->nome), stdin);
                novo->nome[strcspn(novo->nome, "\n")] = 0; // Remover o '\n'
                printf("Digite o telefone do contato: ");
                fgets(novo->telefone, sizeof(novo->telefone), stdin);
                novo->telefone[strcspn(novo->telefone, "\n")] = 0; // Remover o '\n'
                novo->esquerda = NULL;
                novo->direita = NULL;
                raiz = inserir(raiz, novo);
                break;
            }
            case 2: {
                char nome[50];
                printf("Digite o nome do contato a buscar: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = 0; // Remover o '\n'
                Contato* encontrado = buscar(raiz, nome);
                if (encontrado) {
                    printf("Contato encontrado: %s - %s\n", encontrado->nome, encontrado->telefone);
                } else {
                    printf("Contato nao encontrado.\n");
                }
                break;
            }
            case 3: {
                char nome[50];
                printf("Digite o nome do contato a remover: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = 0; // Remover o '\n'
                raiz = remover(raiz, nome);
                break;
            }
            case 4:
                ImprimirArvoreDeContatos(raiz);
                break;
            case 5:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 5); 
    return 0;
}

// função de inserir contato
Contato* inserir(Contato* raiz, Contato* novo) {
    if (raiz == NULL) {
        return novo;
    }
    if (compararNomes(novo->nome, raiz->nome) < 0) {
        raiz->esquerda = inserir(raiz->esquerda, novo);
    } else if (compararNomes(novo->nome, raiz->nome) > 0) {
        raiz->direita = inserir(raiz->direita, novo);
    } else {
        printf("Contato com esse nome ja existe.\n");
        free(novo);
    }
    return raiz;
}

// função de buscar contato
Contato* buscar(Contato* raiz, const char* nome) {
 if (raiz == NULL) {
    return NULL;
 
    }
    if (compararNomes(nome, raiz->nome) == 0) {
        return raiz;
    } else if (compararNomes(nome, raiz->nome) < 0) {
        return buscar(raiz->esquerda, nome);
    } else {
        return buscar(raiz->direita, nome);
    }


// Função auxiliar: encontrar o menor nó (para remoção)
Contato* encontrarMinimo(Contato* raiz) {
    while(raiz && raiz->esquerda != NULL) {
        raiz = raiz->esquerda;
    }
    return raiz;
} 