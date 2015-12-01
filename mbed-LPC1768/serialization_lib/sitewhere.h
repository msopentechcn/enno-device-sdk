/**
 * sitewhere.h - Library for interacting with SiteWhere using Google Protocol Buffers.
 * See https://developers.google.com/protocol-buffers/ for details on protocol buffers.
 * The lightweight C implementation was generated with nanopb (http://code.google.com/p/nanopb/).
 *
 * Copyright 2013-2014 Reveal Technologies LLC.
 */
#ifndef _SITEWHERE_H
#define _SITEWHERE_H

//#include "Arduino.h"
#include "sitewhere.pb.h"
#include "pb_encode.h"

#include <string.h>
#include <stdint.h>
#include <stddef.h>
//#include "string.h"						  //warning!
//#include "stdint.h"                     	  //warning!
//#include "stddef.h"						  //warning!


#ifdef __cplusplus
extern "C" {
#endif

/** Create an encoded registration message for sending to SiteWhere */
unsigned int sw_register(char* hardwareId, char* specificationToken, uint8_t* buffer, size_t length,
		char* originator);

/** Create an encoded acknowledgement message for sending to SiteWhere */
unsigned int sw_acknowledge(char* hardwareId, char* message, uint8_t* buffer, size_t length, char* originator);

/** Create an encoded measurement message for sending to SiteWhere */
unsigned int sw_measurement(char* hardwareId, char* name, float value, int64_t eventDate,
		uint8_t* buffer, size_t length, char* originator);

/** Create an encoded location message for sending to SiteWhere */
unsigned int sw_location(char* hardwareId, float lat, float lon, float elevation, int64_t eventDate,
		uint8_t* buffer, size_t length, char* originator);

/** Create an encoded alert message for sending to SiteWhere */
unsigned int sw_alert(char* hardwareId, char* type, char* message, int64_t eventDate,
		uint8_t* buffer, size_t length, char* originator);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
