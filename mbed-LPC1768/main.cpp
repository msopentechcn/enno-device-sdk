/*
 * ennoForMbed.c
 *
 *  Created on: 2015年11月20日
 *      Author: v-chawei
 */
#include "mbed.h"
#include "MQTTClient.h"
#include "enno_api.h"


#define MQTTCLIENT_QOS2 1
DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);


int main(int argc, char* argv[]){
    Error_t rc = NONE_ERROR;
    float version = 0.5;
    char *ennoCommandPrefix = "SiteWhere/commands/";
    char *ennoSystemPrefix = "SiteWhere/system/";
    char *ennoSystemInbound = "SiteWhere/input/protobuf";
    char *deviceId = "enno-mbed-test-id-1";
    char *topicType = "command";
    char *hostURL = "139.217.10.99";
    int port = 1883;
    char *appKey = NULL;

    led1=1;led2=1;led3=1;led4=1;                    //light on
    wait(0.5);
    led1=!led1;led2=!led2;led3=!led3;led4=!led4;    //light off
    wait(0.5);
    led1=!led1;led2=!led2;led3=!led3;led4=!led4;    //light on
    wait(0.5);
    led1=!led1;led2=!led2;led3=!led3;led4=!led4;    //light off

    printf("HelloMQTT: version is %f\n", version);

    printf("Connecting to enno server %s:%d...\n",hostURL,port);

    rc = enno_connect(hostURL, port, appKey);
    if (NONE_ERROR != rc) {
        printf("Error(%d) on connecting.\n", rc);
        exit(rc);
    }
    printf("connected.\n");

    printf("Subscribing system topic...");
    char systemTopic[64];
    char commandTopic[64];
    strcpy(systemTopic,ennoSystemPrefix);
    strcat(systemTopic,deviceId);

    rc = enno_subscribe(systemTopic, topicType);
    if (NONE_ERROR != rc) {
       printf("Error on subscrie system topic.");
       exit(rc);
    }
   printf("subscribed.\n");

    printf("Send device registeration...\n");

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
        rc = enno_publish(ennoSystemInbound, &Messages_enno);
        led1=!led1;
        printf("Location updated.\n");
    }

    return rc;
}





