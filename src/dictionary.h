#ifndef __DICTIONARY_H__
#define __DICTIONARY_H__

#include "dependencies.h"

typedef struct dictionaryNode
{
    char word[MAXBUFFERSIZE];
    char definition[MAXBUFFERSIZE];
    struct dictionaryNode* next;
}dictionaryNode;

void initDictionary(dictionaryNode **root, int *checkIfInit);
int checkWordInDictionary(dictionaryNode *root, char *word);
void insertWord(dictionaryNode **root, char *word);
void modifiyWordDefinition(dictionaryNode **root, char *word, char *definition);
void deleteWord(dictionaryNode **root, char *word);
void showDictionary(dictionaryNode *root, char *clientFeedbackMessageBuffer);

#endif