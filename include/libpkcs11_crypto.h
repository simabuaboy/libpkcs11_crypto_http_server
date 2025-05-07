#pragma once

#include <pkcs11-helper-1.0/pkcs11.h>

int pkcs11_encrypt_decrypt(
    const char *module_path,
    const char *user_pin,
    const CK_BYTE *input_data,
    CK_ULONG input_len,
    CK_BYTE *output_encrypted,
    CK_ULONG *output_encrypted_len,
    CK_BYTE *output_decrypted,
    CK_ULONG *output_decrypted_len
);