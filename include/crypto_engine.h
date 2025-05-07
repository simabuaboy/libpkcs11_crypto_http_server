#pragma once

#include <pkcs11-helper-1.0/pkcs11.h>

void encrypt_data(CK_FUNCTION_LIST_PTR p11, CK_SESSION_HANDLE session, CK_OBJECT_HANDLE key,
                  const CK_BYTE *plaintext, CK_ULONG plainLen, CK_BYTE *ciphertext, CK_ULONG *cipherLen);
void decrypt_data(CK_FUNCTION_LIST_PTR p11, CK_SESSION_HANDLE session, CK_OBJECT_HANDLE key,
                  const CK_BYTE *ciphertext, CK_ULONG cipherLen, CK_BYTE *plaintext, CK_ULONG *plainLen);
