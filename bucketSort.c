#define TAMANHO 10 // tamanho do bucket

struct bucket {
        int qtd;
        int valores[TAMANHO];
};

void bucketSort(int *Vetor, int n){
        int i, j, maior, menor, numBaldes, posicao;
        struct bucket *balde;

        maior = menor = Vetor[0];

        for(i = 1; i < n; i++){
                if(Vetor[i] > maior){
                        maior = Vetor[i];
                }
                if(Vetor[i] < menor){
                        menor = Vetor[i]
                }
        }

        // Inicializando bucket's //
        numBaldes = (maior - menor) / TAMANHO + 1;
        balde = (struct bucket *) malloc(numBaldes * sizeof(struct bucket)); // Aloca espaço em memória para os bucket's //

        for(i = 0; i < numBaldes; i++){
                balde[i].qtd = 0;
        }

        for(i = 0; i < n; i ++){
                posicao = (Vetor[i] - menor) / TAMANHO;
                balde[posicao].valores[balde[posicao].qtd] = Vetor[i]; // Insere no final da lista ligada //
        }
};