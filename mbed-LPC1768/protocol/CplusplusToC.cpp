/*
 * CplusplusToC.c
 *
 *  Created on: 2015年11月20日
 *      Author: v-chawei
 */
#include "MQTTEthernet.h"
#include "MQTTClient.h"
#include "string.h"
#include "enno_api.h"

static MQTTEthernet ipstack = MQTTEthernet();
static MQTT::Client<MQTTEthernet, Countdown> client = MQTT::Client<MQTTEthernet, Countdown>(ipstack);
int arrivedcount=0;
int symbol=0;

extern "C"{
    int trans_connect(char *hostURL, uint16_t port, char *appKey);
    int trans_subscribe(char *topic, char *topicType);
    int trans_publish(char *path, char *payload, int payloadlen);
    int trans_unsubscribe(char *pTopic);
    int trans_disconnect();
    int trans_loop(int timeout);
}

 int trans_connect(char *hostURL, uint16_t port, char *appKey){
     int i = 0;
     if( symbol==0 ){
         ipstack.start();
         symbol=1;
     }
     i = ipstack.connect(hostURL, port);
     if (i == 0 ){
         MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
         data.MQTTVersion = 3;
         data.clientID.cstring = "mbed-sample";
         data.username.cstring = NULL;
         data.password.cstring = NULL;
         if ((i = client.connect(data)) != 0)
             printf("Error: connect failed %d\n", i);
     }
     return i;
}


 void systemMessageHandler(MQTT::MessageData& md)
 {
    uint8_t buffer[512];
     MQTT::Message &message = md.message;
     printf("system message received.\n");
     printf("Message arrived: qos %d, retained %d, dup %d, packetid %d\n", message.qos, message.retained, message.dup, message.id);
     printf("Payload %.*s\n", message.payloadlen, (char*)message.payload);
     ++arrivedcount;
     ennoSystemMessageHandler(md.topicName.cstring, (char*)message.payload, message.payloadlen);
 }
 void commandMessageHandler(MQTT::MessageData& md)
  {
    uint8_t buffer[512];
      MQTT::Message &message = md.message;
      printf("command message received.\n");
      printf("Message arrived: qos %d, retained %d, dup %d, packetid %d\n", message.qos, message.retained, message.dup, message.id);
      printf("Payload %.*s\n", message.payloadlen, (char*)message.payload);
      ++arrivedcount;
  }
int trans_subscribe(char *topic, char *topicType){
     int i = 0;
     if((strcmp(topicType,"system")) == 0){
         if ((i = client.subscribe(topic, MQTT::QOS2, systemMessageHandler)) != 0){
             printf("Error: subscribe failed %d\n", i);
         }
     }else{
         if ((i = client.subscribe(topic, MQTT::QOS2, commandMessageHandler)) != 0){
                     printf("Error: subscribe failed %d\n", i);
          }
     }
     return i;
}
int trans_publish(char *path, char *payload, int payloadlen){
     int i = 0;
     MQTT::Message message;
     message.qos = MQTT::QOS0;
     message.retained = false;
     message.dup = false;
     message.payload = payload;
     message.payloadlen = payloadlen;
     if ((i = client.publish(path, message)) != 0){
        printf("Error: publish failed %d\n", i);
     }
     return i;
}
int trans_unsubscribe(char *pTopic){
    int i = 0;
    if ((i = client.unsubscribe(pTopic))!= 0){
        printf("Error: unsubscribe failed %d\n", i);
    }
    return i;
}
int trans_disconnect(){
    int i = 0;
    if ((i = client.disconnect()) != 0)
         printf("Error: disconnect failed %d\n", i);
    if(i == 0 ) {
        ipstack.disconnect();
    }
    return i;
}
int trans_loop(int timeout){
    int i = 0;
    i = client.yield(timeout);
    return i;
}
