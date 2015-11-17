#include <MQTTPacket.h>
#include <enno_api.h>
#include "sitewhere.h"
#include "MQTTClient.h"


static unsigned char writebuf[ENNO_IOT_TX_BUF_LEN];
static unsigned char readbuf[ENNO_IOT_RX_BUF_LEN];

static Network n;
static Client c;

//static iot_disconnect_handler clientDisconnectHandler;    //warning defined but not used

/**
 * @brief Communication Protocol Type
 *
 * Defining an MQTT version Type
 *
 */
typedef enum {
    MQTT_3_1 = 3,               // MQTT 3.1   (protocol_lib message byte = 3)
    MQTT_3_1_1 = 4              // MQTT 3.1.1 (protocol_lib message byte = 4)
} MQTT_Ver_t;

/**
 * @brief Quality of Service Type
 *
 * Defining a QoS type.
 * @note QoS 2 is \b NOT supported by the Enno IoT Service at the time of this SDK release.  //?wzy
 *
 */
typedef enum {
    QOS_0,                      // QoS 0 = at most once delivery
    QOS_1,                      // QoS 1 = at least once delivery
    QOS_2                       // QoS 2 is NOT supported
} QoSLevel;

/**
 * @brief Last Will and Testament Definition
 *
 * Defining a type for LWT parameters.
 * @note Retained messages are \b NOT supported by the Enno IoT Service at the time of this SDK release.  //?
 *
 */
typedef struct {
    const char *pTopicName;     // LWT Topic
    const char *pMessage;       // Message to be delivered as LWT
    bool isRetained;            // NOT supported
    QoSLevel qos;               // QoS of LWT message
} MQTTWillOptions;

/**
 * @brief MQTT Message Parameters
 *
 * Defines a type for properties of MQTT messages including topic, payload an QoS.
 *
 */
typedef struct {
    QoSLevel qos;               // Message Quality of Service
    bool isRetained;            // Retained messages are \b NOT supported by the Enno IoT Service at the time of this SDK release.
    bool isDuplicate;           // Is this message a duplicate QoS > 0 message?  Handled automatically by the MQTT client.
    uint16_t id;                // Message sequence identifier.  Handled automatically by the MQTT client.
    void *pPayload;             // Pointer to MQTT message payload (bytes).
    uint32_t PayloadLen;        // Length of MQTT payload.
} MQTTMessageParams;

typedef void (*iot_disconnect_handler)(void);

typedef struct {
    char *pClientID;                                 // Pointer to a string defining the MQTT client ID (this needs to be unique \b per \b device )
    char *pUserName;                                 // Not used in the Enno IoT Service
    char *pPassword;                                 // Not used in the Enno IoT Service
    MQTT_Ver_t MQTTVersion;                          // Desired MQTT version used during connection
    uint16_t KeepAliveInterval_Sec;                  // MQTT keep alive interval in seconds.  Defines inactivity time allowed before determining the connection has been lost.
    bool isCleansession;                             // MQTT clean session.  True = this session is to be treated as clean.  Previous server state is cleared and no stated is retained from this connection.
    bool isWillMsgPresent;                           // Is there a LWT associated with this connection?
    MQTTWillOptions will;                            // MQTT LWT parameters.
    uint32_t mqttCommandTimeout_ms;                  // Timeout for MQTT blocking calls.  In milliseconds.
    iot_disconnect_handler disconnectHandler;        // Callback to be invoked upon connection loss.
} ConnectParams;

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
        .pClientID = NULL,
        .pUserName = NULL,
        .pPassword = NULL,
        .MQTTVersion = MQTT_3_1,
        .KeepAliveInterval_Sec = 10,
        .isCleansession = true,
        .isWillMsgPresent = false,
        .will={.pTopicName = NULL, .pMessage = NULL, .isRetained = false, .qos = QOS_0},
        .mqttCommandTimeout_ms = 2000,
        .disconnectHandler = NULL
};




void echoMessageHandler(MessageData *md) {
    MQTTMessage *message = md->message;
    printf("Message arrived:\n");
    printf("%.*s\t", md->topicName->lenstring.len, md->topicName->lenstring.data);
    printf("%.*s", (int) message->payloadlen, (char *) message->payload);
}


static Error_t parseConnectParamsForError(char *hostURL) {
    Error_t rc = NONE_ERROR;
    if (
            NULL == hostURL) {
        rc = NULL_VALUE_ERROR;
        printf("ERROR: hostURL can not be empty!\n");
    }
    return rc;
}

static bool isPowerCycle = true;

Error_t enno_connect(char *hostURL, uint16_t port, char *appKey){

    Error_t rc = NONE_ERROR;
    rc = parseConnectParamsForError(hostURL);

    if (NONE_ERROR == rc) {
        NewNetwork(&n);
        rc = ConnectNetwork(&n, hostURL, port);

        if (NONE_ERROR == rc) {
            if (ConnectParamsDefault.isCleansession || isPowerCycle){
                MQTTClient(&c, &n, (unsigned int) (ConnectParamsDefault.mqttCommandTimeout_ms), writebuf, ENNO_IOT_TX_BUF_LEN,
                            readbuf, ENNO_IOT_RX_BUF_LEN);
                isPowerCycle = false;
            }
            MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
            data.willFlag = 0;
            data.MQTTVersion = 4;
            data.clientID.cstring = ConnectParamsDefault.pClientID;
            data.username.cstring = ConnectParamsDefault.pUserName;
            data.password.cstring = ConnectParamsDefault.pPassword;

            data.will.topicName.cstring = (char *) ConnectParamsDefault.will.pTopicName;
            data.will.message.cstring = (char *) ConnectParamsDefault.will.pMessage;
            data.will.qos = ConnectParamsDefault.will.qos;
            data.will.retained = ConnectParamsDefault.will.isRetained;

            data.keepAliveInterval = ConnectParamsDefault.KeepAliveInterval_Sec;
            data.cleansession = ConnectParamsDefault.isCleansession;

            if (0 != MQTTConnect(&c, &data)){
                rc = NONE_ERROR;
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

Error_t enno_subscribe(char *topic, void *callback){
    Error_t rc = NONE_ERROR;

    callbackRef = callback;
    if (0 != MQTTSubscribe(&c, topic, (enum QoS) QOS_0, callbackWrapper)){
        rc = SUBSCRIBE_ERROR;
    }
    return rc;
}

Error_t enno_publish(char *path, EnnoMessages *messages_enno){
    Error_t rc = NONE_ERROR;
    unsigned int len = 0;
    unsigned char buffer[512];

    switch (messages_enno->messageType) {
        case 'R':
            len = sw_register(messages_enno->Register.pHardwareId, messages_enno->Register.pSpecificationToken, buffer,
                              512, messages_enno->Register.pOriginator);
            break;
        case 'A':
            len = sw_alert(messages_enno->Register.pHardwareId, messages_enno->Alert.pAlertType,
                           messages_enno->Alert.pAlertMessage, messages_enno->Alert.eventDate, buffer, 512,
                           messages_enno->Alert.pOriginator);
            break;
        case 'M':
            len = sw_measurement(messages_enno->Register.pHardwareId, messages_enno->Measurement.pName,
                                 messages_enno->Measurement.value, messages_enno->Measurement.eventDate, buffer, 512,
                                 messages_enno->Measurement.pOriginator);
            break;
        case 'L':
            len = sw_location(messages_enno->Register.pHardwareId, messages_enno->Location.lat,
                              messages_enno->Location.lon, messages_enno->Location.ele, messages_enno->Location.eventDate,
                              buffer, 512, messages_enno->Location.pOriginator);
            break;
        default:
            printf("WARN:Nothing published.\n");
            break;
    }

    MQTTMessage Message;
    Message.dup = MQTTMessageParamsDefault.isDuplicate;
    Message.id = MQTTMessageParamsDefault.id;
    Message.qos = (enum QoS) MQTTMessageParamsDefault.qos;
    Message.retained = MQTTMessageParamsDefault.isRetained;
    Message.payload = buffer;
    Message.payloadlen = len;


    if (0 != MQTTPublish(&c, path, &Message)) {
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







