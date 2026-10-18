#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <time.h>
#include <grp.h>
#include <unistd.h>

int main(int argc, char *argv[]){

    int opt;
    int l = 0;
    int a = 0;

    while ((opt = getopt(argc, argv, "la")) != -1){

        switch(opt){
            case 'l':
                l = 1;
                break;
            case 'a':
                a = 1;
                break;
            default:
                break;

        }
    }

    char *path = ".";
    if (optind < argc){

        path = argv[optind];

    }

    struct dirent **namelist;

    int count = scandir(path, &namelist, NULL, alphasort);

    if (count == -1){
        perror("scandir");

        return 1;
    }

    long total = 0;

    if (l == 1){
            for (int i = 0; i < count; i++){
                char *name = namelist[i]->d_name;

                if (!a && name[0] == '.'){
                    continue;
                }

                char full_path[5000];
                snprintf(full_path, sizeof(full_path), "%s/%s", path, name);

                struct stat info;
                if (lstat(full_path, &info) == -1){
                    continue;
                }

                total += info.st_blocks;
            }
            printf("total %ld\n", total / 2);
    }

    int columns = 0;
    for (int i = 0; i < count; i++){
        char *name = namelist[i]->d_name;
        
        if (!a && name[0] == '.'){
            free(namelist[i]);
            continue;
        }

        if (l == 1) {

        char full_path[5000];

        snprintf(full_path, sizeof(full_path), "%s/%s", path, name);

        struct stat info;

        if (lstat(full_path, &info) == -1) {
            perror("lstat");
            free(namelist[i]);
            continue;
        }

        struct passwd *pw;

        pw = getpwuid(info.st_uid);

        struct group *gr;

        gr = getgrgid(info.st_gid);

        struct tm *tm_info;

        tm_info = localtime(&info.st_mtime);
        char str_time[80];
        
        if (S_ISDIR(info.st_mode))
            printf("d");
        else if (S_ISLNK(info.st_mode))
            printf("l");
        else
            printf("-");


        printf("%c", (info.st_mode & S_IRUSR) ? 'r' : '-');
        printf("%c", (info.st_mode & S_IWUSR) ? 'w' : '-');
        printf("%c", (info.st_mode & S_IXUSR) ? 'x' : '-');

        printf("%c", (info.st_mode & S_IRGRP) ? 'r' : '-');
        printf("%c", (info.st_mode & S_IWGRP) ? 'w' : '-');
        printf("%c", (info.st_mode & S_IXGRP) ? 'x' : '-');

        printf("%c", (info.st_mode & S_IROTH) ? 'r' : '-');
        printf("%c", (info.st_mode & S_IWOTH) ? 'w' : '-');
        printf("%c", (info.st_mode & S_IXOTH) ? 'x' : '-');

        printf(" %ld",info.st_nlink);

        if (pw == NULL){
            printf(" %d", info.st_uid);
        }
        else{
            printf(" %s", pw->pw_name);
        }

        if (gr == NULL){
            printf(" %d", info.st_gid);
        }
        else{
            printf(" %s", gr->gr_name);
        }

        strftime(str_time, sizeof(str_time), "%b %e %H:%M", tm_info);

        printf("%6ld ", (long)info.st_size);
        printf("%s ", str_time);

        if(S_ISDIR(info.st_mode)){
            printf("\033[34m");
        }
        else if (S_ISLNK(info.st_mode)){
            printf("\033[36m");
        }
        else if (info.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)){
            printf("\033[32m");
        }

        printf("%s\033[0m", name);

        if (S_ISLNK(info.st_mode)) {
            char target[4096];
            ssize_t len = readlink(full_path, target, sizeof(target) - 1);

            if (len != -1) {
                target[len] = '\0';
                printf(" -> %s", target);
            }
        }
        printf("\n");

    } else {
        char full_path[5000];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, name);

        struct stat info;
        if (lstat(full_path, &info) == -1) {
            perror("lstat");
            free(namelist[i]);
            continue;
        }

        if (S_ISDIR(info.st_mode)) {
            printf("\033[34m");
        }
        else if (S_ISLNK(info.st_mode)) {
            printf("\033[36m");
        }
        else if (info.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)) {
            printf("\033[32m");
        }

        if (columns > 0){
            printf("  ");
        }
        printf("%s\033[0m", name);
        columns++;

    }

    free(namelist[i]);
    }
    if (l == 0 && columns != 0){
            printf("\n");
        }

    free(namelist);

    return 0;

}