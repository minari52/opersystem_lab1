#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>

int main(int argc, char *argv[]){

    if (argc != 3){
        printf("Usage: ./mychmod mode file\n");
        return 1;
    }

    if (argv[1][0] >= '0' && argv[1][0] <= '7'){
        char *end;
        long parser_mode = strtol(argv[1], &end, 8);
        if (*end != '\0' || parser_mode < 0 || parser_mode > 0777){
            printf("mychmod: invalid mode\n");
            return 1;
        }

        mode_t mode = (mode_t)parser_mode;
        char *filename = argv[2];

        if (chmod(filename, mode) == -1){
            perror("chmod");

            return 1;
        }

        return 0;

    }

    struct stat info;

    if (stat(argv[2], &info) == -1){
        perror("stat");
        return 1;
    }

    int categories = 0;
    int rights = 0;

    char op;
    mode_t permissions = 0;

    int i = 0;
    while (argv[1][i] != '\0' && argv[1][i] != '+' && argv[1][i] != '=' && argv[1][i] != '-'){
        if (argv[1][i] == 'u'){
            categories |= 1;
        }
        else if (argv[1][i] == 'g'){
            categories |= 2;
        }
        else if (argv[1][i] == 'o'){
            categories |= 4;
        }
        else if(argv[1][i] == 'a'){
            categories |= 7;
        }
        else{
            printf("mychmod: invalid mode\n");
            return 1;
        }

        i++;
    }
    if(categories == 0){
        categories = 7;
    }

    if (argv[1][i] == '+' || argv[1][i] == '-' || argv[1][i] == '='){
        op = argv[1][i];
    }
    else if(argv[1][i] == '\0'){
        printf("mychmod: invalid mode\n");
        return 1;
    }

    if (i == 0 && argv[1][1] == '\0') {
    printf("mychmod: invalid mode\n");
    return 1;
    }

    int j = i + 1;
    while(argv[1][j] != '\0'){  
        switch (argv[1][j]){
            case 'r'
            : rights |= 1;
            break;
            case 'w'
            : rights |= 2;
            break;
            case 'x'
            : rights |= 4;
            break;
            default :
            printf("mychmod: invalid mode\n");
            return 1;
        }
        j++;
    }

    if (categories & 1){
        if (rights & 1) permissions |= S_IRUSR;
        if (rights & 2) permissions |= S_IWUSR;
        if (rights & 4) permissions |= S_IXUSR;
    }

    if (categories & 2){
        if (rights & 1) permissions |= S_IRGRP;
        if (rights & 2) permissions |= S_IWGRP;
        if (rights & 4) permissions |= S_IXGRP;
    }

    if (categories & 4){
        if (rights & 1) permissions |= S_IROTH;
        if (rights & 2) permissions |= S_IWOTH;
        if (rights & 4) permissions |= S_IXOTH;
    }
    
    mode_t current_mode = info.st_mode & 0777;

    if (op == '+'){
        current_mode |= permissions;
        if (chmod(argv[2], current_mode) == -1){
            perror("chmod");
            return 1;
        }
    }

    if (op == '-'){
        current_mode &= ~permissions;
        if (chmod(argv[2], current_mode) == -1){
            perror("chmod");
            return 1;
        }
    }

    mode_t clear_mask = 0;
    if(categories & 1){
        clear_mask |= S_IRUSR | S_IWUSR | S_IXUSR;
    }   
    if(categories & 2){
        clear_mask |= S_IRGRP | S_IWGRP | S_IXGRP;
    }
    if(categories & 4){
        clear_mask |= S_IROTH | S_IWOTH | S_IXOTH;
    }

    if (op == '='){
        current_mode &= ~clear_mask;
        current_mode |= permissions;

        if(chmod(argv[2], current_mode) == -1){
            perror("chmod");
            return 1;
        }
    }

    return 0;


}
