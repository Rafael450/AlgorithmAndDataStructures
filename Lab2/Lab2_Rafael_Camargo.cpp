/* Rafael Camargo                          */
/* Turma 3                                 */
/*                                         */
/* Exercício 2 : Indice de Livro           */

/* Programa compilado com gcc (Rev9, Built by MSYS2 project) 10.2.0     */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef char logic;
const logic True = 1, False = 0;

typedef char informacao[22];

typedef struct celula celula;
typedef celula *noh;
typedef celula *arvore;
struct celula
{
    informacao info;
    noh pai, fesq, idir;
};

typedef celula *arvore;

FILE *FileIn, *FileOut;

arvore NovaArvore() //Cria nova arvore
{
    arvore A;
    
    A = (noh) malloc(sizeof(celula));
    strcpy(A->info, " ");
    A->pai = NULL;
    A->idir = NULL;
    A->fesq = NULL;
    return A;
}

void InserirSub(noh pailocal, informacao ninfo) //Insere como filho esquerdo do noh
{
    noh p = (pailocal->fesq != NULL) ? pailocal->fesq : NULL;
    pailocal->fesq = (noh) malloc(sizeof(celula));
    strcpy(pailocal->fesq->info, ninfo);
    pailocal->fesq->pai = pailocal;
    pailocal->fesq->fesq = NULL;
    pailocal->fesq->idir = p;

}

void InserirSeguinte(noh iesq, informacao ninfo) //Insere seguinte ao noh
{
    noh p = (iesq->idir != NULL) ? iesq->idir : NULL;
    iesq->idir = (noh) malloc(sizeof(celula));
    strcpy(iesq->idir->info, ninfo);
    iesq->idir->idir = p;
    iesq->idir->pai = iesq->pai;
    iesq->idir->fesq = NULL;
}

void TransferirSub(noh origem, noh paidestino) //Transfere como filho do noh
{
    noh q;

    if(origem == origem->pai->fesq)
        origem->pai->fesq = origem->pai->fesq->idir;
    else
    {
        noh p;
        for(p = origem->pai->fesq; p->idir != origem; p = p->idir);
        p->idir = p->idir->idir;
    }
    origem->pai = paidestino;
    q = paidestino->fesq;
    paidestino->fesq = origem;
    origem->idir = q;
}

void TransferirSeguinte(noh origem, noh iesq) //Tranfere seguinte ao noh
{
    noh q;

    if(origem == origem->pai->fesq)
        origem->pai->fesq = origem->pai->fesq->idir;
    else
    {
        noh p;
        for(p = origem->pai->fesq; p->idir != origem; p = p->idir);
        p->idir = p->idir->idir;
    }
    origem->pai = iesq->pai;
    q = iesq->idir;
    iesq->idir = origem;
    origem->idir = q;
}

void Esvaziar(noh *local, logic topo) // Esvazia toda subarvore
{
    if(topo && (*local)->pai != NULL) 
    {
        if((*local) == (*local)->pai->fesq)
            (*local)->pai->fesq = (*local)->idir;
        else
        {
            noh p;
            for(p = (*local)->pai->fesq; p->idir != (*local); p = p->idir);
            p->idir = (*local)->idir;
        }
        (*local)->idir = NULL;
    }
    if((*local)->idir != NULL)
        Esvaziar(&(*local)->idir, False);
    if((*local)->fesq != NULL)
        Esvaziar(&(*local)->fesq, False);

    free(*local);
}

noh Procurar(noh local, informacao infolocal) //Procura na arvore
{
    if(strcmp(local->info, infolocal) == 0)
        return local;
    else if(local->idir == NULL && local->fesq == NULL)
        return NULL;
    else
    {
        noh q = NULL;
        if(local->fesq != NULL)
            q = Procurar(local->fesq, infolocal);
        if(local->idir != NULL && q == NULL)
            q = Procurar(local->idir, infolocal);
        return q;
    }
}

void Titulo(arvore *livro, logic *first) //Comando Titulo
{
    informacao titulo;
    fscanf(FileIn, "%s", titulo);

    if(*first)
        *first = False;
    else if(strcmp((*livro)->info, " ") != 0)
    {
        noh q = *livro;
        arvore B = NovaArvore();
        *livro = B;
        q->pai = *livro;
        (*livro)->fesq = q;
    }
    strcpy((*livro)->info, titulo);
}

void Inserir(arvore *livro) //Comando inserir
{
    char subcomando[12], nome1[22], nome2[22];
    fscanf(FileIn, " %s %s %s", nome1, subcomando, nome2);
    noh p = Procurar(*livro, nome2);

    if(p == NULL)
        fprintf(FileOut, "ERRO: nao encontrado item %s\n--------------------------------------------------\n", nome2);
    else if(strcmp(subcomando, "SUB") == 0)
        InserirSub(p, nome1); 
    else if(strcmp(subcomando, "SEGUINTE_A") == 0)
    {
        if(p != *livro)
            InserirSeguinte(p, nome1);
        else
            fprintf(FileOut, "Operação inválida.\n--------------------------------------------------\n");
    }
}

void Remover(arvore *livro) //Comando remover
{
    char nome[22];
    fscanf(FileIn, " %s", nome);
    noh p = Procurar(*livro, nome);

    if(p == NULL)
        fprintf(FileOut, "ERRO: nao encontrado item %s\n--------------------------------------------------\n", nome);
    else if(p == *livro)
    {
        Esvaziar(&(*livro), True);
        *livro = NovaArvore();
    }
    else
        Esvaziar(&p, True);
}

void Escrever(noh atual, int distancia, char numeros[]) //Recorrencia para percorrer a arvore e escrever a lista
{
    char numeros2[16];
    strcpy(numeros2, numeros);
    int size = (numeros2[0] != '*') ? strlen(numeros2) : 0;
    numeros2[size+1] = '\0';
    numeros2[size] = distancia + '0';

    for(int i = 0; i < strlen(numeros2); i++)
        fprintf(FileOut, "%d.", numeros2[i]-'0');
    fprintf(FileOut, "      %s\n", atual->info);
    if(atual->fesq != NULL)
        Escrever(atual->fesq, 1, numeros2);
    if(atual->idir != NULL)
        Escrever(atual->idir, distancia + 1, numeros);
}

void Lista(arvore *livro, int *n) // Comando lista
{
    (*n)++;
    char numeros[16] = "*";
    fprintf(FileOut, "LISTA %d\n\n      %s\n", *n, (*livro)->info);
    if((*livro)->fesq != NULL)
        Escrever((*livro)->fesq, 1, numeros);
    fprintf(FileOut, "--------------------------------------------------\n");
}

void Transferir(arvore *livro) // Comando Transferir
{
    char subcomando[12], nome1[22], nome2[22];
    fscanf(FileIn, " %s %s %s", nome1, subcomando, nome2);
    noh p = Procurar(*livro, nome2), q = Procurar(*livro, nome1);
    if(p == NULL)
        fprintf(FileOut, "ERRO: nao encontrado item %s\n", nome2);
    else if(strcmp(subcomando, "SUB") == 0)
        TransferirSub(q, p);
    else if(strcmp(subcomando, "SEGUINTE_A") == 0)
    {
        if(q != *livro)
            TransferirSeguinte(q, p);
        else
            fprintf(FileOut, "Operação inválida.\n--------------------------------------------------\n");
    }
}

void Fim(logic *fim) // Comando fim
{
    *fim = True;
}


int main()
{
    int numerolista = 0;
    logic fim = False, first = True;
    arvore livro;


    FileIn = fopen("entrada2.txt", "r");
    FileOut = fopen("Lab2_Rafael_Camargo.txt", "w");
    livro = NovaArvore();

    const char comandos[6][12] =  {
                                    "TITULO",
                                    "INSERIR",
                                    "REMOVER",
                                    "LISTA",
                                    "TRANSFERIR",
                                    "FIM"
                                 };
                       

    //Escrevendo cabecalho da saida
    fprintf(FileOut, "qualquer coisa\nqualquer coisa\nqualquer coisa\n--------------------------------------------------\n"); 

    //Pulando o cabecalho da entrada
    for(int i = 0; i < 4;)
    {
        char c;
        fscanf(FileIn, "%c", &c);
        if(c == '\n')
            i++;
    }
    //Lendo comando
    while(!fim)
    {
        char comando[12];
        int numero_comando;

        fscanf(FileIn, " %s", comando);

        for(int i = 0; i < 6; i++)
        {
            if(strcmp(comando, comandos[i]) == 0)
            {
                numero_comando = i;
                i = 6;
            }
        }

        //Aplicando comandos
        switch(numero_comando)
        {
            case 0:
                Titulo(&livro, &first);
                break;
            case 1:
                Inserir(&livro);
                break;
            case 2:
                Remover(&livro);
                break;
            case 3:
                Lista(&livro, &numerolista);
                break;
            case 4:
                Transferir(&livro);
                break;
            case 5:
                Fim(&fim);
                break;
        }
    }

    // Esvaziando arvore
    Esvaziar(&livro, True); 

    //Fechamento de arquivos
    fclose(FileIn);
    fclose(FileOut);

    return 0;
}
