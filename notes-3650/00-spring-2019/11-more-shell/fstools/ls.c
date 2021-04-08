
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <assert.h>


int
main(int _ac, char* _av[])
{
    int rv;
    DIR* wd = opendir(".");
    assert(wd != 0);

    printf("name\tmode\tsize\n");

    struct dirent* ent;
    struct stat st;
    // show: man readdir
    while ((ent = readdir(wd))) {
        char* name = ent->d_name;
        rv = stat(name, &st);
        assert(rv != -1);

        printf("%s\t%04o\t%ld\n", name, st.st_mode, st.st_size);
    }

    closedir(wd);
    return 0;
}


