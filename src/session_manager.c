#include "session_manager.h"
#include "error_handling.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

CK_SESSION_HANDLE session;

int cek_session(CK_FUNCTION_LIST_PTR p11,const char *user_pin){
    CK_SLOT_ID slots[10];
    CK_ULONG slotCount = 10;
    handle_rv(p11->C_GetSlotList(CK_TRUE, slots, &slotCount), "GetSlotList");
    if (slotCount == 0) {
        fprintf(stderr, "No token found\n");
        return -1;
    }

    handle_rv(p11->C_OpenSession(slots[0], CKF_SERIAL_SESSION | CKF_RW_SESSION, NULL, NULL, &session), "OpenSession");
    if(handle_rv(p11->C_Login(session, CKU_USER, (CK_UTF8CHAR_PTR)user_pin, strlen(user_pin)), "Login") == CKR_PIN_INCORRECT) return -1;
    return 0;
}

CK_SESSION_HANDLE initialize_session() {
    
    return session;
}

void cleanup_session(CK_FUNCTION_LIST_PTR p11, CK_SESSION_HANDLE session) {
    p11->C_Logout(session);
    p11->C_CloseSession(session);
    p11->C_Finalize(NULL);
}