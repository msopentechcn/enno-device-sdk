#include <stdio.h>
#include <stdbool.h>
#include "enno_api.h"
#include "sitewhere.pb.h"
#include "sitewhere.h"
#include "pb_decode.h"
#include "custom.h"
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

/** Handle the 'ping' command */
static void handlePing(ArduinoCustom_ping ping, char* originator) {
    unsigned int len = 0;
    printf("Ping received.\n");
    /*
    if (len = sw_acknowledge(hardwareId, "Ping received.", buffer, sizeof(buffer), originator)) {
        mqttClient.publish(outbound, buffer, len);
    }*/
}

/** Handle the 'testEvents' command */

static void handleTestEvents(ArduinoCustom_testEvents testEvents, char* originator) {
    unsigned int len = 0;
    /*
    if (len = sw_location(hardwareId, 33.755f, -84.39f, 0.0f, NULL, buffer, sizeof(buffer), originator)) {
        mqttClient.publish(outbound, buffer, len);
    }
    if (len = sw_measurement(hardwareId, "engine.temp", 25.1f, NULL, buffer, sizeof(buffer), originator)) {
        mqttClient.publish(outbound, buffer, len);
    }
    if (len = sw_alert(hardwareId, "engine.overheat", "The engine is overheating!", NULL, buffer, sizeof(buffer), originator)) {
        mqttClient.publish(outbound, buffer, len);
    }*/
}

/** Handle the 'serialPrintln' command */
static void handleSerialPrintln(ArduinoCustom_serialPrintln serialPrintln, char* originator) {
    unsigned int len = 0;
    /*
    Serial.println(serialPrintln.message);
    if (len = sw_acknowledge(hardwareId, "Message sent to Serial.println().", buffer, sizeof(buffer), originator)) {
        mqttClient.publish(outbound, buffer, len);
    }*/
}

void ennoCommandMessageHandler(char* topic, char* payload, int length){
    ArduinoCustom__Header header;
    pb_istream_t stream = pb_istream_from_buffer(payload, length);
    if (pb_decode_delimited(&stream, ArduinoCustom__Header_fields, &header)) {
        printf("Decoded header for custom command.\n");
        if (header.command == ArduinoCustom_Command_PING) {
            ArduinoCustom_ping ping;
            if (pb_decode_delimited(&stream, ArduinoCustom_ping_fields, &ping)) {
                handlePing(ping, header.originator);
            }
        } else if (header.command == ArduinoCustom_Command_TESTEVENTS) {
            ArduinoCustom_testEvents testEvents;
            if (pb_decode_delimited(&stream, ArduinoCustom_testEvents_fields, &testEvents)) {
                handleTestEvents(testEvents, header.originator);
            }
        } else if (header.command == ArduinoCustom_Command_SERIALPRINTLN) {
            ArduinoCustom_serialPrintln serialPrintln;
            if (pb_decode_delimited(&stream, ArduinoCustom_serialPrintln_fields, &serialPrintln)) {
                handleSerialPrintln(serialPrintln, header.originator);
            }
        }
    }

}