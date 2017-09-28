#pragma once

#define _APPINFO_ALLOW_MP_MAPS 1

bool AppInfo_Init();
const char* AppInfo_AppDir();
const char* AppInfo_FFDir();
const char* AppInfo_ZoneDir();
const char* AppInfo_IWDDir();
const char* AppInfo_RawDir();
const char* AppInfo_OutDir();
bool AppInfo_MapExists(const char* mapname);
