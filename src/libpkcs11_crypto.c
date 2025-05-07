#include "libpkcs11_crypto.h"
#include "module_loader.h"
#include "session_manager.h"
#include "key_manager.h"
#include "crypto_engine.h"
#include "error_handling.h"
#include <string.h>
#include <stdio.h>

int pkcs11_encrypt_decrypt(
    const char *module_path,
    const char *user_pin,
    const CK_BYTE *input_data,
    CK_ULONG input_len,
    CK_BYTE *output_encrypted,
    CK_ULONG *output_encrypted_len,
    CK_BYTE *output_decrypted,
    CK_ULONG *output_decrypted_len
) {
    if(cek_module_path(module_path) == -1){
        return -1;
    }
    CK_FUNCTION_LIST_PTR p11 = load_pkcs11_module(module_path);
    
    if(cek_session(p11, user_pin) == -1){
        return -1;
    }
    CK_SESSION_HANDLE session = initialize_session(p11, user_pin);
    
    CK_OBJECT_HANDLE aesKey = generate_aes_key(p11, session);

    encrypt_data(p11, session, aesKey, input_data, input_len, output_encrypted, output_encrypted_len);
    
    decrypt_data(p11, session, aesKey, output_encrypted, *output_encrypted_len, output_decrypted, output_decrypted_len);

    cleanup_session(p11, session);
    return 0;
}
