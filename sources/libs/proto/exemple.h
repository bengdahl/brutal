#pragma once

// This is file is auto generated by BID, don't edit it!

#include <syscalls/types.h>

typedef enum {
EXAMPLE_PC_LOAD_LETTER,
} ExampleError;

/* --- Types ---------------------------------------------------------------- */

typedef uint64_t ExampleTime;

typedef struct {
ExampleTime time;
} ExamplePingPong;

/* --- Messages ------------------------------------------------------------- */

typedef ExamplePingPong ExamplePingRequest;

typedef ExamplePingPong ExamplePingResponse;

typedef struct {
int foo;
} ExampleTestRequest;

typedef struct {
float bar;
} ExampleTestResponse;

typedef struct {
enum {
EXAMPLE_FOO,
EXAMPLE_BAR,
} type;
struct {
int data;
} nested;
} ExampleNotificationEvent;

typedef enum {
EXAMPLE_INVALID,
EXAMPLE_ERROR,
EXAMPLE_PING_REQUEST,
EXAMPLE_PING_RESPONSE,
EXAMPLE_TEST_REQUEST,
EXAMPLE_TEST_RESPONSE,
EXAMPLE_NOTIFICATION_EVENT,
} ExampleMessageType;

typedef struct {
BrMsgHeader header;
union {
ExampleError error;
ExampleNotificationEvent notification_event;
ExamplePingRequest ping_request;
ExamplePingResponse ping_response;
ExampleTestRequest test_request;
ExampleTestResponse test_response;
};
} ExampleMessage;
