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

    // 打开哈夫曼编码文件
    FILE *codesFile = fopen(argv[1], "r");
    if (!codesFile)
    {
        fprintf(stderr, "无法打开哈夫曼编码文件\n");
        return 1;
    }

    // 构建解码哈夫曼树
    Node *huffmanTreeRoot = buildHuffmanTreeFromCodes(codesFile);
    fclose(codesFile);

    // 解码哈夫曼编码文件
    decodeFile(argv[2], argv[3], huffmanTreeRoot);

    printf("文件解压完成，解压后的文件保存在：%s\n", argv[3]);
    return 0;
}
