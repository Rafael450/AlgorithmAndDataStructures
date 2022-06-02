/* Rafael Camargo                          */
/* Turma 3                                 */
/*                                         */
/* Exercício 5 : PERT                      */

/* Programa compilado com gcc (Rev9, Built by MSYS2 project) 10.2.0     */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//Typedefs e structs
typedef char frase[32];

typedef struct tarefa tarefa;
struct tarefa
{
    frase descricao;
    int duracao;
    int tempoatual;
    char precendente[54];
    int cor;
    bool existe;
};

typedef struct pilha pilha;
struct pilha
{
    int top;
    char itens[54];
};

//Variaveis globais
bool Adjacencias[54][54];
tarefa tarefas[54];

int Indice(char c) // Determina o indice de um caractere
{
    if(c == '.')
        return 0;
    if(c <= 'Z')
        return c - 'A' + 1;
    else
        return 27 + c - 'a';
}

char AntiIndice(int c) // Determina o caractere de um indice
{
    if(c == 0)
        return '.';
    if(c <= 26)
        return 'A' + c - 1;
    else
        return 'a' + c - 27;
}

void LerPrecedentes(char B, FILE *FileIn) // Função de leitura do precedente 
{
    char precedentes[105];

    fgets(precedentes, 105, FileIn);

    for(int i = 0; i < strlen(precedentes); i += 2)
    {
        Adjacencias[Indice(precedentes[i])][Indice(B)] = true;
        tarefas[Indice(B)].precendente[i/2] = precedentes[i];
    }
}

pilha StackIniciar()  // Inicia a pilha
{
    pilha stack;
    stack.top = -1;
    return stack;
}

void StackPush(pilha *stack, char x) // Insere elemento na pilha
{
    ++stack->top;
    stack->itens[stack->top] = x;
}

char StackPop(pilha *stack)  // Retira elemento da pilha
{
    char x = stack->itens[stack->top];
    stack->top--;
    return x;
}

void ProcuraCicloEOrdenar(pilha *ciclo, pilha *ordenacao, char atual, bool *achou, bool *fimciclo)  // Funcao que ao mesmo tempo procura ciclo e determina a ordem topologica
{
    if(!(*achou))
    {
        bool ehfolha = true;
        tarefas[Indice(atual)].cor = 1;

        for(int i = 0; i < 54 && !(*achou); i++)
        {
            if(Adjacencias[Indice(atual)][i] == true)
            {
                ehfolha = false;

                if(tarefas[i].cor == 0)
                    ProcuraCicloEOrdenar(ciclo, ordenacao, AntiIndice(i), achou, fimciclo);
                if(tarefas[i].cor == 1)
                {
                    *achou = true;
                    StackPush(ciclo, AntiIndice(i));
                }
            }
        }

        if(ehfolha)
        {
            char aux[2];
            aux[0] = atual;
            Adjacencias[Indice(atual)][53] = true;
            strcat(tarefas[53].precendente, aux);
        }
    }

    if(*achou && !(*fimciclo))
    {
        StackPush(ciclo, atual);
        if(atual == ciclo->itens[0])
            *fimciclo = true;
    }

    tarefas[Indice(atual)].cor = 2;
    StackPush(ordenacao, atual);
}

void CaminhoCritico(pilha *ordenacao, char *critico) // Funcao que determina o caminho critico a partir da ordem topologica
{
    while(ordenacao->top != -1)
    {
        char a = StackPop(ordenacao);
        for(int i = 0; i < strlen(tarefas[Indice(a)].precendente); i++)
        {
            if(tarefas[Indice(tarefas[Indice(a)].precendente[i])].tempoatual + tarefas[Indice(a)].duracao > tarefas[Indice(a)].tempoatual)
            {
                tarefas[Indice(a)].tempoatual = tarefas[Indice(tarefas[Indice(a)].precendente[i])].tempoatual + tarefas[Indice(a)].duracao;
                critico[Indice(a)] = tarefas[Indice(a)].precendente[i];
            }
        }
    }
    for(int i = 0; i < strlen(tarefas[53].precendente); i++)
    {
        if(tarefas[Indice(tarefas[53].precendente[i])].tempoatual + tarefas[53].duracao > tarefas[53].tempoatual)
        {
            tarefas[53].tempoatual = tarefas[Indice(tarefas[53].precendente[i])].tempoatual + tarefas[53].duracao;
            critico[53] = tarefas[53].precendente[i];
        }
    }
}

int main()
{
    FILE *FileIn, *FileOut;
    bool fim = false, temciclo = false, fimciclo = false, teminicio = false;
    pilha ciclo = StackIniciar();
    pilha ordenacao = StackIniciar();
    pilha criticorev = StackIniciar();
    char critico[54];

    FileIn = fopen("entrada5.txt", "r");
    FileOut = fopen("Lab5_Rafael_Camargo.txt", "w");

    for(int i = 0; i < 7;)  //  Pula 7 linhas
    {
        char c;
        fscanf(FileIn, "%c", &c);
        if(c == '\n')
            i++;
    }

    // Cabecalho
    fprintf(FileOut, "PASSO 1: Retirar o miojo do pacote\nPASSO 2: Ferver 300 ml de agua"); 
    fprintf(FileOut, "\nPASSO 3: Colocar o miojo na panela com agua e deixar por 3 minutos\n");
    fprintf(FileOut, "PASSO 4: Retirar o miojo da panela\nPASSO 5: Delicie-se\n------------------------------------------------------------\n");
   
    while(!fim) // Leitura da entrada
    {
        char tarefa;

        fscanf(FileIn, " %c ", &tarefa);
        tarefas[Indice(tarefa)].existe = true;

        if(tarefa == '-')
            fim = true;
        else
        {
            fgets(tarefas[Indice(tarefa)].descricao, 31, FileIn);
            fscanf(FileIn, " %d ", &(tarefas[Indice(tarefa)].duracao));
            LerPrecedentes(tarefa, FileIn);
        }
    }

    // Procura ciclo e determina a ordem topologica
    for(int i = 0; i < 54; i++)
        if(Adjacencias[0][i] == true && !temciclo)
            ProcuraCicloEOrdenar(&ciclo, &ordenacao, AntiIndice(i), &temciclo, &fimciclo);

    // Verifica o caso de nao existir precedente ou exista um ciclo fora da ordenacao encontrada
    for(int i = 0; i < 54 && !temciclo; i++)
        if(tarefas[i].existe && tarefas[i].cor != 2)
                ProcuraCicloEOrdenar(&ciclo, &ordenacao, AntiIndice(i), &temciclo, &fimciclo);
            
    
    if(temciclo) //Saida caso tenha ciclo
    {
        fprintf(FileOut, "\nO sistema é ciclico e possui o seguinte ciclo:  \n\n");
        char letra;
        while(ciclo.top != -1)
        {
            letra = StackPop(&ciclo);
            fprintf(FileOut, "%c ", letra);
        }
        fprintf(FileOut, "\n\n------------------------------------------------------------\n");
    }
    else // Saida caso nao tenha ciclo
    {
        fprintf(FileOut, "\nUMA ORDENACAO TOPOLOGICA:\n\n");
        for(int i = ordenacao.top; i >= 0; i--)
            fprintf(FileOut, "%c ", ordenacao.itens[i]);
        fprintf(FileOut, "\n\n------------------------------------------------------------\n\nCAMINHO CRITICO:\n\nTAREFA        DESCRICAO           DURACAO\n\n");

        CaminhoCritico(&ordenacao, critico);
        char atual = critico[53];
        
        while(tarefas[Indice(atual)].precendente[0] != '.') // Coloca numa pilha os elementos do caminho critico
        {
            StackPush(&criticorev, atual);
            atual = critico[Indice(atual)];
        }
        StackPush(&criticorev, atual);
        while(criticorev.top != -1)  // Coloca na saida o caminho critico
        {
            char pop = StackPop(&criticorev);
            fprintf(FileOut, "%3c   %s%3d\n", pop, tarefas[Indice(pop)].descricao, tarefas[Indice(pop)].duracao);
        }
        fprintf(FileOut, "                                   ----\nTEMPO MINIMO PARA O PROJETO:        %3d semanas", tarefas[53].tempoatual);
    }

    // Fechamento de arquivos
    fclose(FileIn);
    fclose(FileOut);

    return 0;
}
