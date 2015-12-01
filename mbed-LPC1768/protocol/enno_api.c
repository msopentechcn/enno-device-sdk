/*
 * enno_api.c
 *
 *  Created on: 2015Äê11ÔÂ20ÈÕ
 *      Author: v-chawei
 */
#include <stdio.h>
#include <stdbool.h>
#include "enno_api.h"
#include "sitewhere.pb.h"
#include "sitewhere.h"
#include "pb_decode.h"
//#include "custom.h"
//#include "raspberrypi.pb.h"

bool registered = false;

void ennoSystemMessageHandler(char* topic, char* payload, int length) {
    Device_Header header;

    pb_istream_t stream = pb_istream_from_buffer(payload, length);

    if (pb_decode_delimited(&stream, Device_Header_fields, &header)) {

        if (header.command == Device_Command_REGISTER_ACK) {
            Device_RegistrationAck ack;
            if (pb_decode_delimited(&stream, Device_RegistrationAck_fields, &ack)) {
                if (ack.state == Device_RegistrationAckState_NEW_REGISTRATION) {
                    printf("Registered new device.\n");
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

