#include "module_loader.h"
#include "error_handling.h"
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *handle = NULL;
void *sym = NULL;

int cek_module_path(const char *module_path){
    handle = dlopen(module_path, RTLD_NOW);
    if (!handle) {
        fprintf(stderr, "Failed to load module\n");
        return -1;
    }

    sym = dlsym(handle, "C_GetFunctionList");
    if (!sym) {
        fprintf(stderr, "dlsym failed\n");
        return -1;
    }
    return 0;
}


CK_FUNCTION_LIST_PTR load_pkcs11_module() {

    CK_C_GetFunctionList getFuncList;
    memcpy(&getFuncList, &sym, sizeof(sym));

    CK_FUNCTION_LIST_PTR p11;
    handle_rv(getFuncList(&p11), "C_GetFunctionList");
    handle_rv(p11->C_Initialize(NULL), "C_Initialize");

    return p11;
}