#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int main(int argc, char * argv[]) {
    void *handle;
    char *error;

    handle = dlopen("/path/to/your/library.so", RTLD_NOW);
    if (!handle) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    // Use the library...

    dlclose(handle);
    return EXIT_SUCCESS;
}
