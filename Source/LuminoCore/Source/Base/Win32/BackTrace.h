
#pragma once

#define NOMINMAX
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#pragma warning( push )
#pragma warning ( disable : 4091 )	// VS2015 で dbghelp.h が標準ヘッダなのに警告出すのが気になるので。
#include <dbghelp.h>
#pragma warning( pop )

#ifdef LN_DEBUG
#else
#define LN_BACKTRACE_LOG_HIDE	1
#endif

LN_NAMESPACE_BEGIN

class BackTrace
{
private:

	//dll読み込みヘルパー
	class LoadLibraryHelper
	{
	private:
		HMODULE DllInstance;

	public:
		LoadLibraryHelper()
		{
			this->DllInstance = NULL;
		}
		virtual ~LoadLibraryHelper()
		{
			if (this->DllInstance != NULL)
			{
				::FreeLibrary(this->DllInstance);
				this->DllInstance = NULL;
			}
		}
		bool Load(const char* inDLLName)
		{
			if (this->DllInstance != NULL)
			{
				return false;
			}
			this->DllInstance = ::LoadLibraryA(inDLLName);
			return this->DllInstance != NULL;
		}
		FARPROC GetProcAddress(const char* inProcName)
		{
			if (!this->DllInstance)
			{
				return NULL;
			}
			return ::GetProcAddress(this->DllInstance, inProcName);
		}
	};

private:

	HANDLE	m_process;
	bool	m_isSymbolEngineReady;
	LoadLibraryHelper	mKernel32Librsry;
	LoadLibraryHelper	mDbgHelpLibrsry;

	typedef USHORT(WINAPI *RtlCaptureStackBackTraceDef) (ULONG FramesToSkip, ULONG FramesToCapture, PVOID *BackTrace, /*__out_opt*/ PULONG BackTraceHash);
	RtlCaptureStackBackTraceDef mRtlCaptureStackBackTraceProc;

#if (_WIN64 || __x86_64__)
	//アドレスからモジュールを求める
	typedef BOOL(WINAPI *SymGetModuleInfo64Def) (HANDLE hProcess, DWORD64 dwAddr, PIMAGEHLP_MODULE64 ModuleInfo);
	SymGetModuleInfo64Def mSymGetModuleInfo64Proc;
	//アドレスからシンボルを求める
	typedef BOOL(WINAPI *SymGetSymFromAddr64Def) (HANDLE hProcess, DWORD64 Address, PDWORD64 Displacement, PIMAGEHLP_SYMBOL64 Symbol);
	SymGetSymFromAddr64Def mSymGetSymFromAddr64Proc;
	//アドレスからファイルと行番号を求める
	typedef BOOL(WINAPI *SymGetLineFromAddr64Def) (HANDLE hProcess, DWORD64 Address, PDWORD64 Displacement, PIMAGEHLP_LINE64 Line);
	SymGetLineFromAddr64Def mSymGetLineFromAddr64Proc;
#else
	//アドレスからモジュールを求める
	typedef BOOL(WINAPI *SymGetModuleInfoDef) (HANDLE hProcess, DWORD dwAddr, PIMAGEHLP_MODULE ModuleInfo);
	SymGetModuleInfoDef mSymGetModuleInfoProc;
	//アドレスからシンボルを求める
	typedef BOOL(WINAPI *SymGetSymFromAddrDef) (HANDLE hProcess, DWORD Address, PDWORD Displacement, PIMAGEHLP_SYMBOL Symbol);
	SymGetSymFromAddrDef mSymGetSymFromAddrProc;
	//アドレスからファイルと行番号を求める
	typedef BOOL(WINAPI *SymGetLineFromAddrDef) (HANDLE hProcess, DWORD dwAddr, PDWORD pdwDisplacement, PIMAGEHLP_LINE Line);
	SymGetLineFromAddrDef mSymGetLineFromAddrProc;
#endif
	//シンボルエンジンのオプション
	typedef BOOL(WINAPI *SymSetOptionsDef) (DWORD SymOptions);
	SymSetOptionsDef mSymSetOptionsProc;

	//シンボルエンジンの初期化
	typedef BOOL(WINAPI *SymInitializeDef) (HANDLE hProcess, PSTR UserSearchPath, BOOL fInvadeProcess);
	SymInitializeDef mSymInitializeProc;

	//シンボルエンジンの終了
	typedef BOOL(WINAPI *SymCleanupDef) (HANDLE hProcess);
	SymCleanupDef mSymCleanupProc;

public:

	static const BackTrace* GetInstance()
	{
		static BackTrace s;
		return &s;
	}

	BackTrace()
	{
		m_isSymbolEngineReady = false;

		// プロセスを記録
		m_process = ::GetCurrentProcess();

		if (!mKernel32Librsry.Load("kernel32.dll")) {
			return;
		}
		if (!mDbgHelpLibrsry.Load("dbghelp.dll")) {
			return;
		}

		mRtlCaptureStackBackTraceProc = (RtlCaptureStackBackTraceDef)mKernel32Librsry.GetProcAddress("RtlCaptureStackBackTrace");
#if (_WIN64 || __x86_64__)
		mSymGetModuleInfo64Proc = (SymGetModuleInfo64Def)mDbgHelpLibrsry.GetProcAddress("SymGetModuleInfo64");
		mSymGetSymFromAddr64Proc = (SymGetSymFromAddr64Def)mDbgHelpLibrsry.GetProcAddress("SymGetSymFromAddr64");
		mSymGetLineFromAddr64Proc = (SymGetLineFromAddr64Def)mDbgHelpLibrsry.GetProcAddress("SymGetLineFromAddr64");
#else
		mSymGetModuleInfoProc = (SymGetModuleInfoDef)mDbgHelpLibrsry.GetProcAddress("SymGetModuleInfo");
		mSymGetSymFromAddrProc = (SymGetSymFromAddrDef)mDbgHelpLibrsry.GetProcAddress("SymGetSymFromAddr");
		mSymGetLineFromAddrProc = (SymGetLineFromAddrDef)mDbgHelpLibrsry.GetProcAddress("SymGetLineFromAddr");
#endif
		mSymSetOptionsProc = (SymSetOptionsDef)mDbgHelpLibrsry.GetProcAddress("SymSetOptions");
		mSymInitializeProc = (SymInitializeDef)mDbgHelpLibrsry.GetProcAddress("SymInitialize");
		mSymCleanupProc = (SymCleanupDef)mDbgHelpLibrsry.GetProcAddress("SymCleanup");

		// シンボルエンジンの初期化
		mSymSetOptionsProc(SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_LINES | SYMOPT_UNDNAME);
		if (mSymInitializeProc(m_process, NULL, TRUE))
		{
			// シンボルエンジン準備完了
			m_isSymbolEngineReady = true;
		}
	}

	~BackTrace()
	{
		if (m_isSymbolEngineReady)
		{
			mSymCleanupProc(m_process);
			m_isSymbolEngineReady = false;
		}
	}

public:

	/// スタックトレースを取得する
	int Backtrace(void** buffer, int n) const
	{
		if (n >= 63) {
			n = 62;
		}
		return (int)mRtlCaptureStackBackTraceProc(0, n, buffer, NULL);
	}

#ifdef LN_X64 //for64bit
	//シンボルの解決
	void AddressToSymbolString(void* address, char * outBuffer, int len) const
	{
		if (!this->m_isSymbolEngineReady)
		{
			//シンボルエンジンが準備できていない.
			_snprintf_s(outBuffer, len, _TRUNCATE, "0x%p @ ??? @ ??? @ ???:???", address);
			return;
		}

		//モジュール名
		IMAGEHLP_MODULE64 imageModule = { sizeof(IMAGEHLP_MODULE64) };
		BOOL r = mSymGetModuleInfo64Proc(m_process, (DWORD64)address, &imageModule);
		if (!r)
		{
			_snprintf_s(outBuffer, len, _TRUNCATE, "0x%p @ ??? @ ??? @ ???:???", address);
			return;
		}

		//シンボル情報格納バッファ.
		IMAGEHLP_SYMBOL64 * imageSymbol;
		char buffer[MAX_PATH + sizeof(IMAGEHLP_SYMBOL64)] = { 0 };
		imageSymbol = (IMAGEHLP_SYMBOL64*)buffer;
		imageSymbol->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL64);
		imageSymbol->MaxNameLength = MAX_PATH;

		//関数名の取得...
		DWORD64 disp = 0;
		r = mSymGetSymFromAddr64Proc(m_process, (DWORD64)address, &disp, imageSymbol);
		if (!r)
		{//関数名がわかりません
			_snprintf_s(outBuffer, len, _TRUNCATE, "0x%p @ %s @ ??? @ ???:???", address, imageModule.ModuleName);
			return;
		}

		//行番号の取得
		IMAGEHLP_LINE64 line = { sizeof(IMAGEHLP_LINE64) };
		r = mSymGetLineFromAddr64Proc(m_process, (DWORD64)address, &disp, &line);
		if (!r)
		{//行番号が分かりません
			_snprintf_s(outBuffer, len, _TRUNCATE, "0x%p @ %s @ %s @ %s+%d", address,
				imageModule.ModuleName, imageSymbol->Name, imageSymbol->Name, (int)((char*)address - (char*)line.Address));
			return;
		}

		//行番号がわかりました.
		_snprintf_s(outBuffer, len, _TRUNCATE, "0x%p @ %s @ %s @ %s:%d", address, imageModule.ModuleName, imageSymbol->Name, line.FileName, line.LineNumber);
	}
#else // #ifdef LN_X64
	//シンボルの解決
	void AddressToSymbolString(void* address, char * outBuffer, int len) const
	{
		if (!m_isSymbolEngineReady)
		{
			// シンボルエンジンが準備できていない
			_snprintf_s(outBuffer, len, _TRUNCATE, "0x%p @ ??? @ ??? @ ???:???", address);
			return;
		}

		// モジュール名
		IMAGEHLP_MODULE imageModule = { sizeof(IMAGEHLP_MODULE) };
		BOOL r = mSymGetModuleInfoProc(m_process, (DWORD)address, &imageModule);
		if (!r) {
			_snprintf_s(outBuffer, len, _TRUNCATE, "0x%p @ ??? @ ??? @ ???:???", address);
			return;
		}

		// シンボル情報格納バッファ
		IMAGEHLP_SYMBOL * imageSymbol;
		char buffer[MAX_PATH + sizeof(IMAGEHLP_SYMBOL)] = { 0 };
		imageSymbol = (IMAGEHLP_SYMBOL*)buffer;
		imageSymbol->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL);
		imageSymbol->MaxNameLength = MAX_PATH;

		// 関数名の取得
		DWORD disp = 0;
		r = mSymGetSymFromAddrProc(m_process, (DWORD)address, &disp, imageSymbol);
		if (!r) {
			_snprintf_s(outBuffer, len, _TRUNCATE, "0x%p @ %s @ ??? @ ???:???", address, imageModule.ModuleName);
			return;
		}

		// 行番号の取得
		IMAGEHLP_LINE line = { sizeof(IMAGEHLP_LINE) };
		r = mSymGetLineFromAddrProc(m_process, (DWORD)address, &disp, &line);
		if (!r) {
			_snprintf_s(outBuffer, len, _TRUNCATE, "0x%p @ %s @ %s @ %s+%d",
				address, imageModule.ModuleName, imageSymbol->Name, imageSymbol->Name, (int)((char*)address - (char*)line.Address));
			return;
		}

		// 一通り揃った (ファイル名は出さないでおく。フルパスで出るのでローカルのフォルダ構成丸わかりだし… 関数名と行番号があれば十分)
#ifdef LN_BACKTRACE_LOG_HIDE
		_snprintf_s(outBuffer, len, _TRUNCATE, "0x%p @ %s @ %s @ XXXX:%d", address, imageModule.ModuleName, imageSymbol->Name, line.LineNumber);
#else
		_snprintf_s(outBuffer, len, _TRUNCATE, "0x%p @ %s @ %s @ %s:%d", address, imageModule.ModuleName, imageSymbol->Name, line.FileName, line.LineNumber);
#endif
	}
#endif // #ifdef LN_X64

	// シンボルをまとめて解決
	void AddressToFullSymbolString(void** address, int size, char* outBuffer, int len) const
	{
		int writesize = 0;

		int i = 0;
		for (i = 0; i < size; i++)
		{
			void * p = address[i];
			if (p == NULL) {
				break;
			}

			AddressToSymbolString(p, outBuffer + writesize, len - writesize);
			writesize += (int)strlen(outBuffer + writesize);

			if (len - writesize >= 3) {
				strncat_s(outBuffer + writesize, len - writesize, "\r\n", 3);
				writesize += 2;
			}

			if (len <= writesize) {
				break;
			}
		}

		outBuffer[writesize] = '\0';
	}

	//バックトレースの取得して画面に表示
	//void printBackTrace() const
	//{
	//	void* stackBuffer[50];
	//	char symbolBuffer[1024];

	//	int stacksize = this->backtrace(stackBuffer, 50);
	//	this->addressToFullSymbolString(stackBuffer, stacksize, symbolBuffer, 1024);

	//	puts(symbolBuffer);
	//}

};

LN_NAMESPACE_END

