#ifndef FILE_MANAGEMENT_H
#define FILE_MANAGEMENT_H

#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

char* get_file_content(const char* file_path);

#endif