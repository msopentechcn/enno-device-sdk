#ifndef _ENNO_API_H
#define _ENNO_API_H
//
// Created by root on 11/10/15.
//
#include "stddef.h"
#include "stdbool.h"
#include "stdint.h"
#include "enno_error.h"
#include "enno_config.h"



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
    char messageType;
    Register_enno Register;
    Acknowledge_enno Acknowledge;
    Measurement_enno Measurement;
    Location_enno Location;
    Alert_enno Alert;
} EnnoMessages;
extern EnnoMessages EnnoMessagesDefault;


/**
 * @brief Handles the system callback message
 *
 * params:  topic   the topic subscribed by client
 *          payload the received payload
 *          length  the length of payload
 *
 */
void ennoSystemMessageHandler(char* topic, char* payload, int length);

/**
 * @brief Connection Function
 *
 * Called to establish an connection with the Enno IoT Service
 *
 * @param:  HostURL the website address to connect
 *          port    listening port
 *          appKey  a key for authentication
 * @return An IoT Error Type defining successful/failed connection
 */

Error_t enno_connect(char *HostURL, uint16_t port, char *appKey);

/**
 * @brief Subscribe to a topic.
 *
 * Called to send a subscribe message to the broker
 *
 * @param:  Topic       Pointer to a Topic
 *          callback    Pointer to a callback function
 * @return An IoT Error Type defining successful/failed subscription
 */
Error_t enno_subscribe(char *topic, void *callback);
/**
 * @brief Publish a message to a path
 *
 * Called to publish a message to a path.
 *
 * @param:	path           the destination of message sent to
 *          messages_enno  Pointer to the published message
 * @return An IoT Error Type defining successful/failed publish
 */

//Error_t enno_publish(char path, EnnoParams *Params_enno, PublishParams *pParams);
Error_t enno_publish(char *path, EnnoMessages *messages_enno);
/**
 * @brief Unsubscribe to an topic.
 *
 * Called to send an usubscribe message to the broker requesting removal of a subscription
 * to a topic.
 *
 * @param:  pTopic      Pointer to the requested topic string. Ensure the string is null terminated
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
 * Called to yield the current thread to the underlying  client.
 *
 * @param timeout Maximum number of milliseconds to pass thread execution to the client.
 * @return An IoT Error Type defining successful/failed client processing.
 */

Error_t enno_loop(int timeout);

#endif //ENNO_DEVICE_SDK_EMBEDDED_C_ENNO_API_H
