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
    // 将文件中的字节读取到字节流中
    unsigned char *data = readFileToBytes(argv[1], &file_size);

    if (data == NULL)
    {
        return 1;
    }

    // 统计字节流中每个字节出现的次数
    byte_times(data, file_size);
    set_nodes();

    // 创建最小堆，并将每个字符节点插入堆中
    MinHeap *heap = createMinHeap();
    for (int i = 0; i < nodeCount; i++)
    {
        insertNode(heap, nodes[i]);
    }

    // 构建哈夫曼树
    Node *huffmanTreeRoot = buildHuffmanTree(heap);

    // 生成哈夫曼编码
    FILE *outputFile = fopen(argv[2], "wb");
    char code[MAX_CHAR]; // 用于存储正在生成的当前代码的临时数组
    generateHuffmanCodes(huffmanTreeRoot, code, 0, outputFile);

    // 压缩文件
    writeEncodedFile(data, file_size, argv[3]);

    printf("文件压缩完成，文件哈夫曼编码保存在：%s，压缩后的文件保存在：%s\n", argv[2], argv[3]);

    free(data);
    return 0;
}
