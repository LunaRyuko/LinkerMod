#pragma once

#pragma comment(lib, "detours.lib")
#include "../shared/detours/Detours.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>

#define USE_LEGACY_HDR 0

//
// Shared files
//
#include "../shared/shared_utility.h"
#include "../shared/shared_version.h"

#include "PageGuard.h"
#include "print.h"
#include "threading.h"

#include "arguments.h"
#include "vector.h"
#include "hdr.h"

#include "lighting.h"

#include "r_lightmaps.h"
#include "r_lightgrid.h"
#include "r_xmodel_load_obj.h"
#include "r_xsurface_load_obj.h"

#include "common.h"
#include "com_memory.h"
#include "com_files.h"
#include "com_math.h"

#include "com_bsp_load_obj.h"
#include "r_light_load_obj.h"
#include "r_xmodel_load_obj.h"
#include "r_xsurface_load_obj.h"

#include "../D3DBSP_Lib/D3DBSP_Lib/D3DBSP_Lib.h"
#include "../D3DBSP_Lib/D3DBSP_Lib/Lightmaps.h"
#include "../D3DBSP_Lib/D3DBSP_Lib/LightGridColors.h"
#if _DEBUG
#pragma comment(lib, "../../build/Debug/D3DBSP_Lib.lib")
#else
#pragma comment(lib, "../../build/Release/D3DBSP_Lib.lib")
#endif

#define VANILLA_VALUE(NAME, TYPE, ADDRESS) static TYPE& NAME = *(TYPE*)ADDRESS;

using namespace D3DBSP_Lib;
