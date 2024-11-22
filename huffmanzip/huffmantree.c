#include "huffmantree.h"

int ASC[MAX_CHAR] = {0};        // ASC码表
int nodeCount = 0;              // 计数器用来跟踪节点数量
Node *nodes[MAX_CHAR];          // 节点地址数组,用来暂存节点
char codes[MAX_CHAR][MAX_CHAR]; // 用来存储每个字符的编码

void byte_times(const unsigned char *data, long data_size)
{
    for (long i = 0; i < data_size; i++)
    {
        ASC[data[i]]++; // 统计每个字节的出现次数
    }
}

Node *createNode(char character, int frequency)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->character = character;
    node->frequency = frequency;

    // 初始化左右子节点指针为NULL
    node->left = NULL;
    node->right = NULL;

    return node;
}

void set_nodes()
{
    for (int i = 0; i < MAX_CHAR; i++)
    {
        if (ASC[i] > 0) // 如果字符在ASC码表中出现
        {
            nodes[nodeCount++] = createNode((char)i, ASC[i]); // 创建节点并加入节点数组
        }
    }
}

/*此时函数至此我们已经将str内部出现过的任意ASCII按ASC由小到大存储到nodes节点中*/

// 创建一个新的MinHeap
MinHeap *createMinHeap()
{
    MinHeap *heap = (MinHeap *)malloc(sizeof(MinHeap));
    heap->size = 0;
    return heap;
}

// 交换MinHeap中两个节点的位置
void swapNodes(Node **a, Node **b) // 完全交换两个指针指向的节点
{
    Node *temp = *a;
    *a = *b;
    *b = temp;
}

// 堆化上浮 -- 在插入后维护堆的属性
void heapifyUp(MinHeap *heap, int idx)
{
    // 计算当前节点的父节点索引
    int parentIdx = (idx - 1) / 2;

    // 如果当前节点不是根节点，并且父节点的频率大于当前节点的频率
    if (idx && heap->nodes[parentIdx]->frequency > heap->nodes[idx]->frequency)
    {
        // 交换当前节点和父节点的位置
        swapNodes(&heap->nodes[parentIdx], &heap->nodes[idx]);

        // 递归调用堆化上浮函数，继续检查父节点是否需要继续上浮
        heapifyUp(heap, parentIdx);
    }
}

// 堆化下沉 -- 在提取后维护堆的属性
void heapifyDown(MinHeap *heap, int idx)
{
    // 计算当前节点的左子节点和右子节点的索引
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    // 初始化最小节点为当前节点
    int smallest = idx;

    // 如果左子节点存在，并且左子节点的频率小于当前最小节点的频率
    if (left < heap->size && heap->nodes[left]->frequency < heap->nodes[smallest]->frequency)
    {
        // 更新最小节点为左子节点
        smallest = left;
    }

    // 如果右子节点存在，并且右子节点的频率小于当前最小节点的频率
    if (right < heap->size && heap->nodes[right]->frequency < heap->nodes[smallest]->frequency)
    {
        // 更新最小节点为右子节点
        smallest = right;
    }

    // 如果最小节点不是当前节点，说明需要调整
    if (smallest != idx)
    {
        // 交换最小节点和当前节点
        swapNodes(&heap->nodes[smallest], &heap->nodes[idx]);
        // 递归地对最小节点执行堆化下沉操作，以确保子树也满足堆属性
        heapifyDown(heap, smallest);
    }
}

// 向MinHeap中插入一个节点
void insertNode(MinHeap *heap, Node *node)
{
    if (heap->size == MAX_CHAR)
    {
        printf("堆溢出\n");
        return;
    }

    heap->nodes[heap->size] = node;
    heap->size++;
    // 插入后调用堆化上浮维护堆的属性
    heapifyUp(heap, heap->size - 1);
}

// 从MinHeap中提取最小频率的节点
Node *extractMin(MinHeap *heap)
{
    if (heap->size <= 0)
    {
        return NULL;
    }

    Node *minNode = heap->nodes[0];
    heap->nodes[0] = heap->nodes[heap->size - 1];
    heap->size--;
    // 提取后调用堆化下沉维护堆的属性
    heapifyDown(heap, 0);

    return minNode;
}

Node *buildHuffmanTree(MinHeap *heap)
{
    // 只要堆中有多于一个节点，就继续构建树
    while (heap->size > 1)
    {
        // 提取堆中频率最低的两个节点
        Node *left = extractMin(heap);
        Node *right = extractMin(heap);

        // 创建一个新的内部节点，其频率等于两个节点频率之和
        Node *merged = createNode('\0', left->frequency + right->frequency);
        merged->left = left;   // 新节点的左子节点是频率较低的节点
        merged->right = right; // 新节点的右子节点是另一个频率较低的节点

        // 将新节点重新插入堆中
        insertNode(heap, merged);
    }

    // 堆中最后剩下的节点就是霍夫曼树的根节点
    return extractMin(heap);
}

void generateHuffmanCodes(Node *root, char *code, int length, FILE *outputFile)
{
    // 如果根节点为空，直接返回
    if (root == NULL)
    {
        return;
    }

    // 如果这是一个叶子节点，存储该字符的编码
    if (root->left == NULL && root->right == NULL)
    {
        code[length] = '\0';                                                  // 以null终止当前代码
        strcpy(codes[(unsigned char)root->character], code);                  // 将编码存储到codes数组中，以字符为索引，这里已经再codes全局变量中载入了每一个ASC的哈夫曼编码
        fprintf(outputFile, "%d:%s\n", (unsigned char)root->character, code); // 将字符ASC及其霍夫曼编码写入文件
        return;
    }

    // 遍历左子树 - 在编码后追加'0'
    code[length] = '0';
    generateHuffmanCodes(root->left, code, length + 1, outputFile);

    // 遍历右子树 - 在编码后追加'1'
    code[length] = '1';
    generateHuffmanCodes(root->right, code, length + 1, outputFile);
}

void writeEncodedFile(const unsigned char *data, long data_size, const char *outputFilename)
{
    FILE *file = fopen(outputFilename, "wb"); // 打开文件进行二进制写入
    if (!file)
    {
        fprintf(stderr, "写入失败\n");
        return;
    }

    unsigned char buffer = 0; // 用于暂存位
    int bitCount = 0;         // 当前缓冲区中存储的位数

    // 遍历字节流，写入每个字节的哈夫曼编码
    for (long i = 0; i < data_size; i++)
    {
        // 获取当前字节的哈夫曼编码串
        const char *code = codes[data[i]];

        // 遍历编码串的每一位（0 或 1）
        for (int j = 0; code[j] != '\0'; j++)
        {
            if (code[j] == '1')
            {
                buffer = (buffer << 1) | 1; // 向左移动一位，并将最低位设为 1
            }
            else
            {
                buffer = (buffer << 1); // 向左移动一位，最低位为 0
            }
            bitCount++;

            // 如果缓冲区满了 8 位，则写入文件
            if (bitCount == 8)
            {
                fwrite(&buffer, sizeof(unsigned char), 1, file);
                buffer = 0;   // 清空缓冲区
                bitCount = 0; // 重置位数计数器
            }
        }
    }

    // 如果最后缓冲区中还有未写满的位（少于 8 位）
    if (bitCount > 0)
    {
        buffer = buffer << (8 - bitCount); // 将剩余的位向左对齐
        fwrite(&buffer, sizeof(unsigned char), 1, file);
    }

    fclose(file);
}
