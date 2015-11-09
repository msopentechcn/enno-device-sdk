typedef struct {
    void *engine;
} pe_engine_t;

pe_engine_t *pe_engine();

void pe_start(pe_engine_t *engine);

void pe_close(pe_engine_t *engine);


/** Send message to message broker */
void pe_send(pe_engine_t *sender, char *outbound, char *payload, int len);


void pe_subscribe(pe_engine_t *receiver, char *inbound, void *callback);


void loop(pe_engine_t *engine);
