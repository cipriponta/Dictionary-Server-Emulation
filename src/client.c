#include "dependencies.h"

void showClientMenuMessageError()
{
    printf("Wrong Parameters, choose one of the following commands:\n");
    printf("Add word:           ./runServer <hostname> a <word>\n");
    printf("Add word:           ./runServer <hostname> A <word>\n");
    printf("Add definition:     ./runServer <hostname> d <word> <definition>\n");
    printf("Delete word:        ./runServer <hostname> s <word>\n");
    printf("Init dictionary:    ./runServer <hostname> i\n");
    printf("Show dictionary:    ./runServer <hostname> w\n");
}

int main(int argc, char *argv[])
{
    int socketfd;
    char messageBuffer[MAXBUFFERSIZE];
    char serverFeedbackMessageBuffer[MAXBUFFERSIZE];
    int serverFeedbackMessageBufferLength;
    struct hostent *server;
    struct sockaddr_in serverAddress;

    if(argc == 3 && (strcmp(argv[2], "i") == 0 || strcmp(argv[2], "w") == 0))
    {
        strcpy(messageBuffer, argv[2]);
    }
    else if(argc == 4 && (strcmp(argv[2], "a") == 0 || strcmp(argv[2], "A") == 0 || strcmp(argv[2], "s") == 0))
    {
        strcpy(messageBuffer, argv[2]);
        strcat(messageBuffer, "|");
        strcat(messageBuffer, argv[3]);
    }
    else if(argc == 5 && strcmp(argv[2], "d") == 0)
    {
        strcpy(messageBuffer, argv[2]);
        strcat(messageBuffer, "|");
        strcat(messageBuffer, argv[3]);
        strcat(messageBuffer, "|");
        strcat(messageBuffer, argv[4]);
    }
    else
    {
        showClientMenuMessageError();
        showMessageError("Client Wrong Parameters Number");
    }

    if((server = gethostbyname(argv[1])) == NULL)
    {
        showMessageError("Client Get Host By Name Error");
    }

    if((socketfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        showMessageError("Client Socket Error");
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(USED_PORT);
    serverAddress.sin_addr = *((struct in_addr *) server->h_addr);
    memset(&(serverAddress.sin_zero), '\0', 8);

    if(connect(socketfd, (struct sockaddr *)&serverAddress, sizeof(struct sockaddr)) == -1)
    {
        showMessageError("ClientConnect Error");
    }

    if(send(socketfd, messageBuffer, strlen(messageBuffer), 0) == -1)
    {
        showMessageError("Client Send Error");
    }

    if((serverFeedbackMessageBufferLength = recv(socketfd, serverFeedbackMessageBuffer, MAXBUFFERSIZE - 1, 0)) == -1)
    {
        perror("Server Receive Message From Client Error");
    }
    printf("%s\n", serverFeedbackMessageBuffer);

    close(socketfd);

    return 0;
}