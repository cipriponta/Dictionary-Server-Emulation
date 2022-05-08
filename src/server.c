// ***************************************************************************************************
//  ToDo:
//  Server  - Dictionary Functionality
//              - Dictionary Structure (alphabetically ordered linked list)
//              - Client Feedback
//          - Add threads
// ***************************************************************************************************

#include "dependencies.h"
#include "dictionary.h"

typedef enum clientOptionEnum
{
    e_INIT = 0,
    e_ADDWORD,
    e_ADDDEF,
    e_DELETE,
    e_SHOW,
    e_NONE
}clientOptionEnum;

clientOptionEnum parseClientMessage(char *clientMessage, char **word, char **definition)
{
    if(strcmp(clientMessage, "i") == 0)
    {
        return e_INIT;
    }

    if(clientMessage[0] == 'a' || clientMessage[0] == 'A')
    {
        strtok(clientMessage, "|");
        *word = strtok(NULL, "|");
        return e_ADDWORD;
    }

    if(clientMessage[0] == 'd')
    {
        strtok(clientMessage, "|");
        *word = strtok(NULL, "|");
        *definition = strtok(NULL, "|");
        return e_ADDDEF;
    }

    if(clientMessage[0] == 's')
    {
        strtok(clientMessage, "|");
        *word = strtok(NULL, "|");
        return e_DELETE;
    }

    if(strcmp(clientMessage, "w") == 0)
    {
        return e_SHOW;
    }

    return e_NONE;
}

int main()
{
    int socketfd;
    int newsocketfd;
    struct sockaddr_in serverAddress;
    struct sockaddr_in clientAddress;
    int socketSize;
    int checkSock = 1;
    dictionaryNode *root;
    int checkIfInit = 0;

    if((socketfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        showMessageError("Server Socket Error");
    }

    if(setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &checkSock, sizeof(int)) == -1)
    {
        showMessageError("Server Set Socket Option Error");
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(USED_PORT);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    memset(&(serverAddress.sin_zero), '\0', 8);

    if(bind(socketfd, (struct sockaddr *)&serverAddress, sizeof(struct sockaddr)) == -1)
    {
        showMessageError("Server Bind Error");
    }

    if(listen(socketfd, BACKLOG) == -1)
    {
        showMessageError("Server Listen Error");
    }

    while(1)
    {
        char clientMessageBuffer[MAXBUFFERSIZE];
        int  clientMessageBufferLength;
        char clientFeedbackMessageBuffer[MAXBUFFERSIZE];
        char *word;
        char *definition;

        socketSize = sizeof(struct sockaddr_in);
        if((newsocketfd = accept(socketfd, (struct sockaddr *)&clientAddress, &socketSize)) == -1)
        {
            perror("Server Accept Error");
            continue;
        }

        printf("New connection from: %s\n", inet_ntoa(clientAddress.sin_addr));

        if((clientMessageBufferLength = recv(newsocketfd, clientMessageBuffer, MAXBUFFERSIZE - 1, 0)) == -1)
        {
            perror("Server Receive Message From Client Error");
            continue;
        }
        clientMessageBuffer[clientMessageBufferLength] = '\0';

        printf("Command from client: ");
        switch(parseClientMessage(clientMessageBuffer, &word, &definition))
        {
            case e_INIT:
            printf("Init Dictionary");
            initDictionary(root, &checkIfInit);
            break;
            
            case e_ADDWORD:
            printf("Add word <%s>", word);
            break;

            case e_ADDDEF:
            printf("Add word <%s> with definition <%s>", word, definition);
            break;

            case e_DELETE:
            printf("Delete word <%s>", word);
            break;

            case e_SHOW:
            printf("Show Dictionary");
            break;

            default:
            showMessageError("Server Unknown Option From Client Error");
            break;
        }
        printf("\n\n");

        close(newsocketfd);
    }

    return 0;
}