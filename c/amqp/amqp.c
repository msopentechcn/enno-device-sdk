#include "openiot-protocol.h"
#include <proton/message.h>
#include <proton/messenger.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void die(const char *file, int line, const char *message) {
    fprintf(stderr, "%s:%i: %s\n", file, line, message);
    exit(1);
}

void check(pn_messenger_t *messenger) {
    if (pn_messenger_errno(messenger)) {
        die(__FILE__, __LINE__, pn_error_text(pn_messenger_error(messenger)));
    }
}


static const int sendTimeout = 2000;
static const char *payloadkey = (char *) "payload";


pe_engine_t *pe_engine() {
    pe_engine_t *e = (pe_engine_t *) malloc(sizeof(pe_engine_t));
    e->engine = pn_messenger(NULL);
    return e;
}

void pe_start(pe_engine_t *e) {
    pn_messenger_t *messenger = (pn_messenger_t *) (e->engine);
    pn_messenger_start(messenger);
}

void pe_close(pe_engine_t *e) {
    pn_messenger_t *messenger = (pn_messenger_t *) (e->engine);
    pn_messenger_stop(messenger);
    pn_messenger_free(messenger);
}

/** Send message to AMQP broker */
void pe_send(pe_engine_t *engine, char *address, char *payload, int len) {

    pn_messenger_t *messenger = (pn_messenger_t *) (engine->engine);

    pn_message_t *message = pn_message();
    pn_message_set_address(message, address);

    pn_data_t *data = pn_message_properties(message);
    pn_data_put_map(data);
    pn_data_enter(data);
    pn_data_put_string(data, pn_bytes(strlen(payloadkey), payloadkey));
    pn_data_put_string(data, pn_bytes((size_t) len, payload));
    pn_data_exit(data);

    // pn_data_t * body = pn_message_body(message);
    // pn_data_put_string(body, pn_bytes(strlen(payload), payload));

    pn_messenger_put(messenger, message);

    check(messenger);
    pn_messenger_set_timeout(messenger, sendTimeout);
    pn_messenger_send(messenger, -1);

    check(messenger);

    pn_message_free(message);

}
