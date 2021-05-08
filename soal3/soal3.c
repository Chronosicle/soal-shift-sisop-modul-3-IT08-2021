#include <stdio.h>
#include <string.h>
#include <unistd.h> 
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <pthread.h>
#include <ctype.h>


#define MAX 256
char filelama[MAX][MAX];
char cwd[MAX];

char directorylama[100];
unsigned long count;

int counter_f;
char full[1024];


const char *get_filename_ext(const char *filename) {
	const char *dot = strrchr(filename, '.');
	char source[1024];
	sprintf(source, "%s", filename);
	if(!dot || dot == filename){
		printf("");
        return "";
    }else if(source[0] == '.'){
        printf("");
		return "Hidden";
	}else{
        printf("");
		return dot+1;
	}
}

void lower(char* s) {
	int i = 0;
	while(i < strlen(s)){
		s[i] = tolower(s[i]);
		i++;
	}
}

void* fungsif(void *filelama){
	char namafile[100];
	char *x = (char*)filelama;
	printf("");
	char *tmp = strrchr(x, '/');
	if(!tmp) strcpy(namafile,x);
	else strcpy(namafile,tmp);
	const char *ekstensifile = get_filename_ext(namafile);
	printf("");
	char filebaru[100];
	char ekstensifile2[100];	
	strcpy(ekstensifile2,ekstensifile);
	if (strcmp(ekstensifile,"")==0) 
		strcpy(ekstensifile2,"UNKNOWN");
	else if(strcmp(ekstensifile, "Hidden") == 0)
		strcpy(ekstensifile2, "Hidden");
	else if(strcmp(ekstensifile, "bingung") == 0)
		strcpy(ekstensifile2, "bingung");
	lower(ekstensifile2);
	printf("");
	strcpy(filebaru,directorylama);
	strcat(filebaru,"/");
	strcat(filebaru,ekstensifile2);
	strcat(filebaru,"/");
	strcat(filebaru,namafile);
	printf("");
	mkdir(ekstensifile2,0755);
	rename(x,filebaru);
}

void* fungsi_bintang(void *filelama){
	char namafile[100];
	char *x = (char*)filelama;
	char *tmp = strrchr(x, '/');
	printf("");
	if (!tmp)
		strcpy(namafile,x);
	else strcpy(namafile,tmp);
	printf("");
	const char *ekstensifile = get_filename_ext(namafile);
	char filebaru[100];
	char ekstensifile2[100];	
	strcpy(ekstensifile2,ekstensifile);
	printf("");
	if (strcmp(ekstensifile,"")==0) 
		strcpy(ekstensifile2,"UNKNOWN");
	else if(strcmp(ekstensifile, "Hidden") == 0)
		strcpy(ekstensifile2, "Hidden");
	else if(strcmp(ekstensifile, "bingung") == 0)
		strcpy(ekstensifile2, "bingung");
	lower(ekstensifile2);
	printf("");
	strcpy(filebaru,directorylama);
	strcat(filebaru,"/");
	strcat(filebaru,ekstensifile2);
	printf("");
	strcat(filebaru,"/");
	strcat(filebaru,namafile);
	mkdir(ekstensifile2,0755);
	printf("");
	rename(x,filebaru);
}

void* fungsid(void *filelama){
	char namafile[100];
	char *x = (char*)filelama;
	char *tmp = strrchr(x, '/');
	printf("");
	if(!tmp) strcpy(namafile,x);
	else strcpy(namafile,tmp);
	printf("");
	const char *ekstensifile = get_filename_ext(namafile);
	char filebaru[100];
	char ekstensifile2[100];	
	strcpy(ekstensifile2,ekstensifile);
	printf("");
	if (strcmp(ekstensifile,"")==0) 
		strcpy(ekstensifile2,"UNKNOWN");
	else if(strcmp(ekstensifile, "Hidden") == 0)
		strcpy(ekstensifile2, "Hidden");
	else if(strcmp(ekstensifile, "bingung") == 0)
		strcpy(ekstensifile2, "bingung");
	lower(ekstensifile2);
	printf("");
	strcpy(filebaru,directorylama);
	strcat(filebaru,"/");
	strcat(filebaru,ekstensifile2);
	printf("");
	strcat(filebaru,namafile);
	mkdir(ekstensifile2,0755);
	printf("");
	rename(x,filebaru);
}

void listdir(const char *name, int indent)
{
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(name)))
        return;

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            char path[1024];
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
			printf("");
            lower(entry->d_name);
            strcat(full, entry->d_name);
            strcat(full, "/");
            printf("");
            snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
            listdir(path, indent + 2);
        } else {
			printf("");
            strcat(full, entry->d_name);
			strcat(filelama[count], full);
            // printf("%s\n", full);
            count++;
        }
		memset(full, 0, sizeof(full));
    }
    closedir(dir);
}

int main(int argc, char **argv){
	getcwd(directorylama, sizeof(directorylama));
    // printf("Direktori sekarng: %s\n", directorylama);

	if (strcmp(argv[1],"-f")==0){
        printf("Jumlah file: %d\n", argc-2);
		if (argc<3){
			printf("Jumlah argumen kurang!");
            printf("\n");
			return 0;
		}
		pthread_t tid[argc-2];
		for(int i = 1; i <= argc-2; i++){
			FILE *nama_file;
			if(nama_file = fopen(argv[i+1], "r")){
				fclose(nama_file);
				printf("File %d: Berhasil Dikategorikan\n", i);
			}else{
				printf("File %d : Sad, gagal :(\n", i);
			}
		}
        int x = 2;
		do{
			pthread_create(&(tid[x-2]),NULL,fungsif,argv[x]);
			x++;
		}while(x < argc);

		x = 0;
		printf("");

		while(x < argc-2){
			pthread_join(tid[x],NULL);
			x++;
		}	
        printf("");
	}

	else if (strcmp(argv[1],"*")==0){
		if (argc!=2){
			printf("Jumlah argumen kurang!");
            printf("\n");
			return 0;
		}
		listdir(".", 0);
		pthread_t tid[count];
		int x = 0;
		do{
			pthread_create(&(tid[x]),NULL,fungsi_bintang,filelama[x]);
			x++;
		}while(x < count);
		x = 0;
		printf("");
		while(x < count){
			pthread_join(tid[x],NULL);
			x++;
		}	
		printf("");
		memset(filelama, 0, sizeof(filelama));

		return 0;
	}

	else if (strcmp(argv[1], "-d") == 0){
		if (argc<3){
			printf("");
			printf("Jumlah argumen kurang!");
            printf("\n");
			return 0;
		}
		struct dirent *de;
			DIR *dir = opendir(argv[2]);
			if(!dir){
				printf("opendir() failed! Does it exist?\n");
				return 1;
			}

			unsigned long count=0;

			while(de = readdir(dir)){
				if (de->d_type==4) continue;
				sprintf(filelama[count], "%s/%s", argv[2],de->d_name);
				count++;
			}
			closedir(dir);

			int x = 0;

			pthread_t tid[count];
			do{
				pthread_create(&(tid[x]),NULL,fungsid,filelama[x]);
				x++;
			}while(x < count);
				
			printf("");
			x = 0;
			while(x < count){
				pthread_join(tid[x],NULL);
				x++;
			}	
			printf("");

			return 0;
	}
	else
	{
		printf("Inputan salah!\n");
		printf("");
	}
}