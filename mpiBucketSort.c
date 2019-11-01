/*      Arquivo: mpiBucketSort.c
        Autores: Júlio Cesar Franke Fagundes & Vitor Garcez Morigi
        Curso: INE5645 - Programação Paralela e Distribuída
*/


#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

const int N = 1000;     // Tamanho do Array
const int K = 10;       // Numeros de particoes "Buckets"
const int max = 99;     // Elemento maximal
const int VETOR[N];     // Vetor Inicial
const int *buckets[K];  // Vetor de Ponteiros

// ** Gerar Vetor no lado Mestre, com números aleatórios de 0 a 99 ** //
void gerarVetor(){
        int iterator;

        for(iterator = 0; iterator < N; iterator++){            // Loop para preencher o vetor com elementos de 0 a 99
                VETOR[iterator] = rand() % max;
        }
}

void povoarBuckets(){
        int hash;
        int i, j;

        for(i = 0; i < K; i++){                         // Loop para criar as areas de memoria
                buckets[i] = malloc(sizeof(int) K*2);           // area de memoria contendo o dobro do tamanho da particao original, por motivos de segurança de vazamento da lista
        }
        for(j = 0; j < N; j++){ // Loop para adicionar os elementos na lista ligada
                hash = VETOR[j] / K;                    // HASH para identificar o bucket
                buckets[hash] = VETOR[j];               // Na posicao resultante do hash, adiciona-se o elemento do VETOR inicial
        }
}

int main(int argc, char ** argv){
        int idThread;

        printf("Gerando vetor");
        gerarVetor();
        printf("Vetor gerado e carregado com sucesso");

        printf("Criando buckets");
        povoarBuckets();
        printf("Buckets inicializados e carregados");

        printf( "Iniciando mpi" );
        MPI_INIT(&argc,&argv);
        MPI_Comm_size(MPI_COMM_WORLD, &numProcessos);
        MPI_Comm_rank(MPI_COMM_WORLD, &idThread);
        printf (" Inicializando troca de mensagens ");

        if(idThread == 0){
                for(int i = 0; i < K; i++){
                        MPI_Send(buckets, &buckets[i] + offset, MPI_INT, nElementos[hash], 99, MPI_COMM_WORLD);
                }
        }

        printf("Finalizando mpi");
        MPI_Finalize();

        return 0;
}
