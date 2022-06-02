/* Rafael Camargo                          */
/* Turma 3                                 */
/*                                         */
/* Exercício 4 : Ordenacao                 */

/* Programa compilado com gcc (Rev9, Built by MSYS2 project) 10.2.0     */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

typedef char string[55];
typedef string *vector_t;

int contador;

int compara(const char * a, const char * b) // Compara duas strings e soma no contador
{
    contador++;
    return strcmp(a, b);
}

void swap(string *primeiro, string *segundo) // Troca as informações de dois strings
{
    string aux;
    strcpy(aux, *primeiro);
    strcpy(*primeiro, *segundo);
    strcpy(*segundo, aux);
}

//------------------ Funcoes de Bubble-Sort ----------------------

void bubble_sort(vector_t v, int n)
{
    for(int i = 1; i < n; ++i)
        for(int j = n-1; j >= i; --j)
            if(compara(v[j-1], v[j]) > 0)
                swap(&v[j-1], &v[j]);
}

//------------------ Funcoes de Merge-Sort ----------------------

void merge(vector_t v, vector_t aux, int i, int f)
{
    int m = (i+f)/2;
    int iaux = i, ip1 = i;
    int ip2= m+1;

    while(ip1 <= m && ip2 <= f)
        if(compara(v[ip1], v[ip2]) < 0)
            strcpy(aux[iaux++], v[ip1++]);
        else
            strcpy(aux[iaux++], v[ip2++]);
    while(ip1 <= m)
        strcpy(aux[iaux++], v[ip1++]);
    while(ip2 <= f)
        strcpy(aux[iaux++], v[ip2++]);

    for(int j = i; j <= f; j++)
        strcpy(v[j], aux[j]);
}

void merge_sort(vector_t v, vector_t aux, int i, int f)
{
    if(i < f) 
    {
        int m = (i+f)/2;
        merge_sort(v, aux, i, m);
        merge_sort(v, aux, m+1, f);
        merge(v, aux, i, f);
    }
}

//------------------ Funcoes de Quick-Sort ----------------------

int partition(vector_t v, int left, int right, string pivot)
{
    while(1) 
    {
        while(compara(v[left], pivot) < 0)
            left++;

        while(compara(v[right], pivot) > 0)
            right--;

        if(right <= left)
            break;

        swap(&v[left++], &v[right--]);
    }

    return right;
}

void quick_sort(vector_t v, int min, int max)
{
    if(min >= max)
        return;

    string pivot;
    strcpy(pivot,v[(min+max)/2]);
    
    int p = partition(v, min, max, pivot);
    quick_sort(v, min, p);
    quick_sort(v, p+1, max);
}

int main()
{
    FILE *FileIn, *FileOut;
    int input_size;
    float inicio, fim, delta_time;
    

//-----------------------BubbleSort------------------------

    FileIn = fopen("entrada4.txt", "r");
    FileOut = fopen("Lab4_Rafael_Camargo_bubble.txt", "w");

    fscanf(FileIn, " %d ", &input_size);
    vector_t entrada;
    entrada = (string*) malloc((input_size)*sizeof(string));

    for(int i = 0; i < input_size; i++) // Le tamanho da entrada
        fgets(entrada[i], 55, FileIn);

    contador = 0; // Zera contador
    inicio = clock();
    bubble_sort(entrada, input_size);
    fim = clock();
    delta_time = (fim-inicio)/(float) CLOCKS_PER_SEC; // Determina tempo gasto na funcao de bubble-sort

    fprintf(FileOut, "Algoritmo: Bubble-Sort\n\nTamanho da entrada: %d\nComparações feitas: %d\nTempo de execução : %.3f segundos\n\n-------------------------------------\n", input_size, contador, delta_time);

    for(int i = 0; i < input_size; i++) // Imprime saida
        fprintf(FileOut, "%s", entrada[i]); 

    free(entrada);
    fclose(FileOut);
    fclose(FileIn);

//-----------------------MergeSort------------------------

    FileIn = fopen("entrada4.txt", "r");
    FileOut = fopen("Lab4_Rafael_Camargo_merge.txt", "w");

    fscanf(FileIn, " %d ", &input_size);
    entrada = (string*) malloc((input_size)*sizeof(string));

    for(int i = 0; i < input_size; i++) // Le tamanho da entrada
        fgets(entrada[i], 55, FileIn);

    contador = 0;
    vector_t auxiliar;
    auxiliar = (string*) malloc((input_size)*sizeof(string));
    inicio = clock();
    merge_sort(entrada, auxiliar, 0, input_size-1);
    fim = clock();
    delta_time = (fim-inicio)/(float) CLOCKS_PER_SEC; // Determina tempo gasto na funcao de merge-sort

    fprintf(FileOut, "Algoritmo: Merge-Sort\n\nTamanho da entrada: %d\nComparações feitas: %d\nTempo de execução : %.3f segundos\n\n-------------------------------------\n", input_size, contador, delta_time);

    for(int i = 0; i < input_size; i++) // Imprime saida
        fprintf(FileOut, "%s", entrada[i]);

    free(entrada);
    free(auxiliar);
    fclose(FileOut);
    fclose(FileIn);

//-----------------------QuickSort------------------------

    FileIn = fopen("entrada4.txt", "r");
    FileOut = fopen("Lab4_Rafael_Camargo_quick.txt", "w");

    fscanf(FileIn, " %d ", &input_size);
    entrada = (string*) malloc((input_size)*sizeof(string));

    for(int i = 0; i < input_size; i++) // Le tamanho da entrada
        fgets(entrada[i], 55, FileIn);

    contador = 0;
    inicio = clock();
    quick_sort(entrada, 0, input_size-1);
    fim = clock();
    delta_time = (fim-inicio)/(float) CLOCKS_PER_SEC; // Determina tempo gasto na funcao de merge-sort

    fprintf(FileOut, "Algoritmo: Quick-Sort\n\nTamanho da entrada: %d\nComparações feitas: %d\nTempo de execução : %.3f segundos\n\n-------------------------------------\n", input_size, contador, delta_time);

    for(int i = 0; i < input_size; i++) // Imprime saida
        fprintf(FileOut, "%s", entrada[i]);

    free(entrada);
    fclose(FileOut);
    fclose(FileIn);

    return 0;
}
