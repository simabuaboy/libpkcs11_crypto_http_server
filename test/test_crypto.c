#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <string.h>
#include <pkcs11-helper-1.0/pkcs11.h>

#include "../include/libpkcs11_crypto.h"

#define TEST_DATA "hello-pkcs11"
#define PIN "1234"
#define MODULE_PATH "/usr/lib/softhsm/libsofthsm2.so"

static void test_encrypt_decrypt_valid(void **state) {
    CK_BYTE encrypted[128] = {0};
    CK_ULONG encrypted_len = sizeof(encrypted);
    CK_BYTE decrypted[128] = {0};
    CK_ULONG decrypted_len = sizeof(decrypted);

    int res = pkcs11_encrypt_decrypt(
        MODULE_PATH,
        PIN,
        (const CK_BYTE *)TEST_DATA,
        strlen(TEST_DATA),
        encrypted,
        &encrypted_len,
        decrypted,
        &decrypted_len
    );

    assert_int_equal(res, 0);
    assert_int_equal(decrypted_len, strlen(TEST_DATA));
    assert_memory_equal(decrypted, TEST_DATA, decrypted_len);
}

static void test_path_invalid(void **state){
    CK_BYTE encrypted[128] = {0};
    CK_ULONG encrypted_len = sizeof(encrypted);
    CK_BYTE decrypted[128] = {0};
    CK_ULONG decrypted_len = sizeof(decrypted);

    int res = pkcs11_encrypt_decrypt(
        "wrong_path",
        PIN,
        (const CK_BYTE *)TEST_DATA,
        strlen(TEST_DATA),
        encrypted,
        &encrypted_len,
        decrypted,
        &decrypted_len
    );
    assert_int_equal(res, -1);
}

static void test_invalid_pin(void **state) {
    CK_BYTE encrypted[128] = {0};
    CK_ULONG encrypted_len = sizeof(encrypted);
    CK_BYTE decrypted[128] = {0};
    CK_ULONG decrypted_len = sizeof(decrypted);

    int res = pkcs11_encrypt_decrypt(
        MODULE_PATH,
        "wrong-pin",
        (const CK_BYTE *)TEST_DATA,
        strlen(TEST_DATA),
        encrypted,
        &encrypted_len,
        decrypted,
        &decrypted_len
    );

    assert_int_equal(res, -1);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_encrypt_decrypt_valid),
        cmocka_unit_test(test_path_invalid),
        cmocka_unit_test(test_invalid_pin),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
