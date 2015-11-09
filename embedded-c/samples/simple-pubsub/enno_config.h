#ifndef _ENNO_CONFIG_H
#define _ENNO_CONFIG_H

typedef enum {
    Protocol_MQTT = 1,
    Protocol_AMQP = 2,
    Protocol_HTTP = 3
} Communication_Protocol_t;

#define Communication_Protocol_t 1                //The default communication protocol is MQTT

#define ENNO_IOT_HOST  "139.217.10.99"
#define ENNO_IOT_PORT  1883

#define ENNO_IOT_TX_BUF_LEN 512
#define ENNO_IOT_RX_BUF_LEN 512


#endif
