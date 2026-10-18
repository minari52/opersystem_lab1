#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

int main(int argc, char *argv[]){
    int opt;
    int n = 0;
    int b = 0;
    int E = 0;
    while ((opt = getopt(argc, argv, "nbE")) != -1){
        switch (opt){
            case 'n':
                n = 1;
                break;
            case 'b':
                b = 1;
                break;
            case 'E':
                E = 1;
                break;
            default:
                break;
        }
    }
    
    if (b == 1 && n == 1){
        n = 0;
    }

    FILE *file = fopen(argv[optind],"r");
    if (file == NULL){
        printf("No such file or directory\n");
        return 1;
    }

    int number_line = 1;
    int ch;
    int start_line = 1;
    while ((ch = fgetc(file)) != EOF){
        if (start_line){
            if (n == 1){
                printf("%6d\t", number_line++);
            } else if (b == 1 && (ch != '\n')){
                printf("%6d\t", number_line++);
            }
            
        }

        if (E == 1 && ch == '\n'){
            printf("$");
        }

        printf("%c", ch);

        if (ch == '\n') start_line = 1;
        else start_line = 0;
    }

    return 0;
}