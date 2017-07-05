#include "stdafx.h"

SRCLINE(309)
void Image_Release(GfxImage *image)
{
	((void(__cdecl *)(GfxImage *))0x0052ABC0)(image);
}

SRCLINE(615)
unsigned int Image_GetUsage(int imageFlags, D3DFORMAT imageFormat)
{
	if (imageFlags & 0x20000)
	{
		if (imageFormat != D3DFMT_D24S8
			&& imageFormat != D3DFMT_D24X8
			&& imageFormat != D3DFMT_D16)
			return D3DUSAGE_RENDERTARGET;

		return D3DUSAGE_DEPTHSTENCIL;
	}

	if (imageFlags & 0x10000)
		return D3DUSAGE_DYNAMIC;

	return 0;
}

SRCLINE(642)
void Image_Create2DTexture_PC(GfxImage *image, unsigned __int16 width, unsigned __int16 height, int mipmapCount, int imageFlags, D3DFORMAT imageFormat)
{
	ASSERT(image != nullptr);
	ASSERT(!image->texture.basemap);
	// ASSERT(Sys_IsRenderThread());

	image->width	= width;
	image->height	= height;
	image->depth	= 1;
	image->mapType	= 3;

	D3DPOOL memPool	= D3DPOOL_DEFAULT;
	DWORD usage		= Image_GetUsage(imageFlags, imageFormat);

	if (imageFlags & 0x40000 || imageFlags & 0x100)
		memPool = D3DPOOL_SYSTEMMEM;
	else
		memPool = (usage == 0) ? D3DPOOL_MANAGED : D3DPOOL_DEFAULT;

	// D3D9Ex does not allow D3DPOOL_MANAGED
	if (IsD3D9ExAvailable())
	{
		usage	= (usage == 0) ? D3DUSAGE_DYNAMIC : usage;
		memPool = (memPool == D3DPOOL_MANAGED) ? D3DPOOL_DEFAULT : memPool;
	}

	HRESULT hr = dx_device->CreateTexture(width, height, mipmapCount, usage, imageFormat, memPool, &image->texture.map, nullptr);

	if (FAILED(hr))
	{
		g_disableRendering++;
		Com_Error(ERR_FATAL, "dx.device->CreateTexture(width, height, mipmapCount, usage, imageFormat, memPool, &image->texture.map, nullptr) failed: %s\n", R_ErrorDescription(hr));
	}
}

SRCLINE(696)
void Image_Create3DTexture_PC(GfxImage *image, unsigned __int16 width, unsigned __int16 height, unsigned __int16 depth, int mipmapCount, int imageFlags, D3DFORMAT imageFormat)
{
	ASSERT(image != nullptr);
	ASSERT(!image->texture.basemap);
	// ASSERT(Sys_IsRenderThread());

	image->width	= width;
	image->height	= height;
	image->depth	= depth;
	image->mapType	= 4;

	// D3D9Ex does not allow D3DPOOL_MANAGED
	DWORD usage		= (IsD3D9ExAvailable()) ? D3DUSAGE_DYNAMIC : 0;
	D3DPOOL memPool = (IsD3D9ExAvailable()) ? D3DPOOL_DEFAULT : D3DPOOL_MANAGED;

	HRESULT hr = dx_device->CreateVolumeTexture(width, height, depth, mipmapCount, usage, imageFormat, memPool, &image->texture.volmap, nullptr);

	if (FAILED(hr))
	{
		g_disableRendering++;
		Com_Error(ERR_FATAL, "dx.device->CreateVolumeTexture(width, height, depth, mipmapCount, usage, imageFormat, memPool, &image->texture.volmap, nullptr) failed: %s\n", R_ErrorDescription(hr));
	}
}

SRCLINE(738)
void Image_CreateCubeTexture_PC(GfxImage *image, unsigned __int16 edgeLen, int mipmapCount, D3DFORMAT imageFormat)
{
	ASSERT(image != nullptr);
	ASSERT(!image->texture.basemap);
	// ASSERT(Sys_IsRenderThread());

	image->width	= edgeLen;
	image->height	= edgeLen;
	image->depth	= 1;
	image->mapType	= 5;

	// D3DDeviceCaps support for mipping
	if (!r_supportCubedMipMaps)
		mipmapCount = 1;

	// D3D9Ex does not allow D3DPOOL_MANAGED
	DWORD usage		= (IsD3D9ExAvailable()) ? D3DUSAGE_DYNAMIC : 0;
	D3DPOOL memPool = (IsD3D9ExAvailable()) ? D3DPOOL_DEFAULT : D3DPOOL_MANAGED;

	HRESULT hr = dx_device->CreateCubeTexture(edgeLen, mipmapCount, usage, imageFormat, memPool, &image->texture.cubemap, nullptr);
	
	if (FAILED(hr))
	{
		g_disableRendering++;
		Com_Error(ERR_FATAL, "dx.device->CreateCubeTexture(edgeLen, mipmapCount, usage, imageFormat, memPool, &image->texture.cubemap, nullptr) failed: %s\n", R_ErrorDescription(hr));
	}
}

SRCLINE(792)
void Image_PicmipForSemantic(char semantic, Picmip *picmip)
{
	signed int picmipUsed = 0;

	switch (semantic)
	{
	default:
		ASSERT_MSG(false, "Unhandled case");
	case 0:
	case 1:
		picmip->platform[0] = 0;
		picmip->platform[1] = 0;
		return;

	case 2:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
	case 25:
	case 26:
	case 27:
	case 28:
		picmipUsed = *(DWORD *)0x1567F28;
		break;

	case 5:
		picmipUsed = *(DWORD *)0x1567F2C;
		break;

	case 8:
		picmipUsed = *(DWORD *)0x1567F30;
		break;
	}

	picmip->platform[1] = 2;

	if (picmipUsed >= 0)
	{
		if (picmipUsed > 3)
			picmipUsed = 3;
	}
	else
	{
		picmipUsed = 0;
	}

	picmip->platform[0] = picmipUsed;
}

SRCLINE(1401)
void Image_Reload(GfxImage *image)
{
	ASSERT(image != nullptr);

	Image_Release(image);

	if (!Image_LoadFromFile(image))
		Com_PrintError(8, "failed to load image '%s'\n", image->name);
}