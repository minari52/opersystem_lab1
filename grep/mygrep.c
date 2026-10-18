#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]){

    if (argc != 3 && argc != 2){

        fprintf(stderr, "Usage: %s pattern file\n", argv[0]);

        return 1;
    }


    char *pattern = argv[1];

    FILE *input;

    if (argc == 3){

        input = fopen(argv[2], "r");
        if (input == NULL){

        printf("No such file or directory\n");

        return 1;
        }
    }

    else if(argc == 2){

        input = stdin;
        
    }

    char line[1024];
    
    while(fgets(line, sizeof(line), input) != NULL){
        if(strstr(line, pattern) != NULL){

            printf("%s", line);

        }

        if (strlen(line) > 0 && line[strlen(line) - 1] != '\n'){

            printf("\n");
        }
    }
    if (argc == 3){

        fclose(input);

    }

    return 0;
    
}