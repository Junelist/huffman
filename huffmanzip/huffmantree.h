#ifndef _HUFFMANTREE_H
#define _HUFFMANTREE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_CHAR 256 // ASCII?�������

typedef struct Node
{
    char character;     // �ַ�
    int frequency;      // �ַ����ֵ�Ƶ��
    struct Node *left;  // ������
    struct Node *right; // ������
} Node;                 // Huffman���ڵ�

typedef struct MinHeap
{
    Node *nodes[MAX_CHAR]; // �洢Huffman���ڵ�
    int size;              // ����Ԫ������
} MinHeap;                 // ��С��

void byte_times(const unsigned char *data, long data_size);
Node *createNode(char character, int frequency);
void set_nodes();
MinHeap *createMinHeap();
void swapNodes(Node **a, Node **b);
void heapifyUp(MinHeap *heap, int idx);
void heapifyDown(MinHeap *heap, int idx);
void insertNode(MinHeap *heap, Node *node);
Node *extractMin(MinHeap *heap);
Node *buildHuffmanTree(MinHeap *heap);
void generateHuffmanCodes(Node *root, char *code, int length, FILE *outputFile);
void writeEncodedFile(const unsigned char *data, long data_size, const char *outputFilename);

#endif // _HUFFMANTREE_H