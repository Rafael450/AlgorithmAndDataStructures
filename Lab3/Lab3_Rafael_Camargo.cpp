/* Rafael Camargo                          */
/* Turma 3                                 */
/*                                         */
/* Exercício 3 : Agenda Eletronica         */

/* Programa compilado com gcc (Rev9, Built by MSYS2 project) 10.2.0     */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef char frase[42];

typedef struct elemento elemento;
struct elemento
{
    frase tarefa;
    int prioridade;
};

typedef struct heap heap;
struct heap
{
    elemento *vector;
    int size, max_size;
};

//Funcoes "privadas"

void swap(elemento *primeiro, elemento *segundo) // Troca as informações de dois elementos
{
    elemento aux = *primeiro;
    *primeiro = *segundo;
    *segundo = aux;
}


void Arrumador(heap *queue, int posicao)    //  Funcao que arruma o caso de um noh perder a propriedade estrutural quando colocado um filho 
{
    int pai = (posicao/2 != 0) ? posicao/2 : 1;

    if(queue->vector[pai].prioridade < queue->vector[posicao].prioridade)
    {
        swap(&queue->vector[pai], &queue->vector[posicao]);
        Arrumador(queue, pai);
    }
}

void sift(heap *queue, int posicao) // Funçao que arruma o caso de um noh perder a propriedade estrutural quando substituido um noh superior
{
    int maior = posicao;
    
    if(2*posicao <= queue->size && queue->vector[2*posicao].prioridade > queue->vector[maior].prioridade)
        maior = 2*posicao;
    if(2*posicao+1 <= queue->size && queue->vector[2*posicao+1].prioridade > queue->vector[maior].prioridade)
        maior = 2*posicao+1;
    if(maior != posicao)
    {
        swap(&queue->vector[maior], &queue->vector[posicao]);
        sift(queue, maior);
    }
}

// Funcoes "publicas"

void Inicializar(heap *queue, int max)  //  Funcao que inicializa o heap com um valor determinado pro tamanho maximo do vetor
{
    queue->max_size = max;
    queue->size = 0;
    queue->vector = (elemento*) malloc((max+1)*sizeof(elemento));
}

void Insert(heap *queue, elemento inserido) // Funcao de inserir um noh
{
    (queue->size)++;
    queue->vector[queue->size] = inserido;
    Arrumador(queue, queue->size);
}

elemento Maximum(heap *queue)   // Funcao que retorna o noh de maior prioridade
{
    return queue->vector[1];
}

void RemoveMax(heap *queue)     //  Funcao que remove o noh de maior prioridade
{
    queue->vector[1] = queue->vector[queue->size];
    queue->vector[queue->size].prioridade = -1;
    queue->size--;
    sift(queue, 1);
}

void Finalizar(heap *queue)     // Funcao que liberar o espaco de memoria reservado pro vetor do heap
{
    free(queue->vector);
}

bool HeapVazio(heap *queue)     //  Funcao que determina se o heap esta vazio ou nao
{
    if(queue->size == 0)
        return true;
    else
        return false;
}

bool HeapCheio(heap *queue)     //  Funcao que determina se o heap esta cheio ou nao
{
    if(queue->size == queue->max_size)
        return true;
    else
        return false;
}

int main()
{
    int max_size;
    bool fim;
    FILE *FileIn, *FileOut;
    heap priority_queue;

    FileIn = fopen("entrada3.txt", "r");
    FileOut = fopen("Lab3_Rafael_Camargo.txt", "w");

    const char comandos[3][8] = {
                                    "NOVA",
                                    "PROXIMA",
                                    "FIM"
                                };

    for(int i = 0; i < 5;)  //  Pula 5 linhas
    {
        char c;
        fscanf(FileIn, "%c", &c);
        if(c == '\n')
            i++;
    }
    
    fscanf(FileIn, " %d ", &max_size);
    Inicializar(&priority_queue, max_size);

    for(int i = 0; i < 3;)  //  Pula 3 linhas
    {
        char c;
        fscanf(FileIn, "%c", &c);
        if(c == '\n')
            i++;
    }

    fprintf(FileOut, "PASSO 1: Retirar o miojo do pacote\nPASSO 2: Ferver 300 ml de agua"); 
    fprintf(FileOut, "\nPASSO 3: Colocar o miojo na panela com agua e deixar por 3 minutos\n");
    fprintf(FileOut, "PASSO 4: Retirar o miojo da panela\nPASSO 5: Delicie-se\n--------------------------------------------------\n");
    fprintf(FileOut, "RESPOSTAS DAS CONSULTAS\n--------------------------------------------------\n");

    while(!fim)
    {
        char comando[8];
        int numero_comando;
        elemento manipulador;

        fscanf(FileIn, " %s", comando); //Leitor de comandos

        for(int i = 0; i < 3; i++)
        {
            if(strcmp(comando, comandos[i]) == 0)
            {
                numero_comando = i;
                i = 3;
            }
        }

        //Aplicando comandos
        switch(numero_comando) 
        {
            case 0:     //Comando NOVA
                fscanf(FileIn, " %d ", &manipulador.prioridade);
                fgets(manipulador.tarefa, 43, FileIn);

                if(!HeapCheio(&priority_queue))
                {    
                    Insert(&priority_queue, manipulador);
                }
                else        // Caso em que o heap esta cheio
                    fprintf(FileOut, "ERRO     Agenda cheia. Pare de se sugar!\n");

                break;
            case 1: //Comando PROXIMA
                if(!HeapVazio(&priority_queue))     
                {    
                    manipulador = Maximum(&priority_queue);
                    RemoveMax(&priority_queue);
                    fprintf(FileOut, "%2d       %s", manipulador.prioridade, manipulador.tarefa);
                }
                else        // Caso em que o heap esta vazio
                    fprintf(FileOut, "AVISO    Nao ha tarefas na agenda, pode jogar videogame\n");

                break;
            case 2:     //Comando fim;
                fim = true;
                break;
        }
    }

    fprintf(FileOut, "\n--------------------------------------------------\nFICA PARA O DIA SEGUINTE\n--------------------------------------------------\n");

    if(HeapVazio(&priority_queue))      //  Caso em que o heap esta vazio no fim
        fprintf(FileOut, "Agenda vazia! Amanha o dia eh livre :D");

    while(!HeapVazio(&priority_queue))      //Esvazia e imprime o restante da lista no dia seguinte
    {
        elemento finalizador = Maximum(&priority_queue);
        RemoveMax(&priority_queue);
        fprintf(FileOut, "%2d       %s", finalizador.prioridade, finalizador.tarefa);
    }

    Finalizar(&priority_queue);     // Libera o malloc do vector da heap

    return 0;
}
