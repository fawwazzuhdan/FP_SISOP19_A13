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
#include <sys/wait.h>
#include <sys/types.h>

static const char *dirpath = "/home/fawwaz";
char array[1000][1000];
int flag = 0;

void bukadir(char *tmp)
{
    char fpath[1000];
    char path[1000];
    char sip[1000];
    char oke[1000];
    char iya[1000];
    char tidak[1000];
    pid_t tes;
    DIR *d;
    struct dirent *dir;
    d = opendir(tmp);
    while ((dir = readdir(d)) != NULL)
    {
        strcpy(sip, dir->d_name);
        sip[1] = '\0';
        if ((strcmp(dir->d_name, ".") != 0 || strcmp(dir->d_name, "..") != 0) && strcmp(sip, ".") != 0)
        {
            strcpy(path, tmp);
            if (dir->d_type == 8)
            {
                strcpy(path, tmp);
                char *extensions = strrchr(dir->d_name, '.');
                if (extensions != NULL)
                {
                    if (strcmp(extensions, ".mp3") == 0)
                    {
                        sprintf(fpath, "%s/%s", path, dir->d_name);
                        int j = strlen(fpath);
                        while (fpath[j] != '/')
                        {
                            j--;
                        }
                        snprintf(oke, j + 1, "%s", fpath);
                        if (strcmp(dirpath, oke) != 0)
                        {
                            tes = fork();
                            if (tes == 0)
                            {
                                strcpy(iya, dirpath);
                                char *arg[4] = {"cp", fpath, iya, NULL};
                                execv("/bin/cp", arg);
                            }
                            sprintf(tidak, "%s/%s", dirpath, fpath + j + 1);
                            strcpy(array[flag], tidak);
                            flag++;
                        }
                    }
                }
            }
            else if (dir->d_type == 4)
            {
                sprintf(fpath, "%s/%s", path, dir->d_name);
                bukadir(fpath);
            }
        }
    }
    closedir(d);
}

void xmp_destroy(void *private_data)
{
    pid_t tes;
    for (int i = 0; i < flag; i++)
    {
        tes = fork();
        if (tes == 0)
        {
            char *arg[4] = {"rm", array[i], NULL};
            execv("/bin/rm", arg);
        }
    }
}

static int xmp_getattr(const char *path, struct stat *stbuf)
{
    int res;
    char fpath[1000];
    sprintf(fpath, "%s%s", dirpath, path);
    res = lstat(fpath, stbuf);

    if (res == -1)
        return -errno;

    return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                       off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000];
    char sip[1000];
    if (strcmp(path, "/") == 0)
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else
        sprintf(fpath, "%s%s", dirpath, path);
    int res = 0;

    DIR *dp;
    struct dirent *de;

    (void)offset;
    (void)fi;

    dp = opendir(fpath);
    if (dp == NULL)
        return -errno;

    while ((de = readdir(dp)) != NULL)
    {
        struct stat st;
        memset(&st, 0, sizeof(st));
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;
        strcpy(sip, de->d_name);
        sip[1] = '\0';
        if ((strcmp(de->d_name, ".") != 0 || strcmp(de->d_name, "..") != 0) && strcmp(sip, ".") != 0)
        {
            if (de->d_type == 8)
            {
                char *extensions = strrchr(de->d_name, '.');
                if (extensions != NULL)
                {
                    if (strcmp(extensions, ".mp3") == 0)
                    {
                        res = (filler(buf, de->d_name, &st, 0));
                        if (res != 0)
                            break;
                    }
                }
            }
            // if (de->d_type == 8)
            // {
            //     if (strcmp(de->d_name, ".") != 0 || strcmp(de->d_name, "..") != 0)
            //     {
            //         res = (filler(buf, de->d_name, &st, 0));
            //         if (res != 0)
            //             break;
            //     }
            // }
        }
    }

    closedir(dp);
    return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
                    struct fuse_file_info *fi)
{
    char fpath[1000];
    if (strcmp(path, "/") == 0)
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else
        sprintf(fpath, "%s%s", dirpath, path);
    int res = 0;
    int fd = 0;

    (void)fi;
    fd = open(fpath, O_RDONLY);
    if (fd == -1)
        return -errno;

    res = pread(fd, buf, size, offset);
    if (res == -1)
        res = -errno;

    close(fd);
    return res;
}

static struct fuse_operations xmp_oper = {
    .getattr = xmp_getattr,
    .readdir = xmp_readdir,
    .read = xmp_read,
    .destroy = xmp_destroy,
};

int main(int argc, char *argv[])
{
    char tes[1000];
    strcpy(tes, dirpath);
    bukadir(tes);
    // for (int i = 0; i < flag; i++)
    // {
    //     // printf("%d", i);
    //     printf("%s\n", array[i]);
    // }
    umask(0);
    return fuse_main(argc, argv, &xmp_oper, NULL);
}