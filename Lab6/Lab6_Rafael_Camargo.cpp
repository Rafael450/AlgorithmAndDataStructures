/* Rafael Camargo                          */
/* Turma 3                                 */
/*                                         */
/* Exercicio 6: Fibra Otica                */

/* Programa compilado com gcc (Rev9, Built by MSYS2 project) 10.2.0     */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define inf 2147483646

//Typedefs e structs
typedef char frase[11];

typedef struct aresta aresta;
struct aresta
{
    int peso;
    int destino;
};

typedef struct arestaLista arestaLista;
struct arestaLista
{
    aresta elementos[151];
    int size;
};

typedef struct vertice vertice;
struct vertice
{
    frase nome;
    arestaLista vizinhos;
    int chave;
    int antecessor;
    bool visitado;
};

typedef struct verticeLista verticeLista;
struct verticeLista
{
    vertice elementos[151];
    int size;
};

void InsertAresta(arestaLista *list, aresta x) //Funcão de inserir numa lista de aresta
{
    list->elementos[list->size] = x;
    list->size++;
}

void InsertVertice(verticeLista *vertices, vertice x) //Funcão de inserir numa lista de vertices
{
    vertices->elementos[vertices->size] = x;
    vertices->size++;
}

int EncontraIndice(verticeLista *vertices, frase nome) //Funcão de encontrar indice de determinada string
{
    for(int i = 0; i < vertices->size; i++)
        if(strcmp(nome, vertices->elementos[i].nome) == 0)
            return i;
    return -1; // -1 é o valor retornado caso não encontre;
}

void PrimAlg(verticeLista *vertices)  // Algoritmo de Prim
{
    int numVert = vertices->size;
    vertices->elementos[0].chave = 0;

    while(numVert != 0)
    {
        int dmin = inf, menorVert;

        for(int i = 0; i < vertices->size; i++)
        {
            if(vertices->elementos[i].chave < dmin && !vertices->elementos[i].visitado)
            {
                dmin = vertices->elementos[i].chave;
                menorVert = i;
            }
        }
        vertices->elementos[menorVert].visitado = true;

        for(int i = 0; i < vertices->elementos[menorVert].vizinhos.size; i++)
        {
            if(!vertices->elementos[vertices->elementos[menorVert].vizinhos.elementos[i].destino].visitado && vertices->elementos[menorVert].vizinhos.elementos[i].peso < vertices->elementos[vertices->elementos[menorVert].vizinhos.elementos[i].destino].chave)
            {
                vertices->elementos[vertices->elementos[menorVert].vizinhos.elementos[i].destino].chave = vertices->elementos[menorVert].vizinhos.elementos[i].peso;
                vertices->elementos[vertices->elementos[menorVert].vizinhos.elementos[i].destino].antecessor = menorVert;
            }
        }
        numVert--;
    }
}

void swap(vertice *primeiro, vertice *segundo) // Troca as informações de dois vertices
{
    vertice aux;
    aux = *primeiro;
    *primeiro = *segundo;
    *segundo = aux;
}

void Bubble_Sort(verticeLista *vertices)  // Ordenando vertices
{
    for(int i = 1; i < vertices->size; ++i)
        for(int j = vertices->size-1; j >= i; --j)
            if(vertices->elementos[j-1].chave > vertices->elementos[j].chave) 
                swap(&vertices->elementos[j-1], &vertices->elementos[j]);
}

int main()
{
    FILE *FileIn, *FileOut;
    verticeLista vertices;
    bool fim = false;
    int soma = 0;
    frase antecessores[151];

    FileIn = fopen("entrada6.txt", "r");
    FileOut = fopen("Lab6_Rafael_Camargo.txt", "w");

    for(int i = 0; i < 6;)  //  Pula 6 linhas
    {
        char c;
        fscanf(FileIn, "%c", &c);
        if(c == '\n')
            i++;
    }

    // Cabecalho
    fprintf(FileOut, "No meio do 2o semestre tinha um lab\nTinha um lab no meio do 2o semestre\n");
    fprintf(FileOut, "Nunca me esquecerei desse acontecimento\nNa vida de minhas retinas tão fatigadas\n"); 
    fprintf(FileOut, "---------------------------------------\nOrdem de selecao pelo algoritmo\n---------------------------------------\n"); 

    while(!fim) // Leitura da entrada
    {
        frase nome1, nome2;
        aresta ligacao1, ligacao2;
        int peso, indice1, indice2;

        fim = (fscanf(FileIn, " %s %s %d ", nome1, nome2, &peso) == -1) ? true : false;
        if(!fim)
        {
            indice1 = EncontraIndice(&vertices, nome1);
            if(indice1 == -1)
            {
                vertice aux;
                strcpy(aux.nome, nome1);
                aux.visitado = false;
                aux.antecessor = -1;
                aux.chave = inf;
                InsertVertice(&vertices, aux);
                indice1 = vertices.size-1;
            }

            indice2 = EncontraIndice(&vertices, nome2);
            if(indice2 == -1)
            {
                vertice aux;
                strcpy(aux.nome, nome2);
                aux.visitado = false;
                aux.antecessor = -1;
                aux.chave = inf;
                InsertVertice(&vertices, aux);
                indice2 = vertices.size-1;
            }

            ligacao1.destino = indice2;
            ligacao1.peso = peso;
            ligacao2.destino = indice1;
            ligacao2.peso = peso;
            InsertAresta(&vertices.elementos[indice1].vizinhos, ligacao1);
            InsertAresta(&vertices.elementos[indice2].vizinhos, ligacao2);
        }
    }

    PrimAlg(&vertices);

    for(int i = 1; i < vertices.size; i++) //Imprimindo saida
    {
        fprintf(FileOut, "%-10s %-10s %-2d\n", vertices.elementos[i].nome, vertices.elementos[vertices.elementos[i].antecessor].nome, vertices.elementos[i].chave);
        soma += vertices.elementos[i].chave;
    }
    fprintf(FileOut, "\nCusto total: %d\n\n---------------------------------------\nOrdem crescente de custos\n---------------------------------------\n", soma);

    for(int i = 0; i < vertices.size; i++) //Guardando o nome dos elementos para acessar como antecessores em seguida
        strcpy(antecessores[i], vertices.elementos[i].nome);

    Bubble_Sort(&vertices);

    for(int i = 1; i < vertices.size; i++) //Imprimindo saida ordenada
        fprintf(FileOut, "%-10s %-10s %-2d\n", vertices.elementos[i].nome, antecessores[vertices.elementos[i].antecessor], vertices.elementos[i].chave);

    // Fechamento de arquivos
    fclose(FileIn);
    fclose(FileOut);

    return 0;
}
