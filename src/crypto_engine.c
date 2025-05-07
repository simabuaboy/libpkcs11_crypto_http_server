#include "crypto_engine.h"
#include "error_handling.h"
#include <string.h>

void encrypt_data(CK_FUNCTION_LIST_PTR p11, CK_SESSION_HANDLE session, CK_OBJECT_HANDLE key,
                  const CK_BYTE *plaintext, CK_ULONG plainLen, CK_BYTE *ciphertext, CK_ULONG *cipherLen) {
    CK_BYTE iv[16] = {0};
    CK_MECHANISM mech = {CKM_AES_CBC_PAD, iv, sizeof(iv)};

    handle_rv(p11->C_EncryptInit(session, &mech, key), "EncryptInit");
    handle_rv(p11->C_Encrypt(session, (CK_BYTE_PTR)plaintext, plainLen, ciphertext, cipherLen), "Encrypt");
}

void decrypt_data(CK_FUNCTION_LIST_PTR p11, CK_SESSION_HANDLE session, CK_OBJECT_HANDLE key,
                  const CK_BYTE *ciphertext, CK_ULONG cipherLen, CK_BYTE *plaintext, CK_ULONG *plainLen) {
    CK_BYTE iv[16] = {0};
    CK_MECHANISM mech = {CKM_AES_CBC_PAD, iv, sizeof(iv)};

    handle_rv(p11->C_DecryptInit(session, &mech, key), "DecryptInit");
    handle_rv(p11->C_Decrypt(session, (CK_BYTE_PTR)ciphertext, cipherLen, plaintext, plainLen), "Decrypt");

}
