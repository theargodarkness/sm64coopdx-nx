#ifndef SOCKET_H
#define SOCKET_H

#ifdef WINSOCK
#include "socket_windows.h"
#else
#include "socket_linux.h"
#endif

#include "../network.h"

extern struct NetworkSystem gNetworkSystemSocket;

extern char gGetHostName[];

// set when the system has no IPv6 support and the socket had to be created as IPv4:
// addresses are still handled as IPv4-mapped IPv6 everywhere but at the socket calls
extern bool gSocketIPv4Only;

SOCKET socket_initialize(void);
void socket_shutdown(SOCKET socket);

#endif
