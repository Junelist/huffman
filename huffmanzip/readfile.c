#include "readfile.h"

char *readFileToBytes(const char *filename, long *file_size)
{
    FILE *file;
    char *buffer;

    // ���ļ�
    file = fopen(filename, "rb");
    if (file == NULL)
    {
        perror("�ļ���ʧ��");
        return NULL;
    }

    // ��ȡ�ļ���С
    fseek(file, 0, SEEK_END);
    *file_size = ftell(file);
    rewind(file);

    // Ϊ�ļ����ݷ����ڴ�
    buffer = (char *)malloc(*file_size);
    if (buffer == NULL)
    {
        perror("�ڴ����ʧ��");
        fclose(file);
        return NULL;
    }

    // ��ȡ�ļ����ݵ�������
    if (fread(buffer, 1, *file_size, file) != *file_size)
    {
        perror("�ļ���ȡʧ��");
        fclose(file);
        free(buffer);
        return NULL;
    }

    fclose(file);
    return buffer;
}
