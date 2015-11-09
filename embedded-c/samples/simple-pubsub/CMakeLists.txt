cmake_minimum_required(VERSION 3.3)
project(linux-mqtt-simple-pubsub)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(ROOT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../")

include_directories(${ROOT_SOURCE_DIR}/src/common)
include_directories(${ROOT_SOURCE_DIR}/protocol_lib/mqtt/MQTTClient-C/src)
include_directories(${ROOT_SOURCE_DIR}/protocol_lib/mqtt/MQTTPacket/src)
include_directories(${ROOT_SOURCE_DIR}/src/protocol/mqtt)
include_directories(${ROOT_SOURCE_DIR}/src/protocol/mqtt/platform_linux)
include_directories(${ROOT_SOURCE_DIR}/serialization_lib/protobuf)
include_directories(./)

set(SOURCE_FILES
        pubsub.c
        enno_config.h
        ${ROOT_SOURCE_DIR}/protocol_lib/mqtt/MQTTClient-C/src/MQTTClient.c
        ${ROOT_SOURCE_DIR}/protocol_lib/mqtt/MQTTPacket/src/MQTTConnectClient.c
        ${ROOT_SOURCE_DIR}/protocol_lib/mqtt/MQTTPacket/src/MQTTConnectServer.c
        ${ROOT_SOURCE_DIR}/protocol_lib/mqtt/MQTTPacket/src/MQTTDeserializePublish.c
        ${ROOT_SOURCE_DIR}/protocol_lib/mqtt/MQTTPacket/src/MQTTFormat.c
        ${ROOT_SOURCE_DIR}/protocol_lib/mqtt/MQTTPacket/src/MQTTPacket.c
        ${ROOT_SOURCE_DIR}/protocol_lib/mqtt/MQTTPacket/src/MQTTSerializePublish.c
        ${ROOT_SOURCE_DIR}/protocol_lib/mqtt/MQTTPacket/src/MQTTSubscribeClient.c
        ${ROOT_SOURCE_DIR}/protocol_lib/mqtt/MQTTPacket/src/MQTTSubscribeServer.c
        ${ROOT_SOURCE_DIR}/protocol_lib/mqtt/MQTTPacket/src/MQTTUnsubscribe.h
        ${ROOT_SOURCE_DIR}/protocol_lib/mqtt/MQTTPacket/src/MQTTUnsubscribeClient.c
        ${ROOT_SOURCE_DIR}/protocol_lib/mqtt/MQTTPacket/src/MQTTUnsubscribeServer.c
        ${ROOT_SOURCE_DIR}/serialization_lib/protobuf/double_conversion.c
        ${ROOT_SOURCE_DIR}/serialization_lib/protobuf/pb_decode.c
        ${ROOT_SOURCE_DIR}/serialization_lib/protobuf/pb_encode.c
        ${ROOT_SOURCE_DIR}/serialization_lib/protobuf/sitewhere.c
        ${ROOT_SOURCE_DIR}/serialization_lib/protobuf/sitewhere.pb.c
        ${ROOT_SOURCE_DIR}/src/protocol/mqtt/platform_linux/network_linux.c
        ${ROOT_SOURCE_DIR}/src/protocol/mqtt/platform_linux/timer_linux.c
        ${ROOT_SOURCE_DIR}/src/protocol/mqtt/enno_api_mqtt_wrapper.c)

add_executable(simple-pubsub ${SOURCE_FILES})