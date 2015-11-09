/**
 * @file enno_protocol_interface.h
 * @brief Interface definition for client.
 */

#ifndef _ENNO_PROTOCOL_INTERFACE_H
#define _ENNO_PROTOCOL_INTERFACE_H

#include "stddef.h"
#include "stdbool.h"
#include "stdint.h"
#include "enno_error.h"
#include "enno_config.h"

///////////////////// todo: MQTT related data structure(should all be moved out of enno header) ///////////////////

#include "MQTTClient.h"
static unsigned char writebuf[ENNO_IOT_TX_BUF_LEN];
static unsigned char readbuf[ENNO_IOT_RX_BUF_LEN];

/**
 * @brief Communication Protocol Type
 *
 * Defining an MQTT version Type
 *
 */
typedef enum {
    MQTT_3_1 = 3,    ///< MQTT 3.1   (protocol_lib message byte = 3)
    MQTT_3_1_1 = 4    ///< MQTT 3.1.1 (protocol_lib message byte = 4)
} MQTT_Ver_t;

/**
 * @brief Quality of Service Type
 *
 * Defining a QoS type.
 * @note QoS 2 is \b NOT supported by the Enno IoT Service at the time of this SDK release.  //?wzy
 *
 */
typedef enum {
    QOS_0,    // QoS 0 = at most once delivery
    QOS_1,    //QoS 1 = at least once delivery
    QOS_2    //QoS 2 is NOT supported
} QoSLevel;

/**
 * @brief Last Will and Testament Definition
 *
 * Defining a type for LWT parameters.
 * @note Retained messages are \b NOT supported by the Enno IoT Service at the time of this SDK release.  //?wzy
 *
 */
typedef struct {
    const char *pTopicName;    ///< LWT Topic
    const char *pMessage;    ///< Message to be delivered as LWT
    bool isRetained;        ///< NOT supported
    QoSLevel qos;            ///< QoS of LWT message
} MQTTWillOptions;
extern MQTTWillOptions MQTTWillOptionsDefault;

/**
 * @brief MQTT Message Parameters
 *
 * Defines a type for properties of MQTT messages including topic, payload an QoS.
 *
 */
typedef struct {
    QoSLevel qos;                ///< Message Quality of Service
    bool isRetained;            ///< Retained messages are \b NOT supported by the Enno IoT Service at the time of this SDK release.
    bool isDuplicate;            ///< Is this message a duplicate QoS > 0 message?  Handled automatically by the MQTT client.
    uint16_t id;                ///< Message sequence identifier.  Handled automatically by the MQTT client.
    void *pPayload;                ///< Pointer to MQTT message payload (bytes).
    uint32_t PayloadLen;        ///< Length of MQTT payload.
} MQTTMessageParams;
extern MQTTMessageParams MQTTMessageParamsDefault;


typedef void (*iot_disconnect_handler)(void);
typedef struct {
    char *pHostURL;                                    ///< Pointer to a string defining the endpoint for the MQTT service
    uint16_t port;                                    ///< MQTT service listening port
    char *pClientID;                                ///< Pointer to a string defining the MQTT client ID (this needs to be unique \b per \b device )
    char *pUserName;                                ///< Not used in the Enno IoT Service
    char *pPassword;                                ///< Not used in the Enno IoT Service
    MQTT_Ver_t MQTTVersion;                            ///< Desired MQTT version used during connection
    uint16_t KeepAliveInterval_Sec;                    ///< MQTT keep alive interval in seconds.  Defines inactivity time allowed before determining the connection has been lost.
    bool isCleansession;                            ///< MQTT clean session.  True = this session is to be treated as clean.  Previous server state is cleared and no stated is retained from this connection.
    bool isWillMsgPresent;                            ///< Is there a LWT associated with this connection?
    MQTTWillOptions will;                            ///< MQTT LWT parameters.
    uint32_t mqttCommandTimeout_ms;                    ///< Timeout for MQTT blocking calls.  In milliseconds.
    iot_disconnect_handler disconnectHandler;        ///< Callback to be invoked upon connection loss.

} ConnectParams;
extern ConnectParams ConnectParamsDefault;


typedef struct {
    char *pTopic;                        ///< Pointer to the string defining the desired publishing topic.
    MQTTMessageParams MessageParams;    ///< Parameters defining the message to be published.

} PublishParams;
extern PublishParams PublishParamsDefault;


typedef struct {
    char *pTopic;                    ///< Pointer to the string defining the desired subscription topic.
    QoSLevel qos;                    ///< Quality of service of the subscription.
//	iot_message_handler mHandler;	///< Callback to be invoked upon receipt of a message on the subscribed topic.
} SubscribeParams;
extern SubscribeParams SubscribeParamsDefault;


typedef struct {
    char *pTopicName;
    uint16_t TopicNameLen;
    MQTTMessageParams MessageParams;
} CallbackParams;
extern CallbackParams CallbackParamsDefault;





//////////////////////////////////////// End of MQTT related data structure //////////////////////////////////////


typedef struct {
    char *pHardwareId;
    char *pSpecificationToken;
    char *pOriginator;
} Register_enno;

typedef struct {
    char *pHardwareId;
    char *pMessage;
    char *pOriginator;
} Acknowledge_enno;

typedef struct {
    char *pHardwareId;
    char *pName;
    char *pOriginator;
    float value;
    int64_t eventDate;
} Measurement_enno;

typedef struct {
    char *pHardwareId;
    char *pOriginator;
    float lat;
    float lon;
    float ele;
    int64_t eventDate;
} Location_enno;

typedef struct {
    char *pHardwareId;
    char *pAlertType;
    char *pAlertMessage;
    char *pOriginator;
    int64_t eventDate;
} Alert_enno;

typedef struct {
    Register_enno Register;
    Acknowledge_enno Acknowledge;
    Measurement_enno Measurement;
    Location_enno Location;
    Alert_enno Alert;
} EnnoParams;
extern EnnoParams EnnoParamsDefault;



void ennoSystemMessageHandler(char* topic, char* payload, int length);

/**
 * @brief Connection Function
 *
 * Called to establish an connection with the Enno IoT Service
 *
 * @param pParams	Pointer to connection parameters
 * @return An IoT Error Type defining successful/failed connection
 */
Error_t enno_connect(ConnectParams *pParams);

/**
 * @brief Subscribe to a topic.
 *
 * Called to send a subscribe message to the broker requesting a subscription
 * to a topic.
 * @note Call is blocking.  The call returns after the receipt of the SUBACK control packet.
 *
 * @param pParams	Pointer to subscribe parameters
 * @return An IoT Error Type defining successful/failed subscription
 */
Error_t enno_subscribe(char *pTopic, void *callback, SubscribeParams *pParams);

/**
 * @brief Publish a message on a topic
 *
 * Called to publish a message on a topic.
 * @note Call is blocking.  In the case of a QoS 0 message the function returns
 * after the message was successfully passed to the TLS layer.  In the case of QoS 1
 * the function returns after the receipt of the PUBACK control packet.
 *
 * @param pParams	Pointer to publish parameters
 * @return An IoT Error Type defining successful/failed publish
 */

Error_t enno_publish(char path, EnnoParams *Params_enno, PublishParams *pParams);

/**
 * @brief Unsubscribe to an topic.
 *
 * Called to send an usubscribe message to the broker requesting removal of a subscription
 * to a topic.
 * @note Call is blocking.  The call returns after the receipt of the UNSUBACK control packet.
 *
 * @param pTopic Pointer to the requested topic string. Ensure the string is null terminated
 * @return An IoT Error Type defining successful/failed unsubscription
 */
Error_t enno_unsubscribe(char *pTopic);

/**
 * @brief Disconnect a Connection
 *
 * Called to send a disconnect message to the broker.
 *
 * @return An IoT Error Type defining successful/failed send of the disconnect control packet.
 */
Error_t enno_disconnect();

/**
 * @brief Yield to the  client
 *
 * Called to yield the current thread to the underlying MQTT client.  This time is used by
 * the MQTT client to manage PING requests to monitor the health of the TCP connection as
 * well as periodically check the socket receive buffer for subscribe messages.  Yield()
 * must be called at a rate faster than the keepalive interval.  It must also be called
 * at a rate faster than the incoming message rate as this is the only way the client receives
 * processing time to manage incoming messages.
 *
 * @param timeout Maximum number of milliseconds to pass thread execution to the client.
 * @return An IoT Error Type defining successful/failed client processing.
 *         If this call results in an error it is likely the MQTT connection has dropped.
 *         iot_is_mqtt_connected can be called to confirm.
 */

Error_t enno_loop(int timeout);


#endif



