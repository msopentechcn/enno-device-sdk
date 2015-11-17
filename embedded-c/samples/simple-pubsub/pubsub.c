#include <stdio.h>
#include <unistd.h>
#include <memory.h>
#include <stdint.h>
#include <stdlib.h>
#include "enno_api.h"

void disconnectCallbackHandler(void) {
    printf("MQTT Disconnect");
}

EnnoMessages EnnoMessagesDefault = {
        .messageType = 'R',
        .Register = {.pHardwareId = "enno-default-test-id-5", .pSpecificationToken = "7dfd6d63-5e8d-4380-be04-fc5c73801dfb", .pOriginator = NULL},
        .Acknowledge = {.pMessage = NULL, .pOriginator = NULL},
        .Measurement = {.pName = "Temperature", .pOriginator = NULL, .value = 16, .eventDate = 0},
        .Location = {.pOriginator = NULL, .lat = 27.59, .lon = 86.5, .ele = 8844.43, .eventDate = 0},
        .Alert = {.pAlertType = "enno.info", .pAlertMessage = "I am alive", .pOriginator = NULL, .eventDate = 0}
};

int main(void) {

    Error_t rc = NONE_ERROR;

    char *ennoSystemPrefix = "SiteWhere/system/";
    char *ennoSystemInbound = "SiteWhere/input/protobuf";

    char *hostURL = "139.217.10.99";
    uint16_t port = 1883;
    char *appKey = NULL;
    char *deviceId = "enno_default-test-alpha-4";

    printf("Connecting to enno server %s:%d...\n",hostURL,port);
    rc = enno_connect(hostURL, port, appKey);

    if (NONE_ERROR != rc) {
        printf("Error(%d) on connecting.\n", rc);
        exit(rc);
    }
    printf("connected.\n");

    printf("Subscribing system topic...");
    char systemTopic[64];
    stpcpy(systemTopic,ennoSystemPrefix);
    strcat(systemTopic,deviceId);
    rc = enno_subscribe(systemTopic, ennoSystemMessageHandler);
    if (NONE_ERROR != rc) {
        printf("Error on subscrie system topic.");
        exit(rc);
    }
    printf("subscribed.\n");


    printf("Send device registeration...");
    EnnoMessages Messages_enno = EnnoMessagesDefault;
    Messages_enno.Register.pHardwareId = deviceId;
    Messages_enno.messageType = 'R';
    rc = enno_publish(ennoSystemInbound, &Messages_enno);
    if (NONE_ERROR != rc) {
        printf("Error on Registering device.\n");
        exit(rc);
    }
    printf("sent.\n");

    Messages_enno.messageType = 'L';
    // main loop
    while (NONE_ERROR == rc) {
        enno_loop(1000);
        sleep(1);
        rc = enno_publish(ennoSystemInbound, &Messages_enno);
        printf("Location updated.\n");
    }

    return rc;

}
