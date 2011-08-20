#ifndef ENDPOINTS_H
#define ENDPOINTS_H

uint8_t GetNumReadEndpoints();
uint8_t GetNumWriteEndpoints();

uint8_t GetReadEndpoint(uint8_t endpoint);
void SetWriteEndpoint(uint8_t endpoint, uint8_t value);

void SetReadEndpoint(uint8_t endpoint, uint8_t value);
uint8_t GetWriteEndpoint(uint8_t endpoint);

#endif