#ifndef _HUFFMANTREE_H
#define _HUFFMANTREE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_CHAR 256 // ASCII?最大数量

typedef struct Node
{
    char character;     // 字符
    int frequency;      // 字符出现的频率
    struct Node *left;  // 左子树
    struct Node *right; // 右子树
} Node;                 // Huffman树节点

typedef struct MinHeap
{
    Node *nodes[MAX_CHAR]; // 存储Huffman树节点
    int size;              // 堆中元素数量
} MinHeap;                 // 最小堆

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