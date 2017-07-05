#pragma once

typedef int (__cdecl * FS_FOpenFileRead_t)(const char *filename, int *file);
static FS_FOpenFileRead_t FS_FOpenFileRead = (FS_FOpenFileRead_t)0x004BC190;

typedef void (__cdecl * FS_FCloseFile_t)(int h);
static FS_FCloseFile_t FS_FCloseFile = (FS_FCloseFile_t)0x004BB3C0;

typedef int (__cdecl * FS_Read_t)(void *buffer, int len, int h);
static FS_Read_t FS_Read = (FS_Read_t)0x004BC4B0;

typedef int (__cdecl * FS_ReadFile_t)(const char *qpath, void **buffer);
extern FS_ReadFile_t o_FS_ReadFile;

typedef void (__cdecl * FS_FreeFile_t)(void *buffer);
static FS_FreeFile_t FS_FreeFile = (FS_FreeFile_t)0x004BC910;

int __cdecl FS_ReadFile(const char *qpath, void **buffer);

void FS_Init_TechsetOverride(void);

int __cdecl FS_HashFileName(const char *fname, int hashSize);

const char* FS_GetExtensionSubString(const char* filename);
