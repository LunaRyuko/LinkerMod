#include "stdafx.h"

#ifdef CEG_DEBUG
#define CEG_DEBUG_PRINT Log::Write("Region: 0x%X | Redirected: 0x%X | Return: 0x%X", address, dwNew, _ReturnAddress());
#else
#define CEG_DEBUG_PRINT
#endif

typedef void(__thiscall * sub_5CBF00_t)(void *thisptr, DWORD address, size_t scanSize);
sub_5CBF00_t sub_5CBF00_o;

typedef void(__thiscall * sub_661450_t)(void *thisptr, DWORD address, size_t scanSize);
sub_661450_t sub_661450_o;

typedef void(__thiscall * sub_4E9880_t)(void *thisptr, DWORD address, size_t scanSize);
sub_4E9880_t sub_4E9880_o;

typedef void(__thiscall * sub_640020_t)(void *thisptr, DWORD address, size_t scanSize);
sub_640020_t sub_640020_o;

typedef void(__thiscall * sub_51A140_t)(void *thisptr, DWORD address, size_t scanSize);
sub_51A140_t sub_51A140_o;

typedef void(__thiscall * sub_54A430_t)(void *thisptr, DWORD address, size_t scanSize);
sub_51A140_t sub_54A430_o;

LPVOID g_MemoryBuffer;
ULONG_PTR g_ImageBase;
ULONG_PTR g_ImageEnd;
ULONG_PTR g_ImageCodeSize;

void Patch_CEG()
{
	g_ImageBase = (ULONG_PTR)GetModuleHandle(nullptr);
	g_ImageCodeSize = 0x5A1C00;
	g_ImageEnd = g_ImageBase + g_ImageCodeSize;

	//g_MemoryBuffer = VirtualAlloc(nullptr, g_ImageCodeSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	//memcpy(g_MemoryBuffer, (LPVOID)g_ImageBase, g_ImageCodeSize);

	sub_5CBF00_o = (sub_5CBF00_t)Detours::X86::DetourFunction((PBYTE)0x5CBF00, (PBYTE)&sub_5CBF00);
	sub_661450_o = (sub_661450_t)Detours::X86::DetourFunction((PBYTE)0x661450, (PBYTE)&sub_661450);
	sub_4E9880_o = (sub_4E9880_t)Detours::X86::DetourFunction((PBYTE)0x4E9880, (PBYTE)&sub_4E9880);
	sub_640020_o = (sub_640020_t)Detours::X86::DetourFunction((PBYTE)0x640020, (PBYTE)&sub_640020);
	sub_51A140_o = (sub_51A140_t)Detours::X86::DetourFunction((PBYTE)0x51A140, (PBYTE)&sub_51A140);
	sub_54A430_o = (sub_51A140_t)Detours::X86::DetourFunction((PBYTE)0x54A430, (PBYTE)&sub_54A430);

	Detours::X86::DetourFunction((PBYTE)0x967760, (PBYTE)&hk_memcpy);
	Detours::X86::DetourFunction((PBYTE)0x8EF04F, (PBYTE)&hk_inline_memcpy);
	Detours::X86::DetourFunction((PBYTE)0x8EF168, (PBYTE)&hk_inline_memcpy2);

	PatchMemory(0x00662F20, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);	// Kill Steam DRM IPC creation
	PatchMemory(0x0046C9A0, (PBYTE)"\xCC", 1);
	PatchMemory(0x005F3290, (PBYTE)"\xCC", 1);

	PatchMemory(0x0064F6A0, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);	// Single function, 32bit hash check
	PatchMemory(0x005614A0, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x00417360, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x0056AB40, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x0059BEB0, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);

	PatchMemory(0x00676740, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);	// Direct ExitProcess() check
	PatchMemory(0x005DB020, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x004F02C0, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x00572DF0, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x00679B40, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x004BFB50, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x004D4B80, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x00501080, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x005CAB50, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x004C0DE0, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);

	PatchMemory(0x0041CEB0, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);	// Wrapper ExitProcess() check, executes
	PatchMemory(0x0041CF50, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);	// on certain days of the week
	PatchMemory(0x00427E00, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x00437350, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x00444E80, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x00449C30, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x004545A0, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x0045C550, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x00462B60, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x004682B0, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x00487D80, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x0048C7B0, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x004A9970, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x004C3360, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x004D9650, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x004E3860, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x00515B20, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x005268E0, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x00527200, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x005474A0, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x0054F280, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x00561410, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x0059D820, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x005A8800, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x005B4330, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x005D3F20, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x005EF2A0, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x006099E0, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x00610A60, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x00625500, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x00625980, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x0064FFB0, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x00662EC0, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x00662F80, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x00671860, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x0067B3B0, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x0067D830, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x006857B0, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x0068D300, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x0068E8F0, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);

	PatchMemory(0x009A23B0, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);	// Wrapper ExitProcess() check with HWBP detection,
	PatchMemory(0x009A23F0, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);	// executes on certain days of the week
	PatchMemory(0x009A2430, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x009A2470, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x009A24B0, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x009A24F0, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x009A2530, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x009A2570, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x009A25B0, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x009A25F0, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x009A2630, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x009A2670, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x009A26B0, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x009A26F0, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x009A2730, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x009A2770, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);

	PatchMemory(0x009A2980, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);	// Direct HWBP check
	PatchMemory(0x009A29B0, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);
	PatchMemory(0x009A29E0, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6);

	Detours::X86::DetourFunction((PBYTE)0x8EE640, (PBYTE)&sub_8EE640);	// Patch intentional nullptr

	FixupFunction(0x0060CC10, 0x004F20F0);		// CEGObfuscate<LiveStats_Init> => LiveStats_Init
	FixupFunction(0x00580460, 0x0079E6D0);		// CEGObfuscate<Con_Restricted_SetLists> => Con_Restricted_SetLists
}

DWORD __declspec(noinline) GetNewAddress(DWORD dwOld)
{
	if (dwOld > g_ImageBase && dwOld < g_ImageEnd)
	{
		__debugbreak();
		return ((DWORD)g_MemoryBuffer + (dwOld - g_ImageBase));
	}

	return dwOld;
}

void *hk_memcpy(void *dest, const void *src, size_t size)
{
	PVOID dwNew = (PVOID)GetNewAddress((DWORD)src);

	return memcpy(dest, dwNew, size);
}

void __declspec(naked) hk_inline_memcpy()
{
	static DWORD dwCall = 0x8EE910;
	static DWORD dwJmp = 0x8EF056;

	__asm
	{
		push eax
		push ecx
		push edx

		push esi
		call GetNewAddress
		add esp, 4
		mov esi, eax

		pop edx
		pop ecx
		pop eax

		rep movs dword ptr es : [edi], dword ptr ds : [esi]
		call DWORD PTR ds : [dwCall]
		jmp DWORD PTR ds : [dwJmp]
	}
}

void __declspec(naked) hk_inline_memcpy2()
{
	static DWORD dwJmp = 0x8EF16E;

	__asm
	{
		push eax
		push ecx
		push edx

		push esi
		call GetNewAddress
		add esp, 4
		mov esi, eax

		pop edx
		pop ecx
		pop eax

		rep movs dword ptr es : [edi], dword ptr ds : [esi]
		mov ecx, dword ptr ss : [esp + 0x24]
		jmp DWORD PTR ds : [dwJmp]
	}
}

void *sub_8EE640(void *Nullptr1, void *Nullptr2)
{
	__debugbreak();

	if (Nullptr1 != nullptr || Nullptr2 != nullptr)
		__debugbreak();

	*(void **)0xBA1C24 = Nullptr2;
	return (void *)0xBA1C24;
}

void __fastcall sub_5CBF00(void *thisptr, PVOID _EDX, DWORD address, size_t scanSize)
{
	DWORD dwNew = GetNewAddress(address);

	CEG_DEBUG_PRINT;

	sub_5CBF00_o(thisptr, dwNew, scanSize);
}

void __fastcall sub_661450(void *thisptr, PVOID _EDX, DWORD address, size_t scanSize)
{
	DWORD dwNew = GetNewAddress(address);

	CEG_DEBUG_PRINT;

	sub_661450_o(thisptr, dwNew, scanSize);
}

void __fastcall sub_4E9880(void *thisptr, PVOID _EDX, DWORD address, size_t scanSize)
{
	DWORD dwNew = GetNewAddress(address);

	CEG_DEBUG_PRINT;

	sub_4E9880_o(thisptr, dwNew, scanSize);
}

void __fastcall sub_640020(void *thisptr, PVOID _EDX, DWORD address, size_t scanSize)
{
	DWORD dwNew = GetNewAddress(address);

	CEG_DEBUG_PRINT;

	sub_640020_o(thisptr, dwNew, scanSize);
}

void __fastcall sub_51A140(void *thisptr, PVOID _EDX, DWORD address, size_t scanSize)
{
	DWORD dwNew = GetNewAddress(address);

	CEG_DEBUG_PRINT;

	sub_51A140_o(thisptr, dwNew, scanSize);
}

void __fastcall sub_54A430(void *thisptr, PVOID _EDX, DWORD address, size_t scanSize)
{
	DWORD dwNew = GetNewAddress(address);

	CEG_DEBUG_PRINT;

	sub_54A430_o(thisptr, dwNew, scanSize);
}