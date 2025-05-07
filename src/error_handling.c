#include "error_handling.h"
#include <stdio.h>

CK_RV handle_rv(CK_RV rv, const char *msg) {
    if (rv != CKR_OK) {
        fprintf(stderr, "%s failed: 0x%lX\n", msg, rv);
        return rv;
    }
    return CKR_OK;
}
