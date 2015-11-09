#include <protocolbuffer/openiot.pb.h>
#include <protocolbuffer/pb_encode.h>
#include "openiot-encode.h"


// Signals end of stream.
uint8_t zero = 0;

unsigned int openiot_register(char *hardwareId, char *specificationToken, uint8_t *buffer, size_t length,
                              char *originator) {
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, length);

    OpenIoT_Header header = {};
    header.command = OpenIoT_Command_REGISTER;
    if (originator != NULL) {
        header.has_originator = true;
        strcpy(header.originator, originator);
    }
    if (!pb_encode_delimited(&stream, OpenIoT_Header_fields, &header)) {
        return 0;
    }

    OpenIoT_RegisterDevice registerDevice = {};
    strcpy(registerDevice.hardwareId, hardwareId);
    strcpy(registerDevice.specificationToken, specificationToken);
    if (!pb_encode_delimited(&stream, OpenIoT_RegisterDevice_fields, &registerDevice)) {
        return 0;
    }

    return stream.bytes_written;
}

unsigned int openiot_acknowledge(char *hardwareId, char *message, uint8_t *buffer, size_t length, char *originator) {
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, length);

    OpenIoT_Header header = {};
    header.command = OpenIoT_Command_ACKNOWLEDGE;
    if (originator != NULL) {
        header.has_originator = true;
        strcpy(header.originator, originator);
    }
    if (!pb_encode_delimited(&stream, OpenIoT_Header_fields, &header)) {
        return 0;
    }

    OpenIoT_Acknowledge ack = {};
    strcpy(ack.hardwareId, hardwareId);
    if (message != NULL) {
        ack.has_message = true;
        strcpy(ack.message, message);
    }
    if (!pb_encode_delimited(&stream, OpenIoT_Acknowledge_fields, &ack)) {
        return 0;
    }

    return stream.bytes_written;
}

unsigned int openiot_measurement(char *hardwareId, char *name, char *value, int64_t eventDate,
                                 uint8_t *buffer, size_t length, char *originator) {
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, length);

    OpenIoT_Header header = {};
    header.command = OpenIoT_Command_DEVICEMEASUREMENT;
    if (originator != NULL) {
        header.has_originator = true;
        strcpy(header.originator, originator);
    }
    if (!pb_encode_delimited(&stream, OpenIoT_Header_fields, &header)) {
        return 0;
    }


    OpenIoT_DeviceMeasurements measurements = {};
    strcpy(measurements.hardwareId, hardwareId);

    OpenIoT_Measurement measurement = {};
    strcpy(measurement.measurementId, name);
    //measurement.measurementValue = float_to_double(value);
    //measurement.measurementValue = value;
    strcpy(measurement.measurementValue, value);
    measurements.measurement[0] = measurement;
    measurements.measurement_count = 1;

    if (eventDate != 0) {
        measurements.has_eventDate = true;
        measurements.eventDate = eventDate;
    }
    if (!pb_encode_delimited(&stream, OpenIoT_DeviceMeasurements_fields, &measurements)) {
        return 0;
    }

    return stream.bytes_written;
}

unsigned int openiot_measurement_multi(char *hardwareId, char **measurementTokens, int64_t eventDate,
                                       uint8_t *buffer, size_t length, char *originator) {
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, length);

    OpenIoT_Header header = {};
    header.command = OpenIoT_Command_DEVICEMEASUREMENT;
    if (originator != NULL) {
        header.has_originator = true;
        strcpy(header.originator, originator);
    }
    if (!pb_encode_delimited(&stream, OpenIoT_Header_fields, &header)) {
        return 0;
    }

    OpenIoT_DeviceMeasurements measurements = {};
    strcpy(measurements.hardwareId, hardwareId);

    measurements.measurement_count = 0;
    if (measurementTokens) {
        int i;
        for (i = 0; *(measurementTokens + i); i++) {
            OpenIoT_Measurement measurement = {};
            char *key = strtok(*(measurementTokens + i), ":");
            //float value = atof(strtok(NULL, ":"));
            char *value = strtok(NULL, ":");
            //printf("value is %lf\n", value);
            strcpy(measurement.measurementId, key);
            //measurement.measurementValue = float_to_double(value);
            //measurement.measurementValue = value;
            strcpy(measurement.measurementValue, value);
            measurements.measurement[i] = measurement;
            measurements.measurement_count++;
            free(*(measurementTokens + i));
        }
        free(measurementTokens);
    }

    if (eventDate != 0) {
        measurements.has_eventDate = true;
        measurements.eventDate = eventDate;
    }
    if (!pb_encode_delimited(&stream, OpenIoT_DeviceMeasurements_fields, &measurements)) {
        return 0;
    }

    return stream.bytes_written;
}


unsigned int openiot_location(char *hardwareId, char *lat, char *lon, char *ele, int64_t eventDate,
                              uint8_t *buffer, size_t length, char *originator) {
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, length);

    OpenIoT_Header header = {};
    header.command = OpenIoT_Command_DEVICELOCATION;
    if (originator != NULL) {
        header.has_originator = true;
        strcpy(header.originator, originator);
    }
    if (!pb_encode_delimited(&stream, OpenIoT_Header_fields, &header)) {
        return 0;
    }

    OpenIoT_DeviceLocation location = {};
    strcpy(location.hardwareId, hardwareId);

    strcpy(location.latitude, lat);
    strcpy(location.longitude, lon);
    strcpy(location.elevation, ele);
    location.has_elevation = true;

    if (eventDate != 0) {
        location.has_eventDate = true;
        location.eventDate = eventDate;
    }
    if (!pb_encode_delimited(&stream, OpenIoT_DeviceLocation_fields, &location)) {
        return 0;
    }

    return stream.bytes_written;
}

unsigned int openiot_alert(char *hardwareId, char *alertType, char *alertMessage, int64_t eventDate,
                           uint8_t *buffer, size_t length, char *originator) {
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, length);

    OpenIoT_Header header = {};
    header.command = OpenIoT_Command_DEVICEALERT;
    if (originator != NULL) {
        header.has_originator = true;
        strcpy(header.originator, originator);
    }
    if (!pb_encode_delimited(&stream, OpenIoT_Header_fields, &header)) {
        return 0;
    }

    OpenIoT_DeviceAlert alert = {};
    strcpy(alert.hardwareId, hardwareId);
    strcpy(alert.alertType, alertType);
    strcpy(alert.alertMessage, alertMessage);
    if (eventDate != 0) {
        alert.has_eventDate = true;
        alert.eventDate = eventDate;
    }
    if (!pb_encode_delimited(&stream, OpenIoT_DeviceAlert_fields, &alert)) {
        return 0;
    }

    return stream.bytes_written;
}
