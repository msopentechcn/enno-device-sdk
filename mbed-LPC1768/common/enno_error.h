#ifndef ENNO_ERROR_INTERFACE_H
#define ENNO_ERROR_INTERFACE_H

#if defined(__cplusplus) /* If this is a C++ compiler, use C linkage */
extern "C" {
#endif
/**
 * @brief IoT Error enum
 *
 * Enumeration of return values from the IoT_* functions within the SDK.
 */

typedef enum {
    NONE_ERROR = 0,
    /** Success return value - no error occurred. */
            GENERIC_ERROR = -1,
    /** A generic error.  A placeholder for a more specific error. */
            NULL_VALUE_ERROR = -2,
    /** A required parameter was passed as null. */
            CONNECTION_ERROR = -3,
    /** A connection could not be established. */
            SUBSCRIBE_ERROR = -4,
    /** The subscribe failed.  A SUBACK was not returned from the service. */
            PUBLISH_ERROR = -5,
    /** The publish failed.  In the case of a QoS 1 message a PUBACK was not received. */
            DISCONNECT_ERROR = -6,
    /** The disconnect failed.  The disconnect control packet could not be sent. */
            YIELD_ERROR = -7,
    /** An error occurred when yielding to the IoT MQTT client.  A possible cause is an unexpected TCP socket disconnect. */
            TCP_CONNECT_ERROR = -8,
    /** The TCP socket could not be established. */
            UNSUBSCRIBE_ERROR = -15,            /** The unsubscribe failed.  The unsubscribe control packet could not be sent. */

} Error_t;
#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
}
#endif

#endif
