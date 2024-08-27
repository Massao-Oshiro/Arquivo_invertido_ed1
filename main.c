#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TAM_NOME 20
#define TAM_TEXTO 100000
#define TAM_PALAVRA 15

typedef struct ocorrencia {
    int posicao;
    struct ocorrencia* proximo;
} ocorrencia;

typedef struct Node {
    char palavra[TAM_PALAVRA];
    int frequencia;
    ocorrencia* primeira;
    ocorrencia* ultima;
    struct Node* prox;
} Node;

typedef struct arquivoInvertido {
    Node* primeiro;
    Node* ultimo;
} arquivoInvertido;

arquivoInvertido* criarArquivoInvertido() {
    arquivoInvertido* arquivoInv = (arquivoInvertido*) malloc(sizeof(arquivoInvertido));
    arquivoInv->primeiro = NULL;
    arquivoInv->ultimo = NULL;
    return arquivoInv;
}

void destroiArquivoInvertido(arquivoInvertido* A) {
    Node* pAux = A->primeiro;
    while (pAux != NULL) {
        Node* prox = pAux->prox;
        ocorrencia* pAux2 = pAux->primeira;
        while (pAux2 != NULL) {
            ocorrencia* prox2 = pAux2->proximo;
            free(pAux2);
            pAux2 = prox2;
        }
        free(pAux);
        pAux = prox;
    }
    A->primeiro = NULL;
    A->ultimo = NULL;
}

void removerAcentos(char* str) {
    char* src = str;
    char* dst = str;

    while (*src) {
        unsigned char c = *src;
        if ((c >= 192 && c <= 197) || (c >= 224 && c <= 229)) {  // A, a
            *dst = 'a';
        } else if ((c >= 200 && c <= 203) || (c >= 232 && c <= 235)) {  // E, e
            *dst = 'e';
        } else if ((c >= 204 && c <= 207) || (c >= 236 && c <= 239)) {  // I, i
            *dst = 'i';
        } else if ((c >= 210 && c <= 214) || (c >= 242 && c <= 246)) {  // O, o
            *dst = 'o';
        } else if ((c >= 217 && c <= 220) || (c >= 249 && c <= 252)) {  // U, u
            *dst = 'u';
        } else if (c == 199 || c == 231) {  // C, c
            *dst = 'c';
        } else if (c == 209 || c == 241) {  // N, n
            *dst = 'n';
        } else {
            *dst = *src;
        }
        src++;
        dst++;
    }
    *dst = '\0';
}

void adicionarPalavra(arquivoInvertido* A, char* palavra, int posicao) {
    Node* pAux = A->primeiro;

    while (pAux != NULL) {
        if (strcmp(pAux->palavra, palavra) == 0) {
            pAux->frequencia++;
            ocorrencia* nova = (ocorrencia*) malloc(sizeof(ocorrencia));
            nova->posicao = posicao;
            nova->proximo = NULL;
            if (pAux->ultima != NULL) {
                pAux->ultima->proximo = nova;
            }
            pAux->ultima = nova;
            return;
        }
        pAux = pAux->prox;
    }

    Node* novo = (Node*) malloc(sizeof(Node));
    strcpy(novo->palavra, palavra);
    ocorrencia* nova = (ocorrencia*) malloc(sizeof(ocorrencia));
    nova->posicao = posicao;
    nova->proximo = NULL;
    novo->primeira = nova;
    novo->ultima = nova;
    novo->frequencia = 1;
    novo->prox = NULL;

    if (A->primeiro == NULL) {
        A->primeiro = novo;
        A->ultimo = novo;
    } else {
        A->ultimo->prox = novo;
        A->ultimo = novo;
    }
}

void apresentarArquivoInvertido(char* texto, arquivoInvertido* A) {
    if (texto == NULL) {
        return;
    }

    removerAcentos(texto);

    int posicao = 0, j = 0;
    int comprimentoTexto = strlen(texto);
    char palavra[TAM_PALAVRA];

    for (int i = 0; i <= comprimentoTexto; i++) {
        if (isalnum(texto[i])) {
            if (j < TAM_PALAVRA - 1) {
                palavra[j++] = texto[i];
            }
        } else {
            if (j > 0) {
                palavra[j] = '\0';
                adicionarPalavra(A, palavra, posicao);
                posicao = i + 1;
                j = 0;
            }
            if (ispunct(texto[i])) {
                palavra[0] = texto[i];
                palavra[1] = '\0';
                adicionarPalavra(A, palavra, posicao);
                posicao = i + 1;
            }
        }
        if (isspace(texto[i])) {
            posicao = i + 1;
        }
    }

    printf("ARQUIVO INVERTIDO = \n\n");
    Node* pAux = A->primeiro;
    while (pAux != NULL) {
        printf("palavra = %s\nfrequencia = %d\nposicoes = ", pAux->palavra, pAux->frequencia);
        ocorrencia* pAux2 = pAux->primeira;
        while (pAux2 != NULL) {
            printf("%d ", pAux2->posicao);
            pAux2 = pAux2->proximo;
        }
        printf("\n\n");
        pAux = pAux->prox;
    }
}

void procurarNoTexto(arquivoInvertido* A, char* texto) {
    if (A->primeiro == NULL) {
        printf("Nao foi possivel procurar a palavra no texto.\n");
        return;
    }

    Node* pAux = A->primeiro;
    char palavra[TAM_PALAVRA];
    int flag = 0;

    printf("Digite a palavra desejada = ");
    scanf("%s", palavra);
    removerAcentos(palavra);
    printf("\n");

    while (pAux != NULL) {
        if (strcmp(palavra, pAux->palavra) == 0) {
            flag = 1;
            break;
        }
        pAux = pAux->prox;
    }

    if (flag) {
        printf("NUMERO DE OCORRENCIAS = %d\n", pAux->frequencia);
        ocorrencia* pOcor = pAux->primeira;
        int resposta = 0;
        while(pOcor != NULL) {
            printf("POSICAO = %d\n", pOcor->posicao);
            resposta = 0;
            int inicio = pOcor->posicao;

            while (inicio > 0 && texto[inicio - 1] != '.' && texto[inicio -1] != '\n') {
                inicio--;
            }

            int fim = pOcor->posicao;
            while (texto[fim] != '.' && texto[fim] != '\0' && texto[fim] != '\n') {
                fim++;
            }
            printf("FRASE COM A PALAVRA = ");

            for (int i = inicio; i <= fim && texto[i] != '\0'; i++) {
                printf("%c", texto[i]);
            }
            printf("\n\n");
            if(pOcor->proximo != NULL) {
                do {
                    printf("DESEJA VER A PROXIMA OCORRENCIA?\n1-SIM\n0-NAO\n");
                    scanf("%d", &resposta);
                    printf("\n");
                } while(resposta != 1 && resposta != 0);

                if(resposta == 1) {
                    pOcor = pOcor->proximo;
                } else {
                    break;
                }
            } else {
                break;
            }
        }
    } else {
        printf("PALAVRA NAO ENCONTRADA.\n");
    }
}

int main(void) {
    FILE* arquivo;
    int opcao = -1;
    char* nomeArquivo = (char*) calloc(TAM_NOME, sizeof(char));
    char* texto = (char*) calloc(TAM_TEXTO, sizeof(char));
    size_t bytesLidos;
    arquivoInvertido* arquivInv = criarArquivoInvertido();

    while (opcao != 4) {
        do {
            opcao = -1;
            printf("\n1 - LER UM ARQUIVO\n");
            printf("2 - APRESENTAR ARQUIVO INVERTIDO\n");
            printf("3 - PROCURAR\n");
            printf("4 - SAIR DO PROGRAMA\n");
            scanf("%d", &opcao);
        } while (1 != opcao && 2 != opcao && 3 != opcao && 4 != opcao);

        switch (opcao) {
            case 1:
                printf("DIGITE O NOME DO ARQUIVO = ");
                scanf("%s", nomeArquivo);
                printf("\n");
                arquivo = fopen(nomeArquivo, "r");
                if (arquivo != NULL) {
                    printf("ARQUIVO ABERTO COM SUCESSO\n\n");
                    bytesLidos = fread(texto, sizeof(char), TAM_TEXTO, arquivo);
                    if (bytesLidos) {
                        printf("ARQUIVO LIDO COM SUCESSO\n\n");
                    } else {
                        printf("ERRO AO LER ARQUIVO\n\n");
                    }
                } else {
                    printf("ERRO AO ABRIR ARQUIVO\n\n");
                }
                break;

            case 2:
                if (bytesLidos) {
                    apresentarArquivoInvertido(texto, arquivInv);
                } else {
                    printf("LEIA UM ARQUIVO ANTES\n\n");
                }
                break;

            case 3:
                procurarNoTexto(arquivInv, texto);
                break;

            case 4:
                if (arquivo != NULL) {
                    fclose(arquivo);
                }
                printf("SAINDO DO PROGRAMA\n\n");
                break;
        }
    }

    free(nomeArquivo);
    free(texto);
    destroiArquivoInvertido(arquivInv);
    free(arquivInv);
    getchar();
    return 0;
}
