/* Rafael Camargo                           */
/* Turma 3                                  */
/*                                          */
/* Exercício 1: Torre de Controle           */

/* Programa compilado com gcc (Rev9, Built by MSYS2 project) 10.2.0     */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef char logic;
const logic True = 1, False = 0;

typedef struct noh noh;
struct noh 
{
    char from[50], numero_voo[4];
    noh *prox;
};

typedef noh* posicao;

typedef struct lista lista;
struct lista
{
    posicao inic, fim;
};

lista NovaLista() // Cria uma nova lista vazia
{
    lista L;
    L.inic = L.fim = (noh*) malloc(sizeof(noh));
    L.inic->prox = NULL;
    return L;
}

posicao Fim(lista L) // Retorna um ponteiro que aponta para o fim da lista
{
    return L.fim;
}

void Inserir(char voo[], char nome[], posicao p, lista *L) // Insere um elemento na posicao seguinte ao ponteiro de posicao p
{
    posicao q;
	q = p->prox;
	p->prox = (noh*) malloc(sizeof(noh));
	strcpy(p->prox->numero_voo, voo);
    strcpy(p->prox->from, nome);
	p->prox->prox = q;

    if(p == Fim(*L) && L->inic->prox != NULL)
    {
        L->fim = p->prox;
    }
}

void Deletando(posicao p, lista *L) // Deleta o elemento seguinte ao elemento apontado pelo ponteiro de posicao p
{
    posicao q;
    if(p->prox == Fim(*L))
    {
        for(posicao r = L->inic; r->prox != NULL; r = r->prox)
        {
            if(r->prox == Fim(*L))
                L->fim = r;
        }
    }

    if(p != NULL && p->prox != NULL)
    {
        q = p->prox;
        p->prox = q->prox;
        free(q);
    }
}

posicao Encontrar(char voo[], lista *L) // Procura na lista um voo com um numero desejado e retorna um ponteiro apontando a posicao anterio a este
{
    for(posicao p = L->inic; p->prox != NULL; p = p->prox)
    {
        if(strcmp(p->prox->numero_voo, voo) == 0)
            return p;
    }
    return NULL;
}

void Esvaziar(lista *L, FILE *FileOut) // Remove e imprime na saida todos os elementos de uma lista
{
    posicao p;

    while(L->inic != NULL)
    {
        p = L->inic;
        if(L->fim != L->inic)
            fprintf(FileOut, "%s    %s", p->prox->numero_voo, p->prox->from);
        L->inic = p->prox;
        free(p);
    }
}

void DeletarLista(lista *L) // Deleta uma lista ja esvaziada
{
    if(L->inic == NULL && L->fim == NULL)
    {
        free(L->fim);
        free(L->inic);
    }
}

logic Vazia(lista *L) // Verifica se uma lista esta vazia
{
    if(L->inic == L->fim)
        return True;
    else 
        return False;
}

int main()
{
    FILE *FileIn, *FileOut;
    FileIn = fopen("entrada1.txt", "r");
    FileOut = fopen("Lab1_Rafael_Camargo.txt", "w");
    lista urgencia, voos;
    logic fim = False;

    voos = NovaLista();
    urgencia = NovaLista();    
    
    const char comandos[4][16] = {
                                    "pede_pouso",
                                    "pista_liberada",
                                    "URGENCIA",
                                    "FIM"
                                 };

    //Escrevendo cabecalho da saida
    fprintf(FileOut, "ARQUIVO DE SAIDA\nLista com pousos\nAUTORIZACOES DE POUSO\n=========================================\nFLIGHT  FROM\n\n");

    //Pulando o cabecalho da entrada
    for(int i = 0; i < 8; i = i)
    {
        char c;
        fscanf(FileIn, "%c", &c);
        if(c == '\n')
            i++;
    }

    //Lendo comandos
    while(!fim)
    {
        char comando[15], nome[50], numero_voo[5];
        int numero_comando;
        fscanf(FileIn, " %s %s ", comando, numero_voo);
        fgets(nome, 50, FileIn);
        
        for(int i = 0; i < 4; i++)
        {
            if(strcmp(comando, comandos[i]) == 0)
            {
                numero_comando = i;
                i = 4;
            }
        }

        //Aplicando comandos
        switch(numero_comando)
        {
            case 0: //Comando pede_pouso
                Inserir(numero_voo, nome, voos.fim, &voos);
                break;
            case 1: //Comando pista_liberada
                if(urgencia.inic->prox != NULL)
                {
                    strcpy(numero_voo,urgencia.inic->prox->numero_voo);
                    strcpy(nome,urgencia.inic->prox->from);
                    fprintf(FileOut, "%s    %s", numero_voo, nome); 
                    if(!Vazia(&urgencia))
                        Deletando(urgencia.inic, &urgencia);                  
                }
                else if(voos.inic->prox == NULL)
                    fprintf(FileOut, "0000    Nenhum aviao pousando\n");
                else
                {   
                    strcpy(numero_voo,voos.inic->prox->numero_voo);
                    strcpy(nome,voos.inic->prox->from);
                    fprintf(FileOut, "%s    %s", numero_voo, nome);
                    if(!Vazia(&voos))
                        Deletando(voos.inic, &voos);
                }
                break;
            case 2: //Comando URGENCIA
            {
                posicao p = Encontrar(numero_voo, &voos);
                strcpy(nome,p->prox->from);
                Inserir(numero_voo, nome, urgencia.fim, &urgencia);
                Deletando(p, &voos);
                break;
            }
            case 3: //Comando FIM
                fim = True;
                break;
        }
    }

    //Impressão e esvaziamento do restante das listas de voo e urgencia
    fprintf(FileOut, "\nLISTA URGENCIA:\n\n");

    if(Vazia(&urgencia))
    {
        fprintf(FileOut, "Vazia\n");
        Esvaziar(&urgencia, FileOut);
    }
    else
        Esvaziar(&urgencia, FileOut);
    DeletarLista(&urgencia);

    fprintf(FileOut, "\nLISTA PRINCIPAL:\n\n");

    if(Vazia(&voos))
    {
        fprintf(FileOut, "Vazia\n");
        Esvaziar(&urgencia, FileOut);
    }
    else
        Esvaziar(&voos, FileOut);
    DeletarLista(&voos);

    //Fechamento dos arquivos de texto
    fclose(FileIn);
    fclose(FileOut);

    return 0;
}