#ifndef _READFILE_H
#define _READFILE_H

#include <stdio.h>
#include <stdlib.h>

char *readFileToBytes(const char *filename, long *file_size);
#endif // _READFILE_H