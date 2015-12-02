/*
 * enno_api.h
 *
 *  Created on: 2015年11月20日
 *      Author: v-chawei
 */
#ifndef PROTOCOL_ENNO_API_H_
#define PROTOCOL_ENNO_API_H_

#if defined(__cplusplus)
extern "C" {
#endif

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

#define EnnoMessagesDefault {'R',{"enno-mbed-test-id-1", "9556b504-52cc-4194-89c0-f301a782b286", NULL},{"enno-mbed-test-id-1", NULL, NULL},{"enno-mbed-test-id-1", "temperature", NULL, 10, 0},{"enno-mbed-test-id-1", NULL, 27.59, 86.55, 8844.43, 0},{"enno-mbed-test-id-1", "enno.info", "I am alive", NULL, 0}}
/**
 * @brief Handles the system callback message
 *
 * params:  topic   the system topic subscribed by client
 *          payload the received payload
 *          length  the length of payload
 *
 */
void ennoSystemMessageHandler(char* topic, char* payload, int length);
/**
 * @brief Handles the command callback message
 *
 * params:  topic   the command topic subscribed by client
 *          payload the received payload
 *          length  the length of payload
 *
 */
void ennoCommandMessageHandler(char* topic, char* payload, int length);

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
Error_t enno_subscribe(char *topic, char *topicType);
/**
 * @brief Publish a message to a path
 *
 * Called to publish a message to a path.
 *
 * @param:  path           the destination of message sent to
 *          messages_enno  Pointer to the published message
 * @return An IoT Error Type defining successful/failed publish
 */
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
#ifdef __cplusplus
}
#endif

#endif /* PROTOCOL_ENNO_API_H_ */
