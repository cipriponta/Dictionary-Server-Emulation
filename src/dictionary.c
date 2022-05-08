#include "dictionary.h"

void initDictionary(dictionaryNode **root, int *checkIfInit)
{
    *root = NULL;
    *checkIfInit = 1;
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

void showDictionary(dictionaryNode *root)
{
    dictionaryNode *i = root;

    while(i != NULL)
    {
        printf("\n<%s> - <%s>", i->word, i->definition);
        i = i->next;
    }
}