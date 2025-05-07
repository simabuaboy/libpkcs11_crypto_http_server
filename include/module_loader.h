#pragma once

#include <pkcs11-helper-1.0/pkcs11.h>

int cek_module_path(const char *module_path);
CK_FUNCTION_LIST_PTR load_pkcs11_module();