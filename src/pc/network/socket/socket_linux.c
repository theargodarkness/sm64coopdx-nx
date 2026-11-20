#ifndef WINSOCK
#include <stdbool.h>
#include "socket_linux.h"

extern bool gSocketIPv4Only;
#include "../network.h"
#include "pc/debuglog.h"

SOCKET socket_initialize(void) {
    // initialize socket
    SOCKET sock = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
    gSocketIPv4Only = false;
    if (sock == INVALID_SOCKET) {
        // no IPv6 on this system, fall back to a plain IPv4 socket
        sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        gSocketIPv4Only = (sock != INVALID_SOCKET);
    }
    if (sock == INVALID_SOCKET) {
        LOG_ERROR("socket failed with error %d", SOCKET_LAST_ERROR);
        return INVALID_SOCKET;
    }

    // set non-blocking mode
    int rc = fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) | O_NONBLOCK);
    if (rc == (int)INVALID_SOCKET) {
        LOG_ERROR("fcntl failed with error: %d", rc);
        return INVALID_SOCKET;
    }

    // Set socket to dual-stack
    int v6only = 0;
    if (!gSocketIPv4Only && setsockopt(sock, IPPROTO_IPV6, IPV6_V6ONLY, (void *)&v6only, sizeof(v6only)) < 0) {
        LOG_ERROR("setsockopt(IPV6_V6ONLY) failed.");
        return INVALID_SOCKET;
    };

    LOG_INFO("socket initialized.");

    return sock;
}

void socket_shutdown(SOCKET socket) {
    if (socket == INVALID_SOCKET) { return; }
    int rc = closesocket(socket);
    if (rc == (int)SOCKET_ERROR) {
        LOG_ERROR("closesocket failed with error %d\n", SOCKET_LAST_ERROR);
    }
}

#endif
