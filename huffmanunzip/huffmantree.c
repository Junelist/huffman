#include "huffmantree.h"

// ����һ���ڵ�
Node *createNode(char character)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->character = character;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// ����������������
Node *buildHuffmanTreeFromCodes(FILE *inputFile)
{
    Node *root = createNode('\0'); // �������ڵ�
    char line[MAX_CHAR], code[MAX_CHAR];
    int character;

    // ��ȡÿ�е�ASC��Ͷ�Ӧ�ı���
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
        current->character = (char)character; // ����Ҷ�ӽڵ��ַ�
    }

    return root;
}

// ��������������ļ�
void decodeFile(const char *inputFilename, const char *outputFilename, Node *huffmanTree)
{
    FILE *inputFile = fopen(inputFilename, "rb");
    FILE *outputFile = fopen(outputFilename, "wb");

    if (!inputFile || !outputFile)
    {
        fprintf(stderr, "�ļ���ʧ�ܡ�\n");
        return;
    }

    Node *currentNode = huffmanTree;
    unsigned char buffer;
    while (fread(&buffer, sizeof(unsigned char), 1, inputFile) == 1)
    {
        for (int i = 7; i >= 0; i--)
        {
            // ��ȡ��ǰ�ֽڵĵ� i λ���Ӹ�λ����λ��
            int bit = (buffer >> i) & 1;

            // ���ݵ�ǰλ����������ƶ�
            if (bit == 0)
            {
                currentNode = currentNode->left;
            }
            else
            {
                currentNode = currentNode->right;
            }

            // �������Ҷ�ӽڵ㣬������ַ�
            if (currentNode->left == NULL && currentNode->right == NULL)
            {
                fputc(currentNode->character, outputFile);
                currentNode = huffmanTree; // �ص����ڵ�
            }
        }
    }

    fclose(inputFile);
    fclose(outputFile);
}
