/* Automatically generated nanopb constant definitions */
/* Generated by nanopb-0.3.5-dev at Sun Nov 22 16:31:39 2015. */

#include "raspberrypi.pb.h"

#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif



const pb_field_t Spec_QTwHw2YxY9EFXmPrzmX4pX_fields[1] = {
    PB_LAST_FIELD
};

const pb_field_t Spec_QTwHw2YxY9EFXmPrzmX4pX__Header_fields[5] = {
    PB_FIELD(  1, UENUM   , REQUIRED, STATIC  , FIRST, Spec_QTwHw2YxY9EFXmPrzmX4pX__Header, command, command, 0),
    PB_FIELD(  2, STRING  , OPTIONAL, CALLBACK, OTHER, Spec_QTwHw2YxY9EFXmPrzmX4pX__Header, originator, command, 0),
    PB_FIELD(  3, STRING  , OPTIONAL, CALLBACK, OTHER, Spec_QTwHw2YxY9EFXmPrzmX4pX__Header, nestedPath, originator, 0),
    PB_FIELD(  4, STRING  , OPTIONAL, CALLBACK, OTHER, Spec_QTwHw2YxY9EFXmPrzmX4pX__Header, nestedSpec, nestedPath, 0),
    PB_LAST_FIELD
};

const pb_field_t Spec_QTwHw2YxY9EFXmPrzmX4pX_ping_fields[1] = {
    PB_LAST_FIELD
};

const pb_field_t Spec_QTwHw2YxY9EFXmPrzmX4pX_testEvents_fields[1] = {
    PB_LAST_FIELD
};

const pb_field_t Spec_QTwHw2YxY9EFXmPrzmX4pX_helloWorld_fields[3] = {
    PB_FIELD(  1, STRING  , REQUIRED, CALLBACK, FIRST, Spec_QTwHw2YxY9EFXmPrzmX4pX_helloWorld, greeting, greeting, 0),
    PB_FIELD(  2, BOOL    , REQUIRED, STATIC  , OTHER, Spec_QTwHw2YxY9EFXmPrzmX4pX_helloWorld, loud, greeting, 0),
    PB_LAST_FIELD
};


