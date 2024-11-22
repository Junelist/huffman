#include "huffmantree.h"

int ASC[MAX_CHAR] = {0};        // ASC���
int nodeCount = 0;              // �������������ٽڵ�����
Node *nodes[MAX_CHAR];          // �ڵ��ַ����,�����ݴ�ڵ�
char codes[MAX_CHAR][MAX_CHAR]; // �����洢ÿ���ַ��ı���

void byte_times(const unsigned char *data, long data_size)
{
    for (long i = 0; i < data_size; i++)
    {
        ASC[data[i]]++; // ͳ��ÿ���ֽڵĳ��ִ���
    }
}

Node *createNode(char character, int frequency)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->character = character;
    node->frequency = frequency;

    // ��ʼ�������ӽڵ�ָ��ΪNULL
    node->left = NULL;
    node->right = NULL;

    return node;
}

void set_nodes()
{
    for (int i = 0; i < MAX_CHAR; i++)
    {
        if (ASC[i] > 0) // ����ַ���ASC����г���
        {
            nodes[nodeCount++] = createNode((char)i, ASC[i]); // �����ڵ㲢����ڵ�����
        }
    }
}

/*��ʱ�������������Ѿ���str�ڲ����ֹ�������ASCII��ASC��С����洢��nodes�ڵ���*/

// ����һ���µ�MinHeap
MinHeap *createMinHeap()
{
    MinHeap *heap = (MinHeap *)malloc(sizeof(MinHeap));
    heap->size = 0;
    return heap;
}

// ����MinHeap�������ڵ��λ��
void swapNodes(Node **a, Node **b) // ��ȫ��������ָ��ָ��Ľڵ�
{
    Node *temp = *a;
    *a = *b;
    *b = temp;
}

// �ѻ��ϸ� -- �ڲ����ά���ѵ�����
void heapifyUp(MinHeap *heap, int idx)
{
    // ���㵱ǰ�ڵ�ĸ��ڵ�����
    int parentIdx = (idx - 1) / 2;

    // �����ǰ�ڵ㲻�Ǹ��ڵ㣬���Ҹ��ڵ��Ƶ�ʴ��ڵ�ǰ�ڵ��Ƶ��
    if (idx && heap->nodes[parentIdx]->frequency > heap->nodes[idx]->frequency)
    {
        // ������ǰ�ڵ�͸��ڵ��λ��
        swapNodes(&heap->nodes[parentIdx], &heap->nodes[idx]);

        // �ݹ���öѻ��ϸ�������������鸸�ڵ��Ƿ���Ҫ�����ϸ�
        heapifyUp(heap, parentIdx);
    }
}

// �ѻ��³� -- ����ȡ��ά���ѵ�����
void heapifyDown(MinHeap *heap, int idx)
{
    // ���㵱ǰ�ڵ�����ӽڵ�����ӽڵ������
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    // ��ʼ����С�ڵ�Ϊ��ǰ�ڵ�
    int smallest = idx;

    // ������ӽڵ���ڣ��������ӽڵ��Ƶ��С�ڵ�ǰ��С�ڵ��Ƶ��
    if (left < heap->size && heap->nodes[left]->frequency < heap->nodes[smallest]->frequency)
    {
        // ������С�ڵ�Ϊ���ӽڵ�
        smallest = left;
    }

    // ������ӽڵ���ڣ��������ӽڵ��Ƶ��С�ڵ�ǰ��С�ڵ��Ƶ��
    if (right < heap->size && heap->nodes[right]->frequency < heap->nodes[smallest]->frequency)
    {
        // ������С�ڵ�Ϊ���ӽڵ�
        smallest = right;
    }

    // �����С�ڵ㲻�ǵ�ǰ�ڵ㣬˵����Ҫ����
    if (smallest != idx)
    {
        // ������С�ڵ�͵�ǰ�ڵ�
        swapNodes(&heap->nodes[smallest], &heap->nodes[idx]);
        // �ݹ�ض���С�ڵ�ִ�жѻ��³���������ȷ������Ҳ���������
        heapifyDown(heap, smallest);
    }
}

// ��MinHeap�в���һ���ڵ�
void insertNode(MinHeap *heap, Node *node)
{
    if (heap->size == MAX_CHAR)
    {
        printf("�����\n");
        return;
    }

    heap->nodes[heap->size] = node;
    heap->size++;
    // �������öѻ��ϸ�ά���ѵ�����
    heapifyUp(heap, heap->size - 1);
}

// ��MinHeap����ȡ��СƵ�ʵĽڵ�
Node *extractMin(MinHeap *heap)
{
    if (heap->size <= 0)
    {
        return NULL;
    }

    Node *minNode = heap->nodes[0];
    heap->nodes[0] = heap->nodes[heap->size - 1];
    heap->size--;
    // ��ȡ����öѻ��³�ά���ѵ�����
    heapifyDown(heap, 0);

    return minNode;
}

Node *buildHuffmanTree(MinHeap *heap)
{
    // ֻҪ�����ж���һ���ڵ㣬�ͼ���������
    while (heap->size > 1)
    {
        // ��ȡ����Ƶ����͵������ڵ�
        Node *left = extractMin(heap);
        Node *right = extractMin(heap);

        // ����һ���µ��ڲ��ڵ㣬��Ƶ�ʵ��������ڵ�Ƶ��֮��
        Node *merged = createNode('\0', left->frequency + right->frequency);
        merged->left = left;   // �½ڵ�����ӽڵ���Ƶ�ʽϵ͵Ľڵ�
        merged->right = right; // �½ڵ�����ӽڵ�����һ��Ƶ�ʽϵ͵Ľڵ�

        // ���½ڵ����²������
        insertNode(heap, merged);
    }

    // �������ʣ�µĽڵ���ǻ��������ĸ��ڵ�
    return extractMin(heap);
}

void generateHuffmanCodes(Node *root, char *code, int length, FILE *outputFile)
{
    // ������ڵ�Ϊ�գ�ֱ�ӷ���
    if (root == NULL)
    {
        return;
    }

    // �������һ��Ҷ�ӽڵ㣬�洢���ַ��ı���
    if (root->left == NULL && root->right == NULL)
    {
        code[length] = '\0';                                                  // ��null��ֹ��ǰ����
        strcpy(codes[(unsigned char)root->character], code);                  // ������洢��codes�����У����ַ�Ϊ�����������Ѿ���codesȫ�ֱ�����������ÿһ��ASC�Ĺ���������
        fprintf(outputFile, "%d:%s\n", (unsigned char)root->character, code); // ���ַ�ASC�������������д���ļ�
        return;
    }

    // ���������� - �ڱ����׷��'0'
    code[length] = '0';
    generateHuffmanCodes(root->left, code, length + 1, outputFile);

    // ���������� - �ڱ����׷��'1'
    code[length] = '1';
    generateHuffmanCodes(root->right, code, length + 1, outputFile);
}

void writeEncodedFile(const unsigned char *data, long data_size, const char *outputFilename)
{
    FILE *file = fopen(outputFilename, "wb"); // ���ļ����ж�����д��
    if (!file)
    {
        fprintf(stderr, "д��ʧ��\n");
        return;
    }

    unsigned char buffer = 0; // �����ݴ�λ
    int bitCount = 0;         // ��ǰ�������д洢��λ��

    // �����ֽ�����д��ÿ���ֽڵĹ���������
    for (long i = 0; i < data_size; i++)
    {
        // ��ȡ��ǰ�ֽڵĹ��������봮
        const char *code = codes[data[i]];

        // �������봮��ÿһλ��0 �� 1��
        for (int j = 0; code[j] != '\0'; j++)
        {
            if (code[j] == '1')
            {
                buffer = (buffer << 1) | 1; // �����ƶ�һλ���������λ��Ϊ 1
            }
            else
            {
                buffer = (buffer << 1); // �����ƶ�һλ�����λΪ 0
            }
            bitCount++;

            // ������������� 8 λ����д���ļ�
            if (bitCount == 8)
            {
                fwrite(&buffer, sizeof(unsigned char), 1, file);
                buffer = 0;   // ��ջ�����
                bitCount = 0; // ����λ��������
            }
        }
    }

    // �����󻺳����л���δд����λ������ 8 λ��
    if (bitCount > 0)
    {
        buffer = buffer << (8 - bitCount); // ��ʣ���λ�������
        fwrite(&buffer, sizeof(unsigned char), 1, file);
    }

    fclose(file);
}
