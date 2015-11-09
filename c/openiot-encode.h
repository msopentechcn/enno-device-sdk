/**
 * openiot-encode.h - Library for interacting with OpenIoT server using different serialization/encoding protocols.
 */
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"
#include <stdint.h>

#ifndef OPENIOT_ENCODE_H
#define OPENIOT_ENCODE_H

#ifdef __cplusplus
extern "C" {
#endif

/** Create an encoded registration message for sending to OpenIoT server */
unsigned int openiot_register(char *hardwareId, char *specificationToken, uint8_t *buffer, size_t length,
                              char *originator);

/** Create an encoded acknowledgement message for sending to OpenIoT server */
unsigned int openiot_acknowledge(char *hardwareId, char *message, uint8_t *buffer, size_t length, char *originator);

/** Create an encoded measurement message for sending to OpenIoT server */
unsigned int openiot_measurement(char *hardwareId, char *name, char *value, int64_t eventDate,
                                 uint8_t *buffer, size_t length, char *originator);

/** Support for multiple measurement metrics */
unsigned int openiot_measurement_multi(char *hardwareId, char **measurementTokens, int64_t eventDate,
                                       uint8_t *buffer, size_t length, char *originator);

/** Create an encoded location message for sending to OpenIoT server */
unsigned int openiot_location(char *hardwareId, char *lat, char *lon, char *elevation, int64_t eventDate,
                              uint8_t *buffer, size_t length, char *originator);

/** Create an encoded alert message for sending to OpenIoT server */
unsigned int openiot_alert(char *hardwareId, char *type, char *message, int64_t eventDate,
                           uint8_t *buffer, size_t length, char *originator);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
