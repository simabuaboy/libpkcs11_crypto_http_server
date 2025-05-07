#include "key_manager.h"
#include "error_handling.h"

CK_OBJECT_HANDLE generate_aes_key(CK_FUNCTION_LIST_PTR p11, CK_SESSION_HANDLE session) {
    CK_MECHANISM keyGenMech = {CKM_AES_KEY_GEN, NULL, 0};
    CK_OBJECT_HANDLE aesKey;
    CK_BBOOL trueVal = CK_TRUE;
    CK_ULONG keyLen = 32;
    CK_ATTRIBUTE keyTemplate[] = {
        {CKA_CLASS, &(CK_OBJECT_CLASS){CKO_SECRET_KEY}, sizeof(CK_OBJECT_CLASS)},
        {CKA_KEY_TYPE, &(CK_KEY_TYPE){CKK_AES}, sizeof(CK_KEY_TYPE)},
        {CKA_VALUE_LEN, &keyLen, sizeof(keyLen)},
        {CKA_ENCRYPT, &trueVal, sizeof(trueVal)},
        {CKA_DECRYPT, &trueVal, sizeof(trueVal)},
        {CKA_TOKEN, &(CK_BBOOL){CK_FALSE}, sizeof(CK_BBOOL)}
    };
    handle_rv(p11->C_GenerateKey(session, &keyGenMech, keyTemplate, 6, &aesKey), "GenerateKey");
    return aesKey;
}
