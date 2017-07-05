#include "stdafx.h"

struct XModel
{
	const char *name;
	char _pad1[0xE4];
	bool bad;
};
STATIC_ASSERT_OFFSET(XModel, name, 0x0);
STATIC_ASSERT_OFFSET(XModel, bad, 0xE8);

const char *XModelGetName(XModel *model)
{
	ASSERT(model);

	return model->name;
}

int G_ModelIndex(const char *name)
{
	return ((int(__cdecl *)(const char *))0x004294B0)(name);
}

bool XModelBad(XModel *model)
{
	ASSERT(model);

	if (useFastFile->current.enabled)
		return DB_IsXAssetDefault(ASSET_TYPE_XMODEL, model->name);

	return model->bad;
}

void G_OverrideModel(int modelIndex, const char *defaultModelName)
{
	((void(__cdecl *)(int, const char *))0x0068C110)(modelIndex, defaultModelName);
}

int G_GetHintStringIndex(int *piIndex, const char *pszString)
{
	return ((int(__cdecl *)(int *, const char *))0x0065AB40)(piIndex, pszString);
}

// /game/g_items.cpp:1374
void ClearRegisteredItems()
{
	memset(itemRegistered, 0, MAX_WEAPONS * sizeof(int));

	// defaultweapon is always registered
	itemRegistered[0] = 1;
}

// /game/g_items.cpp:1525
void G_RegisterWeapon(unsigned int weapIndex)
{
	ASSERT(!IsItemRegistered(weapIndex));

	itemRegistered[weapIndex] = 1;
	*(DWORD *)0x1C05348 = 1;
	*(DWORD *)0x1C05344 = 1;

	WeaponDef *weapDef = bg_weaponVariantDefs[weapIndex]->weapDef;

	if (*weapDef->szUseHintString && !G_GetHintStringIndex(&weapDef->iUseHintStringIndex, weapDef->szUseHintString))
		Com_Error(ERR_DROP, "Too many different hintstring values on weapons. Max allowed is %i different strings", 96);

	if (*weapDef->dropHintString && !G_GetHintStringIndex(&weapDef->dropHintStringIndex, weapDef->dropHintString))
		Com_Error(ERR_DROP, "Too many different hintstring values on weapons. Max allowed is %i different strings", 96);

	for (int i = 0; i < 16; i++)
	{
		if (!weapDef->worldModel[i])
			continue;

		int modelIndex = G_ModelIndex(XModelGetName(weapDef->worldModel[i]));

		if (XModelBad(weapDef->worldModel[i]))
			G_OverrideModel(modelIndex, "defaultweapon");
	}

	if (weapDef->projectileModel)
		G_ModelIndex(XModelGetName(weapDef->projectileModel));

	if (weapDef->worldModel[1])
		G_ModelIndex(XModelGetName(weapDef->worldModel[1]));

	if (weapDef->additionalMeleeModel)
		G_ModelIndex(XModelGetName(weapDef->additionalMeleeModel));
}

// /game/g_items.cpp:1584
int IsItemRegistered(int iItemIndex)
{
	ASSERT((iItemIndex >= 0) && (iItemIndex < MAX_WEAPONS));

	return itemRegistered[iItemIndex];
}