#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "huffmantree.h"

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Usage: %s <codes file> <zipint file> <unzipput file>\n", argv[0]);
        return 1;
    }

    // �򿪹����������ļ�
    FILE *codesFile = fopen(argv[1], "r");
    if (!codesFile)
    {
        fprintf(stderr, "�޷��򿪹����������ļ�\n");
        return 1;
    }

    // ���������������
    Node *huffmanTreeRoot = buildHuffmanTreeFromCodes(codesFile);
    fclose(codesFile);

    // ��������������ļ�
    decodeFile(argv[2], argv[3], huffmanTreeRoot);

    printf("�ļ���ѹ��ɣ���ѹ����ļ������ڣ�%s\n", argv[3]);
    return 0;
}
