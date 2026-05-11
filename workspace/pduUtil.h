#include <stdint.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZEOF_PDU_HEADER 7
#define SIZEOF_CKSUM 2

int createPDU(uint8_t * pduBuffer, uint32_t sequenceNumber, uint8_t flag, uint8_t * payload, int payloadLen);
void printPDU(uint8_t *pdu, int pduLen);