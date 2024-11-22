#ifndef _HUFFMANTREE_H_
#define _HUFFMANTREE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR 256

typedef struct Node
{
    char character;            // ´æ´¢×Ö·û
    struct Node *left, *right; // ×óÓÒ×Ó½Úµã
} Node;

Node *createNode(char character);
Node *buildHuffmanTreeFromCodes(FILE *inputFile);
void decodeFile(const char *inputFilename, const char *outputFilename, Node *huffmanTree);
#endif /* _HUFFMANTREE_H_ */