#include <stdio.h>
#include <stdlib.h>

#include "readfile.h"
#include "huffmantree.h"

extern int nodeCount;
extern Node *nodes[MAX_CHAR];

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Usage: %s <input file> <output file> <zipput file>\n", argv[0]);
        return 1;
    }

    long file_size;
    // ���ļ��е��ֽڶ�ȡ���ֽ�����
    unsigned char *data = readFileToBytes(argv[1], &file_size);

    if (data == NULL)
    {
        return 1;
    }

    // ͳ���ֽ�����ÿ���ֽڳ��ֵĴ���
    byte_times(data, file_size);
    set_nodes();

    // ������С�ѣ�����ÿ���ַ��ڵ�������
    MinHeap *heap = createMinHeap();
    for (int i = 0; i < nodeCount; i++)
    {
        insertNode(heap, nodes[i]);
    }

    // ������������
    Node *huffmanTreeRoot = buildHuffmanTree(heap);

    // ���ɹ���������
    FILE *outputFile = fopen(argv[2], "wb");
    char code[MAX_CHAR]; // ���ڴ洢�������ɵĵ�ǰ�������ʱ����
    generateHuffmanCodes(huffmanTreeRoot, code, 0, outputFile);

    // ѹ���ļ�
    writeEncodedFile(data, file_size, argv[3]);

    printf("�ļ�ѹ����ɣ��ļ����������뱣���ڣ�%s��ѹ������ļ������ڣ�%s\n", argv[2], argv[3]);

    free(data);
    return 0;
}
