// Client side - UDP Code				    
// By Hugh Smith	4/1/2017		

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "gethostbyname.h"
#include "networks.h"
#include "safeUtil.h"
#include "pduUtil.h"
#include "cpe464.h"

#define MAXBUF 80

void talkToServer(int socketNum, struct sockaddr_in6 * server);
int readFromStdin(char * buffer);
void checkArgs(int argc, char * argv[], double *errorRate, int *portNumber);

int main (int argc, char *argv[])
 {
	int socketNum = 0;				
	struct sockaddr_in6 server;		// Supports 4 and 6 but requires IPv6 struct
	double errorRate = 0;
	int portNumber = 0;
	
	checkArgs(argc, argv, &errorRate, &portNumber);
	sendErr_init(errorRate, DROP_ON, FLIP_OFF, DEBUG_ON, RSEED_OFF);

	socketNum = setupUdpClientToServer(&server, argv[2], portNumber);
	
	talkToServer(socketNum, &server);
	
	close(socketNum);

	return 0;
}

void talkToServer(int socketNum, struct sockaddr_in6 * server)
{
	int serverAddrLen = sizeof(struct sockaddr_in6);
	char * ipString = NULL;
	int dataLen = 0; 
	char buffer[MAXBUF+1];

	uint8_t pduBuffer[MAXBUF+1+SIZEOF_PDU_HEADER];
	uint32_t sequenceNumber = 0;
	uint8_t flag = 3;
	int pduLen = 0;
	int recvLen = 0;
	
	buffer[0] = '\0';
	while (buffer[0] != '.')
	{
		dataLen = readFromStdin(buffer);

		printf("Sending: %s with len: %d\n", buffer,dataLen);

		pduLen = createPDU(pduBuffer, sequenceNumber, flag, (uint8_t *) buffer, dataLen);
		sequenceNumber++;
		printPDU(pduBuffer, pduLen);
	
		safeSendto(socketNum, pduBuffer, pduLen, 0, (struct sockaddr *) server, serverAddrLen);
		
		recvLen = safeRecvfrom(socketNum, pduBuffer, MAXBUF+SIZEOF_PDU_HEADER, 0, (struct sockaddr *) server, &serverAddrLen);
		
		// print out bytes received
		ipString = ipAddressToString(server);
		printf("Server with ip: %s and port %d said it received %s\n", ipString, ntohs(server->sin6_port), buffer);

		printPDU(pduBuffer, recvLen);
	}
}

int readFromStdin(char * buffer)
{
	char aChar = 0;
	int inputLen = 0;        
	
	// Important you don't input more characters than you have space 
	buffer[0] = '\0';
	printf("Enter data: ");
	while (inputLen < (MAXBUF - 1) && aChar != '\n')
	{
		aChar = getchar();
		if (aChar != '\n')
		{
			buffer[inputLen] = aChar;
			inputLen++;
		}
	}
	
	// Null terminate the string
	buffer[inputLen] = '\0';
	inputLen++;
	
	return inputLen;
}

void checkArgs(int argc, char * argv[], double *errorRate, int *portNumber)
{	
    /* check command line arguments  */
	
	if (argc != 4)
	{
		printf("usage: %s error-rate host-name port-number \n", argv[0]);
		exit(1);
	}
	
	*errorRate = atof(argv[1]);
	*portNumber = atoi(argv[3]);
		
	if ((*errorRate > 1) || (*errorRate < 0)) {
		printf("error rate must be between 1 and 0\n");
		exit(1);
	}

	return;
}





