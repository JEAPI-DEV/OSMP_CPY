/******************************************************************************
 * FILE: osmpLib.c
 * AUTHOR: BS-Team
 * DESCRIPTION:
 * In dieser Datei sind Funktionen der OSMP Bibliothek implementiert
 ******************************************************************************/

#include "osmpLib.h"

int OSMP_GetSuccess(void) {
    return OSMP_SUCCESS;
}

int OSMP_GetMaxMessagesProc(void) {
    return OSMP_MAX_MESSAGES_PROC;
}

int OSMP_GetMaxSlots(void) {
    return OSMP_MAX_SLOTS;
}

int OSMP_GetMaxPayloadLength(void) {
    return OSMP_MAX_PAYLOAD_LENGTH;
}

int OSMP_GetMaxShmNameLength(void) {
    return OSMP_MAX_SHM_NAME_LENGTH;
}

int OSMP_GetMaxLogNameLength(void) {
    return OSMP_MAX_LOGNAME_LENGTH;
}

int OSMP_Init(const int *argc, char ***argv) {
    UNUSED(argc);
    UNUSED(argv);

    // TODO: Implementieren Sie hier die Funktionalität der Funktion.
    return OSMP_NOT_IMPLEMENTED;
}

int OSMP_SizeOf(OSMP_Datatype datatype, unsigned int *size) {
    UNUSED(datatype);
    UNUSED(size);

    // TODO: Implementieren Sie hier die Funktionalität der Funktion.
    return OSMP_NOT_IMPLEMENTED;
}

int OSMP_Size(int *size) {
    UNUSED(size);

    // TODO: Implementieren Sie hier die Funktionalität der Funktion.
    return OSMP_NOT_IMPLEMENTED;
}

int OSMP_Rank(int *rank) {
    UNUSED(rank);

    // TODO: Implementieren Sie hier die Funktionalität der Funktion.
    return OSMP_NOT_IMPLEMENTED;
}

int OSMP_Send(const void *buf, int count, OSMP_Datatype datatype, int dest) {
    UNUSED(buf);
    UNUSED(count);
    UNUSED(datatype);
    UNUSED(dest);

    // TODO: Implementieren Sie hier die Funktionalität der Funktion.
    return OSMP_NOT_IMPLEMENTED;
}

int OSMP_Recv(void *buf, int count, OSMP_Datatype datatype, int *source,
              int *len) {
    UNUSED(buf);
    UNUSED(count);
    UNUSED(datatype);
    UNUSED(source);
    UNUSED(len);

    // TODO: Implementieren Sie hier die Funktionalität der Funktion.
    return OSMP_NOT_IMPLEMENTED;
}

int OSMP_Finalize(void) {
    // TODO: Implementieren Sie hier die Funktionalität der Funktion.
    return OSMP_NOT_IMPLEMENTED;
}

int OSMP_Barrier(void) {
    // TODO: Implementieren Sie hier die Funktionalität der Funktion.
    return OSMP_NOT_IMPLEMENTED;
}

int OSMP_Gather(void *sendbuf, int sendcount, OSMP_Datatype sendtype,
                void *recvbuf, int recvcount, OSMP_Datatype recvtype,
                int root) {
    UNUSED(sendbuf);
    UNUSED(sendcount);
    UNUSED(sendtype);
    UNUSED(recvbuf);
    UNUSED(recvcount);
    UNUSED(recvtype);
    UNUSED(root);

    // TODO: Implementieren Sie hier die Funktionalität der Funktion.
    return OSMP_NOT_IMPLEMENTED;
}

int OSMP_ISend(const void *buf, int count, OSMP_Datatype datatype, int dest,
               OSMP_Request request) {
    UNUSED(buf);
    UNUSED(count);
    UNUSED(datatype);
    UNUSED(dest);
    UNUSED(request);

    // TODO: Implementieren Sie hier die Funktionalität der Funktion.
    return OSMP_NOT_IMPLEMENTED;
}

int OSMP_IRecv(void *buf, int count, OSMP_Datatype datatype, int *source,
               int *len, OSMP_Request request) {
    UNUSED(buf);
    UNUSED(count);
    UNUSED(datatype);
    UNUSED(source);
    UNUSED(len);
    UNUSED(request);

    // TODO: Implementieren Sie hier die Funktionalität der Funktion.
    return OSMP_NOT_IMPLEMENTED;
}

int OSMP_Test(OSMP_Request request, int *flag) {
    UNUSED(request);
    UNUSED(flag);

    // TODO: Implementieren Sie hier die Funktionalität der Funktion.
    return OSMP_NOT_IMPLEMENTED;
}

int OSMP_Wait(OSMP_Request request) {
    UNUSED(request);

    // TODO: Implementieren Sie hier die Funktionalität der Funktion.
    return OSMP_NOT_IMPLEMENTED;
}

int OSMP_CreateRequest(OSMP_Request *request) {
    UNUSED(request);

    // TODO: Implementieren Sie hier die Funktionalität der Funktion.
    return OSMP_NOT_IMPLEMENTED;
}

int OSMP_RemoveRequest(OSMP_Request *request) {
    UNUSED(request);

    // TODO: Implementieren Sie hier die Funktionalität der Funktion.
    return OSMP_NOT_IMPLEMENTED;
}

int OSMP_Log(OSMP_Verbosity verbosity, char *message) {
    UNUSED(verbosity);
    UNUSED(message);

    // TODO: Implementieren Sie hier die Funktionalität der Funktion.
    return OSMP_NOT_IMPLEMENTED;
}
