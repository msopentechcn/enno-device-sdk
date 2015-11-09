#include "openiot-encode.h"
#include "openiot-protocol.h"
#include "support/misc_funcs.inc"

#include <assert.h>

#define MODE_NULL 0
#define MODE_REGISTER 1
#define MODE_MEASUREMENT 2
#define MODE_ALERT 3
#define MODE_LOCATION 4


void usage(void) {
    printf("Usage: openiot-agent-cli [-o addr] [message] -i [hardwareId] -s [specToken] -r|m|a|l [mode]\n");
    printf("-o     \tThe target address [amqp[s]/mqtt://domain[/name]]\n");
    printf("-i     \thardwhereId to uniquely identify this device\n");
    printf("-s     \tspecificationToken assigned by device onboarding server\n");
    printf("-r     \tregistration mode, no argument needed\n");
    printf("-m     \tmeasurement mode, with key:value argument,eg: cpu.utilization:0.55\n");
    printf("-a     \talert mode, with [key:value] format argument,eg: engine.overheat:temperature is above 1000\n");
    printf("-l     \tlocation mode, with [lat:lon:ele] format argument,eg: 23.0000:12.0000:154.0000\n");
    exit(0);
}


/** Unique hardware id for this device */
char *hardwareId = "";
/** Device specification token for hardware configuration */
char *specificationToken = "";
/** Outbound url*/
char *outboundAddress = "";


/** Message buffer */
uint8_t buffer[300];

/** Keeps up with whether we have registered */
//bool registered = true;

/** Timestamp for last event */
struct timeval lastEvent;
struct timeval now;

char **str_split(char *a_str, const char a_delim) {
    char **result = 0;
    size_t count = 0;
    char *tmp = a_str;
    char *last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp) {
        if (a_delim == *tmp) {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char *) * count);

    if (result) {
        size_t idx = 0;
        char *token = strtok(a_str, delim);

        while (token) {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}

int main(int argc, char **argv) {
    opterr = 0;
    int c = 0;
    int mode = MODE_NULL;

    // char * measurementString = (char *) malloc(64);
    // char * alertString = (char *) malloc(64);
    // char * locationString = (char *) malloc(64);

    char measurementString[64];
    char alertString[64];
    char locationString[64];

    while ((c = getopt(argc, argv, "hro:i:s:m:a:l:")) != -1) {

        switch (c) {
            case 'h':
                usage();
                break;
            case 'o':
                outboundAddress = optarg;
                break;
            case 'i':
                hardwareId = optarg;
                break;
            case 's':
                specificationToken = optarg;
                break;
            case 'r':
                mode = MODE_REGISTER;
                break;
            case 'm':
                mode = MODE_MEASUREMENT;
                strcpy(measurementString, optarg);
                break;
            case 'a':
                mode = MODE_ALERT;
                strcpy(alertString, optarg);
                break;
            case 'l':
                mode = MODE_LOCATION;
                strcpy(locationString, optarg);
                break;
            case '?':
                fprintf(stderr, "Wrong option -%c \n", optopt);
                return 1;

            default:
                usage();
        }
    }

    pe_engine_t *engine = pe_engine();
    pe_start(engine);

    char **tokens;
    unsigned int len = 0;
    switch (mode) {
        case MODE_REGISTER:
            if (len = openiot_register(hardwareId, specificationToken, buffer, sizeof(buffer), NULL)) {
                pe_send(engine, outboundAddress, buffer, len);
                printf("R");
            } else {
                fprintf(stderr, "create register message failed.\n");
            }

            break;

        case MODE_MEASUREMENT:
            if (len = openiot_measurement_multi(hardwareId, str_split(measurementString, '|'), 0, buffer,
                                                sizeof(buffer), NULL)) {
                pe_send(engine, outboundAddress, buffer, len);
                printf(".");
            } else {
                fprintf(stderr, "create measurement message failed.\n");
            }


            break;

        case MODE_ALERT:
            tokens = str_split(alertString, ':');

            if (len = openiot_alert(hardwareId, *tokens, *(tokens + 1), 0, buffer, sizeof(buffer), NULL)) {
                pe_send(engine, outboundAddress, buffer, len);
                printf("!");
            } else {
                fprintf(stderr, "create alive message failed.\n");
            }

            break;

        case MODE_LOCATION:
            tokens = str_split(locationString, ':');

            if (len = openiot_location(hardwareId, *tokens, *(tokens + 1), *(tokens + 2), 0, buffer, sizeof(buffer),
                                       NULL)) {
                pe_send(engine, outboundAddress, buffer, len);
                printf("+");
            } else {
                fprintf(stderr, "create location message failed.\n");
            }

            break;

        default:
            fprintf(stderr, "No operation mode specified.\n");
            usage();

    }

    pe_close(engine);
    return 0;

}