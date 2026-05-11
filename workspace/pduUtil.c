#include "pduUtil.h"
#include "checksum.h"

// pduBuffer is the buffer you fill with your PDU header/payload
// sequenceNumber = 32 bit sequence number passed in in host order
// flag = the type of the PDU (e.g. flag = 3 means data)
// payload = payload (data) of the PDU (treat the payload as just bytes)
// dataLen = length of the payload (so # of bytes in data),
//           this is used to memcpy the data into the PDU
// returns the length of the created PDU
//
int createPDU(uint8_t * pduBuffer, uint32_t sequenceNumber, uint8_t flag, uint8_t * payload, int payloadLen)
{
    int pduLen = 0;
    uint32_t sequenceNumber_net = 0;
    uint16_t cksumVal = 0;

    sequenceNumber_net = htonl(sequenceNumber);
    memcpy(pduBuffer, &sequenceNumber_net, sizeof(sequenceNumber_net));
    pduLen += sizeof(sequenceNumber_net);

    memset(pduBuffer + pduLen, 0, SIZEOF_CKSUM);
    pduLen += SIZEOF_CKSUM;

    memcpy(pduBuffer + pduLen, &flag, sizeof(flag));
    pduLen += sizeof(flag);

    memcpy(pduBuffer + pduLen, payload, payloadLen);
    pduLen += payloadLen;

    cksumVal = in_cksum((unsigned short *)pduBuffer, pduLen);
    memcpy(pduBuffer + sizeof(sequenceNumber_net), &cksumVal, sizeof(cksumVal));

    return pduLen;
}

void printPDU(uint8_t *pdu, int pduLen) 
{
    uint32_t sequenceNumber_net = 0;
    uint32_t sequenceNumber_host = 0;
    uint16_t cksumVal = 0;
    uint8_t flag = 0;
    uint8_t payloadLen = pduLen - SIZEOF_PDU_HEADER;
    char payload[payloadLen + 1];

    cksumVal = in_cksum((unsigned short *)pdu, pduLen);
    if (cksumVal != 0)
    {
        fprintf(stderr, "PDU corrupted, cksum failed");
        exit(-1);
    }

    memcpy(&sequenceNumber_net, pdu, sizeof(sequenceNumber_net));
    sequenceNumber_host = ntohl(sequenceNumber_net);

    memcpy(&flag, pdu + sizeof(sequenceNumber_net) + SIZEOF_CKSUM, sizeof(flag));

    memcpy(payload, pdu + SIZEOF_PDU_HEADER, payloadLen);
    payload[payloadLen] = '\0';

    printf(
        "Sequence Number: %d\n"
        "Flag: %d\n"
        "Payload Length: %d\n"
        "Payload: %s\n",
        sequenceNumber_host, flag, payloadLen, payload
    );
}