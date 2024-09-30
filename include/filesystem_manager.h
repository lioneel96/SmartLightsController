#ifndef FILESYSTEM_MANAGER_H
#define FILESYSTEM_MANAGER_H

#include <stdint.h>

bool mountFilesystem(void);
void listDir(const char * dirname, uint8_t levels);
void listFilesInRootDirectory(void);

#endif