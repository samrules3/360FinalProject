/*  Server.c
*   Purpose: To take the incoming messages from a client and retrieve the information from an HTTP server
*
*
*/

#include "UDPIncludes.h"
#include <stdlib.h>




void DieWithError(char *errorMessage);  /* External error handling function */

int main(int argc, char *argv[])
{
    int sock;                    /* Socket */
    struct sockaddr_in servAddr; /* Local address */
    struct sockaddr_in clntAddr; /* Client address */
    unsigned short servPort;     /* Server port */
    unsigned int cliAddrLen;     /* Length of message */
    char msgBuffer[STRMAX];	     /* Buffer for the incoming message */
    int msgSize;                 /* Size of received message */

	int hSock;                     /* socket for http requests*/
	struct sockaddr_in hServAddr;    /* address of http server */
	unsigned short gpsPort = 8082;
	unsigned short dGpsPort = 8084;
	unsigned short lasersPort = 8083;
	char httpBuffer[STRMAX];       /* Buffer for http request */
    
    if (argc != 2)         	/* Test for correct number of parameters */
    {
        fprintf(stderr,
                "Usage:  %s <UDP SERVER PORT>\n", 
                argv[0]);
        exit(1);
    }

    servPort = atoi(argv[1]);  /* First arg:  local port */

    /* Create socket for sending/receiving datagrams */
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        DieWithError("socket() failed");

    /* Construct local address structure */
    memset(&servAddr, 0, sizeof(servAddr));       /* Zero out structure */
    servAddr.sin_family = AF_INET;                /* Internet address family */
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    servAddr.sin_port = htons(servPort);          /* Local port */

    /* Bind to the local address */
    if (bind(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
        DieWithError("bind() failed");	
		
	/* Http setup */
	if ( (hSock = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
		DieWithError("Socket");
		
    memset(&hServAddr, 0, sizeof(hServAddr));      /* Zero out structure */
    hServAddr.sin_family = AF_INET;                /* Internet address family */
    hServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
//    hServAddr.sin_port = htons(gpsPort);         /* Local port */
	if (inet_addr("130.127.192.62", &hServAddr.sin_addr.s_addr) == 0)
		DieWithError("Socket Failed");

    for (;;) /* Run forever */
    {
        /* Set the size of the in-out parameter */
        cliAddrLen = sizeof(clntAddr);

        /* Block until receive message from a client */
        if ((msgSize = recvfrom(sock, msgBuffer, STRMAX, 0,
            (struct sockaddr *) &clntAddr, &cliAddrLen)) < 0)
            DieWithError("recvfrom() failed");
	
		/* stupid http stuff, ignore mostly cause stupid */
		/* determine and set variables */
		if (msgType == GPS || msgType == MOVE || msgType == TURN || msgType == STOP) {
			hServAddr.sin_port = htons(gpsPort);
		} else if (msgType == DGPS) {
			hServAddr.sin_port = htons(dGpsPort);
		} else if (msgType == LASERS) {
			hServAddr.sin_port = htons(lasersPort);
		}
		if (connect(hSock, (struct sockaddr *) &hServAddr, sizeof(hServAddr)) != 0)
			DieWithError("bind() failed");
		if (msgType == GPS || msgType == DGPS || msgType == LASERS) {
			sprintf(httpBuffer, "Get %s HTTP/1.0\n\n", msgType);
		} else if (msgType == MOVE)
			sprintf(httpBuffer, "Get %s HTTP/1.0\n\n", argv[2]);
		}
		/* send request */
		send(hServAddr, httpBuffer, strlen(httpBuffer), 0);
    }
    /* NOT REACHED */
}
