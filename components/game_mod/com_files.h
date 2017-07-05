#pragma once

struct XFile
{
	unsigned int size;
	unsigned int externalSize;
	unsigned int blockSize[7];
};

enum fsMode_t
{
	FS_READ,
	FS_WRITE,
	FS_APPEND,
	FS_APPEND_SYNC,
};

enum DESC_TYPE
{
	DESC_ERROR = 0,
	DESC_DESC = 1,
	DESC_JSON = 2,
};

#define MODDESC_LEN 254
#define DIRLIST_LEN 8192 * (MODDESC_LEN / 48)

extern char dirList[DIRLIST_LEN];

struct FS_ModDesc
{
	// The size of the description data buffer
	// Must be MODDESC_LEN minus the size of 'type'
	static const unsigned int bufLen = MODDESC_LEN - 1;

	BYTE type;
	char data[bufLen];

	FS_ModDesc(void);
	void Clear(void);

	// Logical length
	size_t Length(void) const;
};
STATIC_ASSERT_SIZE(FS_ModDesc, MODDESC_LEN);

VANILLA_FUNC(FS_BuildOSPath, void (__cdecl*)(const char *base, const char *game, const char *qpath, char *ospath), 0x00472030);
VANILLA_FUNC(FS_OSFPathExists, int (__cdecl*)(const char *file), 0x0046C380);

typedef int (__cdecl* FS_FOpenFileRead_t)(const char *filename, int *file);
static FS_FOpenFileRead_t FS_FOpenFileRead = (FS_FOpenFileRead_t)0x004C6E20;

typedef int (__cdecl* FS_FOpenFileByMode_t)(const char *qpath, int *f, fsMode_t mode);
static FS_FOpenFileByMode_t FS_FOpenFileByMode = (FS_FOpenFileByMode_t)0x004DD530;

typedef int (__cdecl* FS_FOpenTextFileWrite_t)(const char *filename);
static FS_FOpenTextFileWrite_t FS_FOpenTextFileWrite = (FS_FOpenTextFileWrite_t)0x004483C0;

typedef void (__cdecl* FS_FCloseFile_t)(int h);
static FS_FCloseFile_t FS_FCloseFile = (FS_FCloseFile_t)0x0046CAA0;

typedef int (__cdecl* FS_Write_t)(const void *buffer, int len, int h);
static FS_Write_t FS_Write = (FS_Write_t)0x00401090;

typedef int (__cdecl* FS_Read_t)(void *buffer, int len, int h);
static FS_Read_t FS_Read = (FS_Read_t)0x004CFB60;

typedef unsigned int (__cdecl* FS_FileRead_t)(void *ptr, unsigned int len, struct _iobuf *stream);
static FS_FileRead_t FS_FileRead = (FS_FileRead_t)0x0047F210;

VANILLA_FUNC(FS_WriteToDemo, int (__cdecl*)(const void *buffer, int len, int h), 0x00470F40);

// If pResult is NULL, no result value is written
typedef int(__cdecl* FS_SV_FOpenFileRead_t)(const char *filename, const char *dir, int *fp, int* pResult);
static FS_SV_FOpenFileRead_t FS_SV_FOpenFileRead = (FS_SV_FOpenFileRead_t)0x00464020;

VANILLA_FUNC(FS_SV_FOpenFileWrite, int(__cdecl*)(const char *filename, const char *dir), 0x00472DC0);

typedef _iobuf *(__cdecl* FS_FileForHandle_t)(int f);
static FS_FileForHandle_t FS_FileForHandle = (FS_FileForHandle_t)0x00516DA0;

typedef char **(__cdecl* Sys_ListFiles_t)(const char *directory, const char *extension, const char *filter, int *numfiles, int wantsubs);
static Sys_ListFiles_t Sys_ListFiles = (Sys_ListFiles_t)0x00690E40;

typedef void (__cdecl* FS_FreeFileList_t)(const char **list, int allocTrackType);
static FS_FreeFileList_t FS_FreeFileList = (FS_FreeFileList_t)0x0067DFE0;

unsigned int __cdecl FS_ReadModDescription(void *ptr, unsigned int len, struct _iobuf *stream);

int __cdecl FS_GetModList(char *listbuf, int bufsize);

//
// Used only by Com_WriteConfigToFile and Com_WriteKeyConfigToFile
//
int __cdecl FS_FOpenFileWriteToDir(const char *filename, const char *dir, const char *osbasepath);
