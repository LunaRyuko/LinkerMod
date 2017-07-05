#pragma once

#define HUNK_MAX_ALIGNMENT 4096

void *Hunk_Alloc(int size, const char *name, int type);
void *Hunk_AllocAlign(int size, int alignment, const char *name, int type);
void Z_Free(void *ptr, int type);
void *Z_Malloc(int size, const char *name, int type);