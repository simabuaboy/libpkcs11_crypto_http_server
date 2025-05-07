#pragma once

#include <pkcs11-helper-1.0/pkcs11.h>

CK_OBJECT_HANDLE generate_aes_key(CK_FUNCTION_LIST_PTR p11, CK_SESSION_HANDLE session);
