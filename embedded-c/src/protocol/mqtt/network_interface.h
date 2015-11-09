#ifndef _NETWORK_INTERFACE_H
#define _NETWORK_INTERFACE_H

//Add the platform specfic network includes to the Network struct
#include "network_linux.h"

typedef struct Network Network;

struct Network {
    int my_socket;

    int (*mqttread)(Network *, unsigned char *, int, int);

    int (*mqttwrite)(Network *, unsigned char *, int, int);

    void (*disconnect)(Network *);
};


int linux_read(Network *, unsigned char *, int, int);

int linux_write(Network *, unsigned char *, int, int);

void linux_disconnect(Network *);

void NewNetwork(Network *);

int ConnectNetwork(Network *, char *, int);

#endif