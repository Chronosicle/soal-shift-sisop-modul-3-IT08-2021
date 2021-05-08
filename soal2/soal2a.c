#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#define row 4
#define col 6

int main()
{
	key_t key = 1234;
	int *value;
	int shmid = shmget(key, sizeof(int)*row*col, IPC_CREAT | 0666);
	value = shmat(shmid, NULL, 0);
	
	int m, n, p, q, c, d, k, sum = 0;
	printf("");
	int first[4][3];
	int second[3][6];
	printf("");
	int multiply[row][col];

	int i = 0, j = 0;

	do{
		while(j < 3){
			scanf("%d", &first[i][j]);
			j++;
		}
		j = 0;
		i++;
	}while(i < 4);

	i = 0;
	j = 0;

	while(i < 3){
		do{
			scanf("%d", &second[i][j]);
			j++;
		}while(j < 6);
		j = 0;
		i++;
	}

	for (c = 0; c < row; c++) {
		for (d = 0; d < col; d++) {
			for (k = 0; k < 3; k++) {
				sum = sum + first[c][k] * second[k][d];
			}
			multiply[c][d] = sum;
			sum = 0;
		}
	}
 
	for (c = 0; c < row; c++) {
		for (d = 0; d < col; d++) {
			value[c*col+d] = multiply[c][d];
			printf("%4d",multiply[c][d]);
		}
		printf("\n");
	}
	sleep(30);
	shmdt(value);
	return 0;
}