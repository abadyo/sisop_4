#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <ctype.h>

static  const  char *dirpath = "/home/abad/tugas/s4/satu/percobaan";
const char *a[1000];
int m = 0;

char *remove_three(const char *filename) {
    size_t len = strlen(filename);
    char *newfilename = malloc(len-2);
    if (!newfilename) /* handle error */;
    memcpy(newfilename, filename, len-3);
    newfilename[len - 4] = 0;
    return newfilename;
}

char* encoder2(const char* kata) {
    char* potong = remove_three(kata);
    char* tes = (char *) malloc(1000);
    char* nodash = (char *) malloc(1000);
    char coba[1000];
    // printf("%s\n", potong);
    char *token = strtok(potong, "_");
    
    // strcpy(tes, token);
    while(token != NULL) {
        strcpy(coba, token);
        // coba[1] = 'a';
        for(int i = 0; i < strlen(coba); i++) {
            
            if(coba[i] >= 'A' && coba[i] <= 'Z') {
                coba[i] = 'Z' + 'A' - coba[i];
            } else if(coba[i] >= 'a' && coba[i] <= 'z') {
                if(coba[i] + 13 <= 'z') coba[i] += 13;
                else coba[i] -= 13;
            }
            // printf("%c\n", coba[i]);
        }
        strcat(tes, coba);
        strcat(tes, "_");
        //printf("%s\n", token);
        token = strtok(NULL, "_");
    }
    //printf("%s\n", token);
    int a = strlen(tes);
    sprintf(nodash, "%.*s.txt",a-1, tes);
    return nodash;
}

// char* encoder(const char* kata) {
//     char pilih[1000];
//     static char hasil2[1000];
//     char hasil[1000];
//     char* terpilih = remove_three(kata);
//     char proses[1000];
//     // char hasil[1000];
//     strcpy(pilih, terpilih);
//     // memmove(&pilih[0], &pilih[0 + 1], strlen(pilih) - 0);
//     char *token = strtok(pilih, "_");
//     while( token != NULL ) {
//         strcpy(proses, token);
//         if(proses[0]>='A'&&proses[0]<='Z') {
//             for(int i=0; i < strlen(proses); i++) {
//                 proses[i] = 'Z' + 'A' - proses[i];
//             }
//         } else if(proses[0] >= 'a' && proses[0] <= 'z') {
//             for(int j = 0; j < strlen(proses); j++) {
//                 if(proses[j] + 13 <= 'z') proses[j] += 13;
//                 else proses[j] -= 13;
//             }
//         }
//         strcat(hasil, proses);
//         strcat(hasil, "_");
//         token = strtok(NULL, "_");
//     }
//     int a = strlen(hasil);
//     sprintf(hasil2, "%.*s.txt",a-1, hasil);
//     return hasil2;
    
// }

static  int  xmp_getattr(const char *path, struct stat *stbuf)
{
    int res;
    char fpath[1000];
    sprintf(fpath,"%s%s",dirpath,path);
    res = lstat(fpath, stbuf);
    if (res == -1) return -errno;
    return 0;
}



static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000];
    if(strcmp(path,"/") == 0)
    {
        path=dirpath;
        sprintf(fpath,"%s",path);
    } else sprintf(fpath, "%s%s",dirpath,path);

    char anime[1000];
    strncpy(anime, fpath+strlen(fpath)-8, 8);

    if(!strcmp(anime, "Animeku_")) {  
        int q = 0;
        for(int i = 0; i < m; i++) {
            if(!strcmp(a[i], fpath)) {
                q = 1;
                break;
            }
        }
        printf("%s\n", fpath);
        // rename("/home/abad/tugas/s4/satu/percobaan/Animeku_/au_AH_ih.txt", "/home/abad/tugas/s4/satu/percobaan/Animeku_/breh_BRUH.txt");
        if(q == 0) {
            DIR *dp = opendir(fpath);
            struct dirent *ep;

            while((ep = readdir(dp)) != NULL) {
                if (strcmp(ep->d_name, ".") != 0 && strcmp(ep->d_name, "..") != 0 && strstr(ep->d_name, ".txt")) {
                    char* hasil = (char *) malloc(1000);
                    hasil = encoder2(ep->d_name);
                    char tes[2000];
                    char tus[2000];
                    printf("%s\n", hasil);
                    sprintf(tes, "%s/%s", fpath, ep->d_name);
                    sprintf(tus, "%s/%s", fpath, hasil);
                    printf("%s\n", tes);
                    printf("%s\n", tus);
                    
                    rename(tes, tus);
                }
            }
            a[m] = fpath;
            m++;
        } 
    }
    for(int j = 0; j < m; j++) {
        printf("%s\n", a[j]);
    }
    // printf("%s\n", anime);

    int res = 0;

    DIR *dp;
    struct dirent *de;
    (void) offset;
    (void) fi;

    dp = opendir(fpath);

    if (dp == NULL) return -errno;

    while ((de = readdir(dp)) != NULL) {
        struct stat st;

        memset(&st, 0, sizeof(st));

        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;
        char tes[1000];
        strcpy(tes, de->d_name);
        // printf("%s\n", tes);
        res = (filler(buf, tes, &st, 0));

        if(res!=0) break;
    }

    closedir(dp);

    return 0;
}

static int xmp_mkdir(const char *path, mode_t mode)
{
    char fpath[1000];
    if(strcmp(path,"/") == 0)
    {
        path=dirpath;

        sprintf(fpath,"%s",path);
    }
    else sprintf(fpath, "%s%s",dirpath,path);

	int res;

	res = mkdir(fpath, mode);
	if (res == -1)
		return -errno;

	return 0;
}



static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000];
    if(strcmp(path,"/") == 0)
    {
        path=dirpath;

        sprintf(fpath,"%s",path);
    }
    else sprintf(fpath, "%s%s",dirpath,path);

    int res = 0;
    int fd = 0 ;

    (void) fi;

    fd = open(fpath, O_RDONLY);

    if (fd == -1) return -errno;

    res = pread(fd, buf, size, offset);

    if (res == -1) res = -errno;

    close(fd);

    return res;
}

static int xmp_rename(const char *from, const char *to)
{
	int res;
    // printf("%s", from);
    // printf("%s", to);
    char tes1[1000];
    char tes2[1000];
    snprintf(tes1, 1000, "%s%s", dirpath, from);
    snprintf(tes2, 1000, "%s%s", dirpath, to);
	res = rename(tes1, tes2);
    encoder2(to);
    // printf("%s\n", encoder2(from));
    // printf("%s\n", encoder2(to));
	if (res == -1)
		return -errno;

	return 0;
}



static struct fuse_operations xmp_oper = {
    .getattr = xmp_getattr,
    .readdir = xmp_readdir,
    .read = xmp_read,
    .rename = xmp_rename,
    .mkdir = xmp_mkdir,
};



int  main(int  argc, char *argv[])
{
    umask(0);

    return fuse_main(argc, argv, &xmp_oper, NULL);
}