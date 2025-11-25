#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para um contato, organizada pelo nome (chave)
typedef struct Contato {
    char nome[50];
    char celular[15];
    struct Contato* esquerda;
    struct Contato* direita;
} Contato;

// funções protótipos
Contato* inserir(Contato* raiz, Contato* novo);
Contato* buscar(Contato* raiz, const char* nome);
Contato* remover(Contato* raiz, const char* nome);  
Contato* encontrarMinimo(Contato* raiz);
void ImprimirArvoreDeContatos(Contato* raiz);
void liberarArvore(Contato* raiz); 
void menu();
int compararNomes(const char* nome1, const char* nome2);

// Função para comparação de nomes (chave)
int compararNomes(const char* nome1, const char* nome2) {
    return strcmp(nome1, nome2);
}

// Implementação da função auxiliar: encontrar o menor nó (para remoção)
Contato* encontrarMinimo(Contato* raiz) {
    Contato* atual = raiz;
    while(atual && atual->esquerda != NULL) {
        atual = atual->esquerda;
    }
    return atual;
}

// Implementação da função de inserir contato
Contato* inserir(Contato* raiz, Contato* novo) {
    if (raiz == NULL) {
        return novo;
    }
    
    int comparacao = compararNomes(novo->nome, raiz->nome);

    if (comparacao < 0) {
        raiz->esquerda = inserir(raiz->esquerda, novo);
    } else if (comparacao > 0) {
        raiz->direita = inserir(raiz->direita, novo);
    } else {
        printf("ERRO: Contato com o nome '%s' ja existe. Nao inserido.\n", novo->nome);
        free(novo);
    }
    return raiz;
}

// Implementação da função de buscar contato
Contato* buscar(Contato* raiz, const char* nome) {
    if (raiz == NULL) {
        return NULL;
    }
    
    int comparacao = compararNomes(nome, raiz->nome);

    if (comparacao == 0) {
        return raiz;
    } else if (comparacao < 0) {
        return buscar(raiz->esquerda, nome);
    } else {
        return buscar(raiz->direita, nome);
    }
}

// Implementação da função de remover contato
Contato* remover(Contato* raiz, const char* nome) {
    if (raiz == NULL) {
        printf("ERRO: Contato '%s' nao encontrado para remocao.\n", nome);
        return raiz;
    }

    int comparacao = compararNomes(nome, raiz->nome);

    if (comparacao < 0) {
        raiz->esquerda = remover(raiz->esquerda, nome);
    } else if (comparacao > 0) {
        raiz->direita = remover(raiz->direita, nome);
    } else {
        // Nó a ser removido ENCONTRADO

        // Caso 1: Nó com zero ou um filho
        Contato* temp;
        if (raiz->esquerda == NULL) {
            temp = raiz->direita;
            free(raiz);
            printf("Contato '%s' removido com sucesso.\n", nome);
            return temp;
        } else if (raiz->direita == NULL) {
            temp = raiz->esquerda;
            free(raiz);
            printf("Contato '%s' removido com sucesso.\n", nome);
            return temp;
        }

        // Caso 2: Nó com dois filhos
        temp = encontrarMinimo(raiz->direita);

        // Copia o conteúdo (nome e celular) do sucessor para o nó atual
        strcpy(raiz->nome, temp->nome);
        strcpy(raiz->celular, temp->celular);

        // Remove o sucessor in-ordem da subárvore direita
        raiz->direita = remover(raiz->direita, temp->nome);
    }

    return raiz;
}

// Implementação da função de imprimir contatos em ordem
void ImprimirArvoreDeContatos(Contato* raiz) {
    if (raiz != NULL) {
        ImprimirArvoreDeContatos(raiz->esquerda);
        printf("- Nome: %s, Celular: %s\n", raiz->nome, raiz->celular);
        ImprimirArvoreDeContatos(raiz->direita);
    }
}

// Implementação da função de liberar memória de forma recursiva da árvore
void liberarArvore(Contato* raiz) {
    if (raiz == NULL) {
        return;
    }
    // Libera recursivamente os filhos esquerdo e direito
    liberarArvore(raiz->esquerda);
    liberarArvore(raiz->direita);
    
    // Libera o nó atual
    free(raiz);
}

// Implementação da função de menu
void menu() {
    printf("\nMenu:\n");
    printf("1. Adicionar contato\n");
    printf("2. Buscar contato\n");
    printf("3. Remover contato\n");
    printf("4. Listar contatos\n");
    printf("5. Sair\n");
    printf("Escolha uma opcao: ");
}

// Função principal
int main () {
    Contato* raiz = NULL;
    int opcao;

    do {
        menu();
        if (scanf("%d", &opcao) != 1) {
            opcao = 0;
        }
        getchar();

        switch (opcao) {
            case 1: {
                Contato* novo = (Contato*)malloc(sizeof(Contato));
                if (novo == NULL) {
                    printf("Erro de alocacao de memoria.\n");
                    break;
                }
                
                printf("Digite o nome do contato (MAIUSCULO recomendado): ");
                fgets(novo->nome, sizeof(novo->nome), stdin);
                novo->nome[strcspn(novo->nome, "\n")] = 0; 

                printf("Digite o celular do contato: ");
                fgets(novo->celular, sizeof(novo->celular), stdin); 
                novo->celular[strcspn(novo->celular, "\n")] = 0; 
                
                novo->esquerda = NULL;
                novo->direita = NULL;
                
                raiz = inserir(raiz, novo);
                break;
            }
            case 2: {
                char nome[50];
                printf("Digite o nome do contato a buscar: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = 0; 
                
                Contato* encontrado = buscar(raiz, nome);
                
                if (encontrado) {
                    printf("Contato encontrado: %s - %s\n", encontrado->nome, encontrado->celular); 
                } else {
                    printf("Contato nao encontrado.\n");
                }
                break;
            }
            case 3: {
                char nome[50];
                printf("Digite o nome do contato a remover: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = 0; 
                
                raiz = remover(raiz, nome);
                break;
            }
            case 4: {
                printf("--- Lista de Contatos (Ordem Alfabetica) ---\n");
                ImprimirArvoreDeContatos(raiz);
                printf("--------------------------------------------\n");
                break;
            }
            case 5:
                printf("Saindo...\n");
                liberarArvore(raiz); 
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 5);   
    return 0;
}
