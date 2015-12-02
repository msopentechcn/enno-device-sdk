/*
 * enno_error.h
 *
 *  Created on: 2015年11月20日
 *      Author: wzy
 */

#ifndef COM_ENNO_ERROR_H_
#define COM_ENNO_ERROR_H_

#if defined(__cplusplus)
extern "C" {
#endif

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

#ifdef __cplusplus
}
#endif
#endif /* COM_ENNO_ERROR_H_ */
