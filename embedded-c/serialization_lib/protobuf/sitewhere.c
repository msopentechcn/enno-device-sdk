#include "sitewhere.h"

#include "double_conversion.h"

// Signals end of stream.
uint8_t zero = 0;

unsigned int sw_register(char* hardwareId, char* specificationToken, uint8_t* buffer, size_t length, char* originator) {
	pb_ostream_t stream = pb_ostream_from_buffer(buffer, length);

	SiteWhere_Header header = { };
	header.command = SiteWhere_Command_REGISTER;
	if (originator != NULL) {
		header.has_originator = true;
		strcpy(header.originator, originator);
	}
	if (!pb_encode_delimited(&stream, SiteWhere_Header_fields, &header)) {
		return 0;
	}

	SiteWhere_RegisterDevice registerDevice = { };
	strcpy(registerDevice.hardwareId, hardwareId);
	strcpy(registerDevice.specificationToken, specificationToken);
	if (!pb_encode_delimited(&stream, SiteWhere_RegisterDevice_fields, &registerDevice)) {
		return 0;
	}

	return stream.bytes_written;
}

unsigned int sw_acknowledge(char* hardwareId, char* message, uint8_t* buffer, size_t length, char* originator) {
	pb_ostream_t stream = pb_ostream_from_buffer(buffer, length);

	SiteWhere_Header header = { };
	header.command = SiteWhere_Command_ACKNOWLEDGE;
	if (originator != NULL) {
		header.has_originator = true;
		strcpy(header.originator, originator);
	}
	if (!pb_encode_delimited(&stream, SiteWhere_Header_fields, &header)) {
		return 0;
	}

	SiteWhere_Acknowledge ack = { };
	strcpy(ack.hardwareId, hardwareId);
	if (message != NULL) {
		ack.has_message = true;
		strcpy(ack.message, message);
	}
	if (!pb_encode_delimited(&stream, SiteWhere_Acknowledge_fields, &ack)) {
		return 0;
	}

	return stream.bytes_written;
}

unsigned int sw_measurement(char* hardwareId, char* name, float value, int64_t eventDate,
		uint8_t* buffer, size_t length, char* originator) {
	pb_ostream_t stream = pb_ostream_from_buffer(buffer, length);

	SiteWhere_Header header = { };
	header.command = SiteWhere_Command_DEVICEMEASUREMENT;
	if (originator != NULL) {
		header.has_originator = true;
		strcpy(header.originator, originator);
	}
	if (!pb_encode_delimited(&stream, SiteWhere_Header_fields, &header)) {
		return 0;
	}

	SiteWhere_DeviceMeasurements measurements = { };
	strcpy(measurements.hardwareId, hardwareId);

	SiteWhere_Measurement measurement = { };
	strcpy(measurement.measurementId, name);
	measurement.measurementValue = float_to_double(value);      //there is a problem
	//measurement.measurementValue = value; 
	
	measurements.measurement[0] = measurement;
	measurements.measurement_count = 1;

	if (eventDate != NULL) {
		measurements.has_eventDate = true;
		measurements.eventDate = eventDate;
	}
	if (!pb_encode_delimited(&stream, SiteWhere_DeviceMeasurements_fields, &measurements)) {
		return 0;
	}

	return stream.bytes_written;
}

unsigned int sw_location(char* hardwareId, float lat, float lon, float ele, int64_t eventDate,
		uint8_t* buffer, size_t length, char* originator) {
	pb_ostream_t stream = pb_ostream_from_buffer(buffer, length);

	SiteWhere_Header header = { };
	header.command = SiteWhere_Command_DEVICELOCATION;
	if (originator != NULL) {
		header.has_originator = true;
		strcpy(header.originator, originator);
	}
	if (!pb_encode_delimited(&stream, SiteWhere_Header_fields, &header)) {
		return 0;
	}

	SiteWhere_DeviceLocation location = { };
	strcpy(location.hardwareId, hardwareId);
	
	location.latitude = float_to_double(lat);                    //there is a problem
	location.longitude = float_to_double(lon);
	location.elevation = float_to_double(ele);
	//location.latitude = lat;
	//location.longitude = lon;
	//location.elevation = ele;

	location.has_elevation = true;

	if (eventDate != NULL) {
		location.has_eventDate = true;
		location.eventDate = eventDate;
	}
	if (!pb_encode_delimited(&stream, SiteWhere_DeviceLocation_fields, &location)) {
		return 0;
	}

	return stream.bytes_written;
}

unsigned int sw_alert(char* hardwareId, char* alertType, char* alertMessage, int64_t eventDate,
		uint8_t* buffer, size_t length, char* originator) {
	pb_ostream_t stream = pb_ostream_from_buffer(buffer, length);

	SiteWhere_Header header = { };
	header.command = SiteWhere_Command_DEVICEALERT;
	if (originator != NULL) {
		header.has_originator = true;
		strcpy(header.originator, originator);
	}
	if (!pb_encode_delimited(&stream, SiteWhere_Header_fields, &header)) {
		return 0;
	}

	SiteWhere_DeviceAlert alert = { };
	strcpy(alert.hardwareId, hardwareId);
	strcpy(alert.alertType, alertType);
	strcpy(alert.alertMessage, alertMessage);
	if (eventDate != NULL) {
		alert.has_eventDate = true;
		alert.eventDate = eventDate;
	}
	if (!pb_encode_delimited(&stream, SiteWhere_DeviceAlert_fields, &alert)) {
		return 0;
	}

	return stream.bytes_written;
}
