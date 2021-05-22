#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#define row 4
#define col 6

typedef struct data {
	int angka;
	unsigned long long hasil;
}data;

int matrixB[row*col];
int counter;

unsigned long long faktorial(unsigned long long a){
    int pembilang = 1, penyebut = 1;
    unsigned long long ans = 0;

    if(a == 0 && matrixB[counter] == 0){
        counter++;
        return 0;
    }
    else if (a >= matrixB[counter]){
        for(int i = a; i > 0; i--){
            pembilang *= i;
        }
        for(int j = a-matrixB[counter]; j > 0; j--){
            if (j == 0){
                penyebut = 1;
                break;
            }
            penyebut *= j;
        }
        ans = pembilang/penyebut;
        counter++;
        return ans;
    }else if (matrixB[counter] > a){
        for(int i = a; i > 0; i--){
            pembilang *= i;
        }
        ans = pembilang;
        counter++;
        return ans;
    }
}

void *fact(void* arg)
{
	data* d = (data*) arg;
    d->hasil=faktorial(d->angka);
}
void main()
{
	key_t key = 1234;
	int *value;
	int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
	value = shmat(shmid, NULL, 0);

	pthread_t tid[row*col];
	data d[row*col];

    int i = 0, j = 0;

    //input Matrix B
    printf("Isi dari Matrix B: ");
    printf("\n");
    while(i < row*col){
        scanf("%d", &matrixB[i]);
        i++;
    }

    i = 0;
    j = 0;

    printf("\n");
    //Menampilkan Matrix dari 2a
    printf("Isi dari Matrix A: ");
    printf("\n");
    do{
        d[i].angka = value[i];
        if (i % 6 == 0) printf("\n");
        printf("%4d", value[i]);
        i++;
    }while(i < row*col);

    i = 0;
    while(i < row*col){
        d[i].angka = value[i];
        pthread_create(&tid[i],NULL, &fact, (void*) &d[i]);
        i++;
    }

    i = 0;
    printf("\n");
    printf("\n");
    printf("Isi dari Final Matrix: ");
    printf("\n");
    i = 0;

	for(i = 0; i< row*col; i++)
	{
		if (i % 6 == 0) printf("\n");
		printf("%llu\t", d[i].hasil);
        pthread_join(tid[i],NULL);
	}


	printf("\n");

	shmdt(value);
	shmctl(shmid, IPC_RMID, NULL);
} 