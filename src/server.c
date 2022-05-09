// ***************************************************************************************************
//  ToDo:
//  Server - Add threads
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

static dictionaryNode *root;
static int checkIfInit = 0;

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

void connectionHandler(void *arg)
{
    int newsocketfd = *((int*) arg);
    char clientMessageBuffer[MAXBUFFERSIZE];
    int  clientMessageBufferLength;
    char clientFeedbackMessageBuffer[MAXBUFFERSIZE];
    char *word;
    char *definition;

    strcpy(clientFeedbackMessageBuffer, "");

    if((clientMessageBufferLength = recv(newsocketfd, clientMessageBuffer, MAXBUFFERSIZE - 1, 0)) == -1)
    {
        showMessageError("Server Receive Message From Client Error");
    }
    clientMessageBuffer[clientMessageBufferLength] = '\0';

    switch(parseClientMessage(clientMessageBuffer, &word, &definition))
    {
        case e_INIT:
        strcat(clientFeedbackMessageBuffer, "Init Dictionary");
        initDictionary(&root, &checkIfInit);
        break;
        
        case e_ADDWORD:
        if(!checkIfInit)
        {
            strcat(clientFeedbackMessageBuffer, "Init dictionary first");
            
        }
        else if(checkWordInDictionary(root, word))
        {
            strcat(clientFeedbackMessageBuffer, "Word <");
            strcat(clientFeedbackMessageBuffer, word);
            strcat(clientFeedbackMessageBuffer, "> is already in the dictionary");
        }
        else
        {
            insertWord(&root, word);
            strcat(clientFeedbackMessageBuffer, "Word <");
            strcat(clientFeedbackMessageBuffer, word);
            strcat(clientFeedbackMessageBuffer, "> was added");
        }
        break;

        case e_ADDDEF:
        if(!checkIfInit)
        {
            strcat(clientFeedbackMessageBuffer, "Init dictionary first");
            
        }
        else if(!checkWordInDictionary(root, word))
        {
            strcat(clientFeedbackMessageBuffer, "Word <");
            strcat(clientFeedbackMessageBuffer, word);
            strcat(clientFeedbackMessageBuffer, "> is not in the dictionary");
        }
        else
        {
            modifiyWordDefinition(&root, word, definition);
            strcat(clientFeedbackMessageBuffer, "Word <");
            strcat(clientFeedbackMessageBuffer, word);
            strcat(clientFeedbackMessageBuffer, "> has now the definition <");
            strcat(clientFeedbackMessageBuffer, definition);
            strcat(clientFeedbackMessageBuffer, ">");
        }
        break;

        case e_DELETE:
        if(!checkIfInit)
        {
            strcat(clientFeedbackMessageBuffer, "Init dictionary first");
            
        }
        else if(!checkWordInDictionary(root, word))
        {
            strcat(clientFeedbackMessageBuffer, "Word <");
            strcat(clientFeedbackMessageBuffer, word);
            strcat(clientFeedbackMessageBuffer, "> is not in the dictionary");
        }
        else
        {
            deleteWord(&root, word);
            strcat(clientFeedbackMessageBuffer, "Word <");
            strcat(clientFeedbackMessageBuffer, word);
            strcat(clientFeedbackMessageBuffer, "> was deleted");
        }
        break;

        case e_SHOW:
        if(checkIfInit)
        {
            showDictionary(root, clientFeedbackMessageBuffer);
        }
        else
        {
            strcat(clientFeedbackMessageBuffer, "Init dictionary first");
        }
        break;

        default:
        showMessageError("Server Unknown Option From Client Error");
        break;
    }

    if(send(newsocketfd, clientFeedbackMessageBuffer, strlen(clientFeedbackMessageBuffer), 0) == -1)
    {
        showMessageError("Server Send Feedback To Client Error");
    }

    close(newsocketfd);
}

int main()
{
    int socketfd;
    int newsocketfd;
    struct sockaddr_in serverAddress;
    struct sockaddr_in clientAddress;
    int socketSize;
    int checkSock = 1;

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
        socketSize = sizeof(struct sockaddr_in);
        if((newsocketfd = accept(socketfd, (struct sockaddr *)&clientAddress, &socketSize)) == -1)
        {
            perror("Server Accept Error");
            continue;
        }

        printf("New connection from: %s\n", inet_ntoa(clientAddress.sin_addr));

        connectionHandler((void*)&newsocketfd);
    }

    return 0;
}