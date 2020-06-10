#pragma once

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "hsp3plugin/hsp3plugin.h"

#define sbAlloc hspmalloc
#define sbFree hspfree

#include <LuminoEngine/Runtime/Lumino.FlatC.generated.h>

void* hspCommon_GetBlockSize(PVal* pval, PDAT* pdat, int* size);
void hspCommon_AllocBlock(PVal* pval, PDAT* pdat, int size);

bool checkAndFetchDefault();
double fetchVADouble_reffunc();

int fetchVAInt();
int fetchVAInt(int defaultValue);
double fetchVADouble();
double fetchVADouble(double defaultValue);
const char* fetchVAString();
const char* fetchVAString(const char* defaultValue);
void* fetchVARawData();
unsigned short* fetchVALabelPointer();

void setVAInt(PVal* pval, APTR aptr, int value);
void setVADouble(PVal* pval, APTR aptr, double value);
void setVAStr(PVal* pval, APTR aptr, const std::string& value);

#define CodeGetVA_TypeChecked(ppval, type) \
	code_getva(ppval); \
	if ((*ppval)->flag != hsp##type##_typeid()) { throw HSPVAR_ERROR_TYPEMISS; }