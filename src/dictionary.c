#include "dictionary.h"

void initDictionary(dictionaryNode **root, int *checkIfInit)
{
    *root = NULL;
    *checkIfInit = 1;
}

int checkWordInDictionary(dictionaryNode *root, char *word)
{
    dictionaryNode *i = root;

    while(i != NULL)
    {
        if(strcmp(i->word, word) == 0)
        {
            return 1;
        }
        i = i->next;
    }
    return 0;
}

void insertWord(dictionaryNode **root, char *word)
{
    dictionaryNode *node;
    node = (dictionaryNode*)malloc(sizeof(dictionaryNode));
    strcpy(node->word, word);
    strcpy(node->definition, "");
    node->next = NULL;

    if((*root) == NULL)
    {
        (*root) = node;
    }
    else if(strcmp(word, (*root)->word) < 0)
    {
        node->next = (*root);
        (*root) = node;
    }
    else
    {
        dictionaryNode *i;
        i = (*root);
        while(i->next != NULL && strcmp(i->next->word, word) < 0)
        {
            i = i->next;
        }
        node->next = i->next;
        i->next = node;
    }
}

void modifiyWordDefinition(dictionaryNode **root, char *word, char *definition)
{
    dictionaryNode *i = *root;

    while(i != NULL)
    {
        if(strcmp(i->word, word) == 0)
        {
            strcpy(i->definition, definition);
        }
        i = i->next;
    }
}

void deleteWord(dictionaryNode **root, char *word)
{
    dictionaryNode *p, *q;

    if((*root) != NULL)
    {
        if(strcmp((*root)->word, word) == 0)
        {
            p = *root;
            *root = (*root)->next;
            free(p);
        }
        else
        {
            q = *root;
            
            while(q->next != NULL && strcmp(q->next->word, word) != 0)
            {
                q = q->next;
            }

            if(q->next != NULL && strcmp(q->next->word, word) == 0)
            {
                p = q->next;
                q->next = q->next->next;
                free(p);
            }
        }
    }
}

void showDictionary(dictionaryNode *root, char *clientFeedbackMessageBuffer)
{
    dictionaryNode *i = root;

    while(i != NULL)
    {
        strcat(clientFeedbackMessageBuffer, "<");
        strcat(clientFeedbackMessageBuffer, i->word);
        strcat(clientFeedbackMessageBuffer, "> - <");
        strcat(clientFeedbackMessageBuffer, i->definition);
        strcat(clientFeedbackMessageBuffer, ">\n");
        i = i->next;
    }
}