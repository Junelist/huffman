#include "readfile.h"

char *readFileToBytes(const char *filename, long *file_size)
{
    FILE *file;
    char *buffer;

    // 打开文件
    file = fopen(filename, "rb");
    if (file == NULL)
    {
        perror("文件打开失败");
        return NULL;
    }

    // 获取文件大小
    fseek(file, 0, SEEK_END);
    *file_size = ftell(file);
    rewind(file);

    // 为文件内容分配内存
    buffer = (char *)malloc(*file_size);
    if (buffer == NULL)
    {
        perror("内存分配失败");
        fclose(file);
        return NULL;
    }

    // 读取文件内容到缓冲区
    if (fread(buffer, 1, *file_size, file) != *file_size)
    {
        perror("文件读取失败");
        fclose(file);
        free(buffer);
        return NULL;
    }

    fclose(file);
    return buffer;
}
