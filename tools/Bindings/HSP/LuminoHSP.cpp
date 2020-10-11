#include "LuminoHSP.h"

bool g_leadSupport = false;

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
	// 引数が省略されているかを確認するには一度 code_getprm() する必要がある
	// （構造体は引数省略で、デフォルトの初期化を使いたい）

	int r = code_getprm();
	g_leadSupport = true;
	return (r == PARAM_ENDSPLIT);	// ')'
}

double fetchVADouble_reffunc()
{
	if (g_leadSupport) {
		g_leadSupport = false;

		// code_getd
		if (mpval->flag != HSPVAR_FLAG_DOUBLE) {
			if (mpval->flag != HSPVAR_FLAG_INT) throw HSPERR_TYPE_MISMATCH;
			return (double)(*(int*)(mpval->pt));		// intの時はdoubleに変換
		}

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

// code_gets() は呼び出すたびに共通のバッファを書き換えるので、戻り値を const char* にすることはできない。
ArgString fetchVAString()
{
	return code_gets();
}

ArgString fetchVAString(const char* defaultValue)
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



std::array<ln::Ref<ln::Variant>, MaxArgs> g_callbackArgs = {};
static std::array<int, MaxArgs> g_callbackArgsBuffer_Int = {};
static std::array<double, MaxArgs> g_callbackArgsBuffer_Double = {};
static std::array<std::string, MaxArgs> g_callbackArgsBuffer_String = {};

// ラベル呼び出しに引数を与えるための関数定義
void ln_args_reffunc(int* typeRes, void** retValPtr)
{
	int index = code_geti();

	const auto& value = g_callbackArgs[index];
	if (value) {
		switch (value->type())
		{
		case ln::VariantType::Null:
			*retValPtr = 0;
			*typeRes = HSPVAR_FLAG_INT;
			break;
		case ln::VariantType::Bool:
			g_callbackArgsBuffer_Int[index] = value->get<bool>() ? 1 : 0;
			*retValPtr = &g_callbackArgsBuffer_Int[index];
			*typeRes = HSPVAR_FLAG_INT;
			break;
		case ln::VariantType::Int:
			g_callbackArgsBuffer_Int[index] = value->get<int>();
			*retValPtr = &g_callbackArgsBuffer_Int[index];
			*typeRes = HSPVAR_FLAG_INT;
			break;
		case ln::VariantType::UInt32:
			g_callbackArgsBuffer_Int[index] = value->get<uint32_t>();
			*retValPtr = &g_callbackArgsBuffer_Int[index];
			*typeRes = HSPVAR_FLAG_INT;
			break;
		case ln::VariantType::Int64:
			g_callbackArgsBuffer_Int[index] = value->get<int64_t>();
			*retValPtr = &g_callbackArgsBuffer_Int[index];
			*typeRes = HSPVAR_FLAG_INT;
			break;
		case ln::VariantType::Float:
			g_callbackArgsBuffer_Double[index] = value->get<float>();
			*retValPtr = &g_callbackArgsBuffer_Double[index];
			*typeRes = HSPVAR_FLAG_DOUBLE;
			break;
		case ln::VariantType::Double:
			g_callbackArgsBuffer_Double[index] = value->get<double>();
			*retValPtr = &g_callbackArgsBuffer_Double[index];
			*typeRes = HSPVAR_FLAG_DOUBLE;
			break;
		case ln::VariantType::String:
			g_callbackArgsBuffer_String[index] = value->get<ln::String>().toStdString();
			*retValPtr = const_cast<char*>(g_callbackArgsBuffer_String[index].data());
			*typeRes = HSPVAR_FLAG_STR;
			break;
		default:
			LN_NOTIMPLEMENTED();
			break;
		}
	}
	else {
		static int nullValue = 0;
		*retValPtr = &nullValue;
		*typeRes = HSPVAR_FLAG_INT;
	}
}

void ln_set_args_cmdfunc()
{
	const int index = fetchVAInt();

	// TODO: いまは int のみ
	const int value = fetchVAInt();

	g_callbackArgs[index] = ln::makeVariant(value);
}
