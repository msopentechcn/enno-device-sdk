#include <stdio.h>
#include <unistd.h>

#include <memory.h>
#include <stdint.h>
#include "enno_mqtt_interface.h"

void disconnectCallbackHandler(void) {
    printf("MQTT Disconnect");
}


int main(void) {

    Error_t rc = NONE_ERROR;

    char *deviceId = "enno-default-test-id-2";
    char *ennoSystemPrefix = "SiteWhere/system/";
    char *ennoSystemInbound = "SiteWhere/input/protobuf";

    ConnectParams connectParams = ConnectParamsDefault;
    connectParams.pClientID = "paho-340629286256420";
    connectParams.pHostURL = "139.217.10.99";
    connectParams.port = 1883;
    connectParams.isWillMsgPresent = false;
    connectParams.mqttCommandTimeout_ms = 2000;
    connectParams.disconnectHandler = disconnectCallbackHandler;

    printf("Connecting to enno server %s:%d...", connectParams.pHostURL, connectParams.port);

    rc = enno_connect(&connectParams);

    if (NONE_ERROR != rc) {
        printf("Error(%d) on connecting.\n", rc);
        exit(rc);
    }

    printf("connected.\n");


    printf("Subscribing system topic...");
    SubscribeParams subParams = SubscribeParamsDefault;
    char systemTopic[64];
    strcpy(systemTopic, ennoSystemPrefix);
    strcat(systemTopic, deviceId);
    subParams.qos = QOS_0;
    rc = enno_subscribe(systemTopic, ennoSystemMessageHandler, &subParams);
    if (NONE_ERROR != rc) {
        printf("Error on subscribing system topic.");
        exit(rc);
    }
    printf("subscribed.\n");


    printf("Send device registeration...");
    PublishParams publishParams = PublishParamsDefault;
    publishParams.pTopic = ennoSystemInbound;

    EnnoParams Params_enno = EnnoParamsDefault;
    Params_enno.Register.pHardwareId = deviceId;

    rc = enno_publish('R', &Params_enno, &publishParams);
    if (NONE_ERROR != rc) {
        printf("Error on Registering device.\n");
        exit(rc);
    }
    printf("sent.\n");


    // main loop
    while (NONE_ERROR == rc) {
        enno_loop(1000);
        sleep(1);
        rc = enno_publish('L', &Params_enno, &publishParams);
        printf("Location updated.\n");
    }

    return rc;

}
