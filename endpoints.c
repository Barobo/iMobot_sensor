#include "global.h"
#include "endpoints.h"

#define NUM_READ_ENDPOINTS 4
#define NUM_WRITE_ENDPOINTS 4

uint8_t read_endpoints[NUM_READ_ENDPOINTS] = { 0x12, 0x23, 0x34, 0x45 };
uint8_t write_endpoints[NUM_WRITE_ENDPOINTS] = { 0x00, 0x00, 0x00, 0x00 };

uint8_t GetNumReadEndpoints()
{
    return NUM_READ_ENDPOINTS;
}
uint8_t GetNumWriteEndpoints()
{
    return NUM_WRITE_ENDPOINTS;
}

uint8_t GetReadEndpoint(uint8_t endpoint)
{
    if (endpoint < NUM_READ_ENDPOINTS)
        return read_endpoints[endpoint];
    else
        return 0;
}

void SetWriteEndpoint(uint8_t endpoint, uint8_t value)
{
    if (endpoint < NUM_WRITE_ENDPOINTS)
        write_endpoints[endpoint] = value;
}


void SetReadEndpoint(uint8_t endpoint, uint8_t value)
{
    if (endpoint < NUM_READ_ENDPOINTS)
        read_endpoints[endpoint] = value;
}
uint8_t GetWriteEndpoint(uint8_t endpoint)
{
    if (endpoint < NUM_WRITE_ENDPOINTS)
        return write_endpoints[endpoint];
    else
        return 0;
}