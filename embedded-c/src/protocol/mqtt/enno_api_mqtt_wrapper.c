#include <MQTTPacket.h>
#include "enno_mqtt_interface.h"
#include "sitewhere.h"
#include "pb_decode.h"


static Network n;
static Client c;
//static iot_disconnect_handler clientDisconnectHandler;    //warning defined but not used

const MQTTWillOptions MQTTwillOptionsDefault = {
        .pTopicName = NULL,
        .pMessage = NULL,
        .isRetained = false,
        .qos = QOS_0
};
MQTTMessageParams MQTTMessageParamsDefault = {
        .qos = QOS_0,
        .isRetained=false,
        .isDuplicate = false,
        .id = 0,
        .pPayload = NULL,
        .PayloadLen = 0
};
ConnectParams ConnectParamsDefault = {
        .pHostURL =  ENNO_IOT_HOST,
        .port = ENNO_IOT_PORT,
        .pClientID = NULL,
        .pUserName = NULL,
        .pPassword = NULL,
        .MQTTVersion = MQTT_3_1,
        .KeepAliveInterval_Sec = 10,
        .isCleansession = true,
        .isWillMsgPresent = false,
        .will={.pTopicName = NULL, .pMessage = NULL, .isRetained = false, .qos = QOS_0},
        .mqttCommandTimeout_ms = 1000,
        .disconnectHandler = NULL
};
PublishParams PublishParamsDefault = {
        .pTopic = NULL,
        .MessageParams = {.qos = QOS_0, .isRetained = false, .isDuplicate = false, .id = 0, .pPayload = NULL, .PayloadLen = 0}
};
SubscribeParams SubscribeParamsDefault = {
        .pTopic = NULL,
        .qos = QOS_0,
//		.mHandler = NULL
};
CallbackParams CallbackParamsDefault = {
        .pTopicName = NULL,
        .TopicNameLen = 0,
        .MessageParams = {.qos = QOS_0, .isRetained=false, .isDuplicate = false, .id = 0, .pPayload = NULL, .PayloadLen = 0}
};
EnnoParams EnnoParamsDefault = {
        .Register = {.pHardwareId = "enno-default-test-id-1", .pSpecificationToken = "417b36a8-21ef-4196-a8fe-cc756f994d0b", .pOriginator = NULL},
        .Acknowledge = {.pMessage = NULL, .pOriginator = NULL},
        .Measurement = {.pName = "Temperature", .pOriginator = NULL, .value = 1, .eventDate = 0},
        .Location = {.pOriginator = NULL, .lat = 27.59, .lon = 86.5, .ele = 84.43, .eventDate = 0},
        .Alert = {.pAlertType = "enno.info", .pAlertMessage = "I am alive", .pOriginator = NULL, .eventDate = 0}
};

bool registered = false;


void echoMessageHandler(MessageData *md) {
    MQTTMessage *message = md->message;
    printf("Message arrived:\n");
    printf("%.*s\t", md->topicName->lenstring.len, md->topicName->lenstring.data);
    printf("%.*s", (int) message->payloadlen, (char *) message->payload);
}


void ennoSystemMessageHandler(char* topic, char* payload, int length) {
    Device_Header header;

    pb_istream_t stream = pb_istream_from_buffer(payload, length);

    if (pb_decode_delimited(&stream, Device_Header_fields, &header)) {

        if (header.command == Device_Command_REGISTER_ACK) {
            Device_RegistrationAck ack;
            if (pb_decode_delimited(&stream, Device_RegistrationAck_fields, &ack)) {
                if (ack.state == Device_RegistrationAckState_NEW_REGISTRATION) {
                    printf("Registered new device\n");
                    registered = true;
                }
                else if (ack.state == Device_RegistrationAckState_ALREADY_REGISTERED) {
                    printf("Device has already registered.\n");
                }
                else if (ack.state == Device_RegistrationAckState_REGISTRATION_ERROR) {
                    printf("Error rigistering device.\n");
                }
            }
        }
    } else {
        printf("Unable to decode system command.\n");
    }
}


static Error_t parseConnectParamsForError(ConnectParams *pParams) {
    Error_t rc = NONE_ERROR;
    if (
            NULL == pParams->pClientID ||
            NULL == pParams->pHostURL) {
        rc = NULL_VALUE_ERROR;
    }
    return rc;
}

static bool isPowerCycle = true;

Error_t enno_connect(ConnectParams *pParams) {

    Error_t rc = NONE_ERROR;

    rc = parseConnectParamsForError(pParams);

    if (NONE_ERROR == rc) {
        NewNetwork(&n);
        rc = ConnectNetwork(&n, pParams->pHostURL, pParams->port);
        //printf("Connecting to %s %d\n", pParams->pHostURL, pParams->port);

        if (NONE_ERROR == rc) {
            if (pParams->isCleansession || isPowerCycle) {
                MQTTClient(&c, &n, (unsigned int) (pParams->mqttCommandTimeout_ms), writebuf, ENNO_IOT_TX_BUF_LEN,
                           readbuf, ENNO_IOT_RX_BUF_LEN);
                isPowerCycle = false;
            }
            MQTTPacket_connectData data = MQTTPacket_connectData_initializer;

            data.willFlag = 0;
            data.MQTTVersion = 4;
            data.clientID.cstring = pParams->pClientID;
            data.username.cstring = pParams->pUserName;
            data.password.cstring = pParams->pPassword;

            data.will.topicName.cstring = (char *) pParams->will.pTopicName;
            data.will.message.cstring = (char *) pParams->will.pMessage;
            data.will.qos = pParams->will.qos;
            data.will.retained = pParams->will.isRetained;

            data.keepAliveInterval = pParams->KeepAliveInterval_Sec;
            data.cleansession = pParams->isCleansession;

            if (0 != MQTTConnect(&c, &data)) {
                rc = CONNECTION_ERROR;
            }
        }
    }
    return rc;
}


void (* callbackRef)(char*, char*, int);
void  callbackWrapper(MessageData *md) {
    MQTTMessage *message = md->message;
    callbackRef(md->topicName->cstring, message->payload, message->payloadlen);
}
Error_t enno_subscribe(char *pTopic, void *callback, SubscribeParams *pParams) {
    Error_t rc = NONE_ERROR;

    callbackRef = callback;
    if (0 != MQTTSubscribe(&c, pTopic, (enum QoS) pParams->qos, callbackWrapper)) {
        rc = SUBSCRIBE_ERROR;
    }
    return rc;
}

Error_t enno_publish(char path, EnnoParams *pParams_enno, PublishParams *pParams) {
    Error_t rc = NONE_ERROR;
    unsigned int len = 0;
    unsigned char buffer[512];

    switch (path) {
        case 'R':
            len = sw_register(pParams_enno->Register.pHardwareId, pParams_enno->Register.pSpecificationToken, buffer,
                              300, pParams_enno->Register.pOriginator);
            break;
        case 'A':
            len = sw_alert(pParams_enno->Register.pHardwareId, pParams_enno->Alert.pAlertType,
                           pParams_enno->Alert.pAlertMessage, pParams_enno->Alert.eventDate, buffer, 300,
                           pParams_enno->Alert.pOriginator);
            break;
        case 'M':
            len = sw_measurement(pParams_enno->Register.pHardwareId, pParams_enno->Measurement.pName,
                                 pParams_enno->Measurement.value, pParams_enno->Measurement.eventDate, buffer, 300,
                                 pParams_enno->Measurement.pOriginator);
            break;
        case 'L':
            len = sw_location(pParams_enno->Register.pHardwareId, pParams_enno->Location.lat,
                              pParams_enno->Location.lon, pParams_enno->Location.ele, pParams_enno->Location.eventDate,
                              buffer, 300, pParams_enno->Location.pOriginator);
            break;
        default:
            printf("WARN:Nothing published.\n");
            break;
    }

    MQTTMessage Message;
    Message.dup = pParams->MessageParams.isDuplicate;
    Message.id = pParams->MessageParams.id;
    Message.qos = (enum QoS) pParams->MessageParams.qos;
    Message.retained = pParams->MessageParams.isRetained;
    Message.payload = buffer;
    Message.payloadlen = len;
    if (0 != MQTTPublish(&c, pParams->pTopic, &Message)) {
        rc = PUBLISH_ERROR;
    }

    return rc;
}


Error_t enno_unsubscribe(char *pTopic) {
    Error_t rc = NONE_ERROR;

    if (0 != MQTTUnsubscribe(&c, pTopic)) {
        rc = UNSUBSCRIBE_ERROR;
    }
    return rc;

}

Error_t enno_disconnect() {
    Error_t rc = NONE_ERROR;
    if (0 != MQTTDisconnect(&c)) {
        rc = DISCONNECT_ERROR;
    }
    n.disconnect(&n);
    //linux_disconnect(&n);
    return rc;
}

Error_t enno_loop(int timeout) {
    Error_t rc = NONE_ERROR;
    if (0 != MQTTYield(&c, timeout)) {
        rc = YIELD_ERROR;
    }
    return rc;
}







