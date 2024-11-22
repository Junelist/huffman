#include "huffmantree.h"

// 创建一个节点
Node *createNode(char character)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->character = character;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// 构建哈夫曼解码树
Node *buildHuffmanTreeFromCodes(FILE *inputFile)
{
    Node *root = createNode('\0'); // 创建根节点
    char line[MAX_CHAR], code[MAX_CHAR];
    int character;

    // 读取每行的ASC码和对应的编码
    while (fscanf(inputFile, "%d:%s", &character, code) != EOF)
    {
        Node *current = root;
        for (int i = 0; code[i] != '\0'; i++)
        {
            if (code[i] == '0')
            {
                if (!current->left)
                    current->left = createNode('\0');
                current = current->left;
            }
            else if (code[i] == '1')
            {
                if (!current->right)
                    current->right = createNode('\0');
                current = current->right;
            }
        }
        current->character = (char)character; // 设置叶子节点字符
    }

    return root;
}

// 解码哈夫曼编码文件
void decodeFile(const char *inputFilename, const char *outputFilename, Node *huffmanTree)
{
    FILE *inputFile = fopen(inputFilename, "rb");
    FILE *outputFile = fopen(outputFilename, "wb");

    if (!inputFile || !outputFile)
    {
        fprintf(stderr, "文件打开失败。\n");
        return;
    }

    Node *currentNode = huffmanTree;
    unsigned char buffer;
    while (fread(&buffer, sizeof(unsigned char), 1, inputFile) == 1)
    {
        for (int i = 7; i >= 0; i--)
        {
            // 获取当前字节的第 i 位（从高位到低位）
            int bit = (buffer >> i) & 1;

            // 根据当前位向左或向右移动
            if (bit == 0)
            {
                currentNode = currentNode->left;
            }
            else
            {
                currentNode = currentNode->right;
            }

            // 如果到达叶子节点，输出该字符
            if (currentNode->left == NULL && currentNode->right == NULL)
            {
                fputc(currentNode->character, outputFile);
                currentNode = huffmanTree; // 回到根节点
            }
        }
    }

    fclose(inputFile);
    fclose(outputFile);
}
