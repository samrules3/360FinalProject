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
    int sock;                        	/* Socket */
    struct sockaddr_in servAddr;     	/* Local address */
    struct sockaddr_in clntAddr;     	/* Client address */
    unsigned short servPort;         	/* Server port */
    unsigned int cliAddrLen;		/* Length of message */
    char msgBuffer[STRMAX];		/* Buffer fo the incoming message */
    int msgSize;                     	/* Size of received message */
    

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
    memset(&servAddr, 0, sizeof(servAddr));   /* Zero out structure */
    servAddr.sin_family = AF_INET;                /* Internet address family */
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    servAddr.sin_port = htons(servPort);      /* Local port */

    /* Bind to the local address */
    if (bind(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
        DieWithError("bind() failed");

    for (;;) /* Run forever */
    {
        /* Set the size of the in-out parameter */
        cliAddrLen = sizeof(clntAddr);

        /* Block until receive message from a client */
        if ((msgSize = recvfrom(sock, msgBuffer, STRMAX, 0,
            (struct sockaddr *) &clntAddr, &cliAddrLen)) < 0)
            DieWithError("recvfrom() failed");
	

    }
    /* NOT REACHED */
