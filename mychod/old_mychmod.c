#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>

int main (int argc, char *argv[]){

    if (argc != 3){
        printf("Usage: ./mychmod mode file\n");
        return 1;
    }

    struct stat info;

    if (stat(argv[2], &info) == -1){
        perror("stat");
        return 1;
    }

    printf("%c", (info.st_mode & S_IRUSR) ? 'r' : '-');
    printf("%c", (info.st_mode & S_IWUSR) ? 'w' : '-');
    printf("%c", (info.st_mode & S_IXUSR) ? 'x' : '-');

    printf("%c", (info.st_mode & S_IRGRP) ? 'r' : '-');
    printf("%c", (info.st_mode & S_IWGRP) ? 'w' : '-');
    printf("%c", (info.st_mode & S_IXGRP) ? 'x' : '-');

    printf("%c", (info.st_mode & S_IROTH) ? 'r' : '-');
    printf("%c", (info.st_mode & S_IWOTH) ? 'w' : '-');
    printf("%c", (info.st_mode & S_IXOTH) ? 'x' : '-');

    int u = 0;
    int g = 0;
    int o = 0;
    int a = 0;
    
    int r = 0;
    int w = 0;
    int x = 0;

    char operation;
    mode_t permissions = 0;

    int i = 0;
    while (argv[1][i] != '\0'){

        if (argv[1][i] == 'u'){
            u = 1;
        }
        else if (argv[1][i] == 'g'){
            g = 1;
        }
        else if (argv[1][i] == 'o'){
            o = 1;
        }
        else if (argv[1][i] == 'a'){
            a = 1;
        }
        i++;

        if (argv[1][i] == '+' || argv[1][i] == '-' || argv[1][i] == '='){
            operation = argv[1][i];
        }
        // i++;

        int j = i + 1;
        while (argv[1][j] != '\0'){
            switch(argv[1][j]){
                case 'r' 
                : permissions |= S_IRUSR | S_IRGRP | S_IROTH;
                break;
                case 'w'
                : permissions |= S_IWUSR | S_IWGRP || S_IWOTH;
                break;
                case 'x'
                : permissions |= S_IXUSR | S_IXGRP | S_IXOTH;
                break;
            }

        }
    }

    mode_t mode = strtol(argv[1], NULL, 8);

    char *filename = argv[2];

    if (chmod(filename, mode) == -1){
        perror("chmod");

        return 1;
    }

    return 0;


}