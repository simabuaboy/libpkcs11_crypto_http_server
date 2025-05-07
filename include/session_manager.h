#pragma once

#include <pkcs11-helper-1.0/pkcs11.h>

int cek_session(CK_FUNCTION_LIST_PTR p11,const char *user_pin);
CK_SESSION_HANDLE initialize_session();
void cleanup_session(CK_FUNCTION_LIST_PTR p11, CK_SESSION_HANDLE session);