#include "LuminoHSP.h"

static bool g_leadSupport = false;

void* hspCommon_GetBlockSize(PVal* pval, PDAT* pdat, int* size)
{
	*size = pval->size - (((char*)pdat) - pval->pt);
	return (pdat);
}

void hspCommon_AllocBlock(PVal* pval, PDAT* pdat, int size)
{
}

bool checkAndFetchDefault()
{
	int r = code_getprm();
	g_leadSupport = true;
	return (r == PARAM_ENDSPLIT);	// ')'
}

double fetchVADouble_reffunc()
{
	if (g_leadSupport) {
		g_leadSupport = false;
		return *((double*)mpval->pt);
	}
	else {
		return code_getd();
	}
}

int fetchVAInt()
{
	return code_geti();
}

int fetchVAInt(int defaultValue)
{
	return code_getdi(defaultValue);
}

double fetchVADouble()
{
	return code_getd();
}

double fetchVADouble(double defaultValue)
{
	return code_getdd(defaultValue);
}

const char* fetchVAString()
{
	return code_gets();
}

const char* fetchVAString(const char* defaultValue)
{
	return code_getds(defaultValue);
}

unsigned short* fetchVALabelPointer()
{
	return code_getlb();
}

void* fetchVARawData()
{
	PVal* MemoryArray = exinfo->HspFunc_prm_getpval();
	HspVarProc* vpSTR = exinfo->HspFunc_getproc(HSPVAR_FLAG_STR);

	HspVarCoreReset(MemoryArray);
	exinfo->HspFunc_array(MemoryArray, 0);

	return vpSTR->GetPtr(MemoryArray);
}

void setVAInt(PVal* pval, APTR aptr, int value)
{
	code_setva(pval, aptr, HSPVAR_FLAG_INT, &value);
}

void setVADouble(PVal* pval, APTR aptr, double value)
{
	code_setva(pval, aptr, HSPVAR_FLAG_DOUBLE, &value);
}

void setVAStr(PVal* pval, APTR aptr, const std::string& value)
{
	code_setva(pval, aptr, HSPVAR_FLAG_STR, value.c_str());
}
