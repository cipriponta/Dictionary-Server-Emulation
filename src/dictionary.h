#ifndef __DICTIONARY_H__
#define __DICTIONARY_H__

#include "dependencies.h"

typedef struct dictionaryNode
{
    char word[MAXBUFFERSIZE];
    char definition[MAXBUFFERSIZE];
}dictionaryNode;

void initDictionary(dictionaryNode *root, int *checkIfInit);
void insertWord(dictionaryNode *root, char *word);
void modifiyWord(dictionaryNode *root, char *word, char *definition);
void deleteWord(dictionaryNode *root, char *word);
void showDictionary(dictionaryNode *root);

#endif