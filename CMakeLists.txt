cmake_minimum_required(VERSION 3.2)
project(openiot_device)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    arduino/PubSubClient/PubSubClient.cpp
    arduino/PubSubClient/PubSubClient.h
    arduino/SiteWhere/agent/agent.c
    arduino/SiteWhere/agent/double_conversion.c
    arduino/SiteWhere/agent/double_conversion.h
    arduino/SiteWhere/agent/pb.h
    arduino/SiteWhere/agent/pb_decode.c
    arduino/SiteWhere/agent/pb_decode.h
    arduino/SiteWhere/agent/pb_encode.c
    arduino/SiteWhere/agent/pb_encode.h
    arduino/SiteWhere/agent/sitewhere-arduino.pb.c
    arduino/SiteWhere/agent/sitewhere-arduino.pb.h
    arduino/SiteWhere/agent/sitewhere.c
    arduino/SiteWhere/agent/sitewhere.h
    arduino/SiteWhere/examples/CustomCommands/custom.c
    arduino/SiteWhere/examples/CustomCommands/custom.h
    arduino/SiteWhere/double_conversion.c
    arduino/SiteWhere/double_conversion.h
    arduino/SiteWhere/pb.h
    arduino/SiteWhere/pb_decode.c
    arduino/SiteWhere/pb_decode.h
    arduino/SiteWhere/pb_encode.c
    arduino/SiteWhere/pb_encode.h
    arduino/SiteWhere/sitewhere-arduino.pb.c
    arduino/SiteWhere/sitewhere-arduino.pb.h
    arduino/SiteWhere/sitewhere.c
    arduino/SiteWhere/sitewhere.h
    c/support/internal/getopt.c
    c/support/internal/getopt.h
    c/support/misc_defs.h
    c/amqp.c
    c/double_conversion.c
    c/double_conversion.h
    c/openiot-agent-cli.c
    c/openiot-encode.c
    c/openiot-encode.h
    c/openiot.pb.c
    c/openiot.pb.h
    c/pb.h
    c/pb_decode.c
    c/pb_decode.h
    c/pb_encode.c
    c/pb_encode.h
    c/protocol.h)

add_executable(openiot_device ${SOURCE_FILES})