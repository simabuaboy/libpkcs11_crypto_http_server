#include <ulfius.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include "libpkcs11_crypto.h"

#include <openssl/evp.h>
#include <openssl/buffer.h>

char* base64_encode(const unsigned char *input, int length) {
    BIO *bmem = NULL, *b64 = NULL;
    BUF_MEM *bptr;
    b64 = BIO_new(BIO_f_base64());
    bmem = BIO_new(BIO_s_mem());
    b64 = BIO_push(b64, bmem);
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    BIO_write(b64, input, length);
    BIO_flush(b64);
    BIO_get_mem_ptr(b64, &bptr);
    char *buff = (char *)malloc(bptr->length + 1);
    memcpy(buff, bptr->data, bptr->length);
    buff[bptr->length] = 0;
    BIO_free_all(b64);
    return buff;
}

int callback_crypto_post(const struct _u_request *request, struct _u_response *response, void *user_data) {
    json_error_t error;
    json_t *json_req = ulfius_get_json_body_request(request, &error);
    if (!json_req) {
        ulfius_set_string_body_response(response, 400, "Invalid JSON");
        return U_CALLBACK_CONTINUE;
    }

    const char *data = json_string_value(json_object_get(json_req, "data"));
    const char *pin = json_string_value(json_object_get(json_req, "pin"));
    const char *module = json_string_value(json_object_get(json_req, "module"));

    if (!data || !pin || !module) {
        ulfius_set_string_body_response(response, 400, "Missing 'data', 'pin' or 'module'");
        json_decref(json_req);
        return U_CALLBACK_CONTINUE;
    }

    CK_BYTE encrypted[1024];
    CK_ULONG encrypted_len = sizeof(encrypted);
    CK_BYTE decrypted[1024];
    CK_ULONG decrypted_len = sizeof(decrypted);

    int result = pkcs11_encrypt_decrypt(
        module,
        pin,
        (const CK_BYTE *)data,
        strlen(data),
        encrypted,
        &encrypted_len,
        decrypted,
        &decrypted_len
    );

    json_decref(json_req);

    if (result != 0) {
        ulfius_set_string_body_response(response, 500, "Encryption/Decryption failed");
        return U_CALLBACK_CONTINUE;
    }

    char *enc_b64 = base64_encode(encrypted, encrypted_len);
    char *decrypted_str = (char *)malloc(decrypted_len + 1);
    memcpy(decrypted_str, decrypted, decrypted_len);
    decrypted_str[decrypted_len] = '\0';

    json_t *json_resp = json_object();
    json_object_set_new(json_resp, "encrypted", json_string(enc_b64));
    json_object_set_new(json_resp, "decrypted", json_string(decrypted_str));

    ulfius_set_json_body_response(response, 200, json_resp);

    free(enc_b64);
    free(decrypted_str);
    json_decref(json_resp);
    return U_CALLBACK_CONTINUE;
}

int main(void) {
    struct _u_instance instance;

    if (ulfius_init_instance(&instance, 8080, NULL, NULL) != U_OK) {
        fprintf(stderr, "Error initializing Ulfius\n");
        return 1;
    }

    ulfius_add_endpoint_by_val(&instance, "POST", "/crypto", NULL, 0, &callback_crypto_post, NULL);

    if (ulfius_start_framework(&instance) == U_OK) {
        printf("REST server running at http://localhost:8080\n");
        getchar();
    } else {
        fprintf(stderr, "Error starting Ulfius framework\n");
    }

    ulfius_stop_framework(&instance);
    ulfius_clean_instance(&instance);
    return 0;
}
