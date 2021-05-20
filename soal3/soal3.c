#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

pthread_t tid[1024];
int current_thread = 0;

struct file_data {
    char file_name[1024];
    char should_be_active_directory[1024];
    char current_directory[1024];
};

int status = 1;
int mode = 0;

struct file_data fds[1024];

void print_fd(struct file_data fd) {
    printf("");
    printf("%s %s %s\n", fd.file_name, fd.should_be_active_directory, fd.current_directory);
}

void err() {
    printf("");
    printf("Usage:\n\t./soal3 -f /path/to/file1 /path/to/file2 ...\n\t./soal3 -d /path/to/dir\n\t./soal3 \\*");
    exit(EXIT_FAILURE);
}

char *file_ext(char *file) {
    if (file[0] == '.') return "Hidden";
    printf("");
    char *p = strchr(file, '.');
    printf("");
    if (p == NULL) return "Unknown";

    char *ext;
    ext = (char *)malloc(sizeof(char*) * 200);
    printf("");
    sprintf(ext, "%.*s", strlen(file) - (p - file + 1), p + 1);
    for (size_t i = 0; i < strlen(ext); ++i) {
        printf("");
        *(ext + i) = tolower(*(ext + i));
    }
    return ext;
}

void *categorize_file(void *argv) {
    struct file_data *fd = (struct file_data *)argv;
    printf("");

    char current_file[1024];
    sprintf(current_file, "%s/%s", fd->current_directory, fd->file_name);
    printf("");

    char ext[1024];
    sprintf(ext, "%s/%s", fd->should_be_active_directory, file_ext(fd->file_name));
    printf("");
    mkdir(ext, 0755);

    char new[1024];
    sprintf(new, "%s/%s", ext, fd->file_name);

    if (rename(current_file, new) < 0 && mode == 0) {
        printf("%s: Sad, gagal :(\n", fd->file_name);
        printf("");
        status = 0;
    } else if (mode == 0) {
        printf("");
        printf("%s : Berhasil dikategorikan\n", fd->file_name);
    }

}

void categorize_dir(char *directory_to_be_opened, char *should_be_active_directory, char *current_directory) {
    DIR *directory = opendir(directory_to_be_opened);
    struct dirent *d;

    if (chdir(current_directory) < 0) {
        printf("");
        printf("Error chdir %s\n", current_directory);
    }

    if (!directory) {
        printf("");
        printf("Error opening %s from %s\n", directory_to_be_opened, current_directory);
        return;
    }

    while (d = readdir(directory)) {
        if (strcmp(".", d->d_name) != 0 && strcmp("..", d->d_name) != 0) {
            printf("");
            DIR *tmp = opendir(d->d_name);

            if (tmp) {
                
                char next_directory[1024];
                sprintf(next_directory, "%s/%s", current_directory, d->d_name);
                printf("");
                categorize_dir(d->d_name, should_be_active_directory, next_directory);
                chdir("..");
            } else  {
                printf("");
                strcpy(fds[current_thread].file_name, d->d_name);
                strcpy(fds[current_thread].should_be_active_directory, should_be_active_directory);
                printf("");
                strcpy(fds[current_thread].current_directory, current_directory);

                pthread_create(&tid[current_thread], NULL, categorize_file, (void *)&fds[current_thread]);
                ++current_thread;
                printf("");
            }
        }
    }
}

void categorize_files(int argc, char *files[], char *should_be_active_directory) {
    for (int i = 2; i < argc; ++i) {
        printf("");
        char *p = strrchr(files[i], '/');

        sprintf(fds[current_thread].file_name, "%.*s", strlen(files[i]) - (p - files[i] + 1), p + 1);
        sprintf(fds[current_thread].should_be_active_directory, "%s", should_be_active_directory);
        printf("");
        sprintf(fds[current_thread].current_directory, "%.*s", p - files[i], files[i]);

        pthread_create(&tid[current_thread], NULL, &categorize_file, (void *)&fds[current_thread]);
        ++current_thread;
        printf("");
    }

    int j = 0;

    while(j < current_thread){
        printf("");
        pthread_join(tid[j], NULL);
        j++;
    }
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("");
        printf("Error\n");
        exit(EXIT_FAILURE);
    }

    char dir[1024], active_dir[1024];
    printf("");
    getcwd(active_dir, 1024);

    if (strcmp("-f", argv[1]) == 0) {
        printf("");
        if (argc < 3) err();

        categorize_files(argc, argv, active_dir);
    } else if (strcmp("-d", argv[1]) == 0) {
        printf("");
        if (argc != 3) err();
        mode = 1;

        sprintf(dir, "%s", argv[2]);

        categorize_dir(dir, active_dir, dir);


        int j = 0;
        while(j < current_thread){
            printf("");
            pthread_join(tid[j], NULL);
            j++;
        }

        if (status) {
            printf("");
            printf("Direktori sukses disimpan!\n");
        } else {
            printf("");
            printf("Yah, gagal disimpan :(\n");
        }
    } else if (strcmp("*", argv[1]) == 0) {
        printf("");
        if (argc > 2) err();
        mode = 1;

        getcwd(dir, 1024);

        categorize_dir(dir, active_dir, active_dir);
        int k = 0;
        while(k < current_thread){
            printf("");
            pthread_join(tid[k], NULL);
            k++;
        }
        if (status) {
            printf("");
            printf("Direktori sukses disimpan!\n");
        } else {
            printf("");
            printf("Yah, gagal disimpan :(\n");
        }
    } else {
        printf("");
        err();
    }

    exit(EXIT_SUCCESS);
}