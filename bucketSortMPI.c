/*
* Arquivo: bucketSortMPI.c
* Autores: Julio Cesar Franke Fagundes & Vitor Garcez Morigi
* Curso: INE5645 - Programacao Paralela e Distida
* Algoritmo : Bucket Sort MPI
*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <mpi.h>

#define RAND_MAX 99
#define NARRAY 100  /* Tamanho do vetor */
#define NBUCKET 10 /* Numero de buckets */
#define INTERVAL 10 /* Delimitador de um bucket e outro */

struct Node
{
    int data;
    struct Node *next;
};

void BucketSort(int arr[]);
struct Node *InsertionSort(struct Node *list);
void print(int arr[]);
void printBuckets(struct Node *list);
int getBucketIndex(int value);
int array[NARRAY];

void BucketSort(int arr[])
{
    int i,j;
    struct Node **buckets;

    /* Aloca memória para os buckets */
    buckets = (struct Node **)malloc(sizeof(struct Node*) * NBUCKET);

    /* Inicializa ponteiros para os buckets */
    for(i = 0; i < NBUCKET; ++i)
    {
        buckets[i] = NULL;
    }

    /* Povoa os buckets */
    /* Cria uma lista encadeada em cada slot de bucket */
    for(i = 0; i < NARRAY; ++i)
    {
        struct Node *current;
        int pos = getBucketIndex(arr[i]);
        current = (struct Node *) malloc(sizeof(struct Node));
        current->data = arr[i];
        current->next = buckets[pos];
        buckets[pos] = current;
    }

    /* Imprime na tela o que tem em cada bucket */
    for(i = 0; i < NBUCKET; i++)
    {
        printf("Bucket[\"%d\"] : ", i);
        printBuckets(buckets[i]);
        printf("\n");
    }

    /* Ordena cada bucket usando Insertion Sort */
    for(i = 0; i < NBUCKET; ++i)
    {
        buckets[i] = InsertionSort(buckets[i]);
    }

    /* Imprime os buckets na tela após estarem ordenados */
    printf("--------------\n");
    printf("Buckets depois de ordenados\n");
    for(i = 0; i < NBUCKET; i++)
    {
        printf("Bucket[\"%d\"] : ", i);
        printBuckets(buckets[i]);
        printf("\n");
    }

    /* Coloca itens de volta a posição original */
    for(j =0, i = 0; i < NBUCKET; ++i)
    {
        struct Node *node;
        node = buckets[i];
        while(node)
        {

            // Pré-condição para evitar estouro de tamanho do vetor
            assert(j < NARRAY);
            arr[j++] = node->data;
            node = node->next;
        }
    }

    /* Libera memória */
    for(i = 0; i < NBUCKET; ++i)
    {
        struct Node *node;
        node = buckets[i];
        while(node)
        {
            struct Node *tmp;
            tmp = node;
            node = node->next;
            free(tmp);
        }
    }
    free(buckets);
    return;
}

/* Insertion Sort */
struct Node *InsertionSort(struct Node *list)
{
    struct Node *k,*nodeList;
    /* É necessário pelo menos dois itens para ordenar */
    if(list == NULL || list->next == NULL)
    {
        return list;
    }

    nodeList = list;
    k = list->next;
    nodeList->next = NULL; /* 1º nó é uma nova lista */
    while(k != NULL)
    {
        struct Node *ptr;
        /* Verifica se insire antes da primeira */
        if(nodeList->data > k->data)
        {
            struct Node *tmp;
            tmp = k;
            k = k->next;
            tmp->next = nodeList;
            nodeList = tmp;
            continue;
        }

        // do começo ao fim
        // acha [i] > [i+1]
        for(ptr = nodeList; ptr->next != NULL; ptr = ptr->next)
        {
            if(ptr->next->data > k->data) break;
        }

        // se encontrado (acima)
        if(ptr->next != NULL)
        {
            struct Node *tmp;
            tmp = k;
            k = k->next;
            tmp->next = ptr->next;
            ptr->next = tmp;
            continue;
        }
        else
        {
            ptr->next = k;
            k = k->next;
            ptr->next->next = NULL;
            continue;
        }
    }
    return nodeList;
}

int getBucketIndex(int value)
{
    return value/INTERVAL;
}

void print(int ar[])
{
    int i;
    for(i = 0; i < NARRAY; ++i)
    {
        printf("%d ", ar[i]);
    }
    printf("\n");
}

void printBuckets(struct Node *list)
{
    struct Node *cur = list;
    while(cur)
    {
        printf("%d ", cur->data);
        cur = cur->next;
    }
}

void gerarVetor(){
	int i;
	srand(time(0));
	for(i = 0; i < NARRAY; i++){			// Loop para preencher o vetor com elementos de 0 a 99
		array[i] = rand() % RAND_MAX;
	}

}

int main(int argc, char ** argv)
{
    int rank, nprocs;
    MPI_Status status;

    gerarVetor();

    printf("Vetor inicial\n");
    print(array);
    printf("------------\n");

    printf("Iniciando MPI...\n");

	if(MPI_Init(&argc, &argv) != MPI_SUCCESS){
		fprintf(stderr, "Nao foi possivel iniciar o MPI\n");
		return -1;
	}

    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	printf ("Inicializando troca de mensagens...\n\n");

	printf("Numero de processos: %d\n", nprocs);
	printf("Processo atual: %d\n", rank);

	//printf("Numero de processos: %d", nprocs);


    if(rank == 0){ // Processo MESTRE
        int i;
        for(i = 0; i < NBUCKET; i++) {
            MPI_Send(&array, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
	} else { // Processo ESCRAVO
		int i;
		for(i = 0; i < NBUCKET; i++) {
            printf("Processo %d executando\n", rank);
            BucketSort(array);
			MPI_Recv(&array, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
		}
	}

    printf("Finalizando MPI...");
    MPI_Finalize();

    printf("------------\n");
    printf("Vetor ordenado\n");
    print(array);

    return 0;
}
