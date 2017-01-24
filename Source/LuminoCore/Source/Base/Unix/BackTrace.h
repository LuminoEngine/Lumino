
#pragma once

#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <cxxabi.h>
#include <execinfo.h>
#include <bfd.h>		// [$ sudo apt-get install binutils-dev] or [binutils-devel]

LN_NAMESPACE_BEGIN

class BackTrace
{
private:

	bool		mIsSymbolEngineReady;
	bfd*		mAbfd;
	asymbol**	mSymbols;
	int			mNSymbols;
	asection*	mSection;
	
public:

	static const BackTrace* GetInstance()
	{
		static BackTrace s;
		return &s;
	}

	BackTrace()
	{
		mIsSymbolEngineReady = false;
		mAbfd = NULL;
		mSymbols = NULL;
		mNSymbols = 0;

		//see http://0xcc.net/blog/archives/000073.html
		mAbfd = bfd_openr("/proc/self/exe", NULL);
 		if (!mAbfd) {
			return;
		}
		bfd_check_format(mAbfd, bfd_object);

		int size = bfd_get_symtab_upper_bound(mAbfd);
		if (size <= 0) {
			return;
		}
		mSymbols = (asymbol**) malloc(size);
		if (!mSymbols) {
			return;
		}
		mNSymbols = bfd_canonicalize_symtab(mAbfd, mSymbols);
		if (!mNSymbols) {
			return ;
		}
		mSection = bfd_get_section_by_name(mAbfd, ".debug_info");
		if (!mSection) {
			return;
		}

		mIsSymbolEngineReady = true;
	}

	~BackTrace()
	{
		if (mSymbols)
		{
			free(mSymbols);
			mSymbols = NULL;
		}
		if (mAbfd)
		{
			bfd_close (mAbfd);
			mAbfd = NULL;
		}
	}

public:

	/// スタックトレースを取得する
	int Backtrace(void** buffer, int n) const
	{
		return ::backtrace(buffer , n);
	}
	
	//シンボルの解決
	void AddressToSymbolString(void* address, char* outBuffer, int len) const
	{
		//see http://d.hatena.ne.jp/syuu1228/20100215/1266262848
		Dl_info info;
		if (! dladdr(address, &info) ) 
		{
			snprintf(outBuffer ,len , "0x%p @ ??? @ ??? @ ???:???" ,address );
			return ;
		}
		if (!info.dli_sname)
		{
			snprintf(outBuffer ,len , "0x%p @ %s @ ??? @ ???:???" ,address , info.dli_fname );
			return ;
		}
		if (!info.dli_saddr)
		{
			snprintf(outBuffer ,len , "0x%p @ %s @ %s @ ???:???" ,address , info.dli_fname , info.dli_sname );
			return ;
		}

		//デマングルして関数名を読める形式に
		int status = 0;
		char * demangled = abi::__cxa_demangle(info.dli_sname,0,0,&status);

		//シンボルエンジンは使えるの？
		if (!mIsSymbolEngineReady)
		{
			snprintf(outBuffer ,len , "0x%p @ %s @ %s @ %s+0x%p" ,
				address , info.dli_fname , (demangled ? demangled : info.dli_sname),
				(demangled ? demangled : info.dli_sname), 
				(unsigned int) ((char *)address - (char *)info.dli_saddr) );
			free(demangled);
			return ;
		}
		
		//ファイル名と行数を求める.
		const char* filename = NULL;
		const char* functionname = NULL;
		unsigned int line = 0;

		int found = bfd_find_nearest_line(mAbfd, mSection, mSymbols,
											(long)address,
											&filename,
											&functionname,
											&line);
		if (found && filename != NULL && functionname != NULL) 
		{
			snprintf(outBuffer ,len , "0x%p @ %s @ %s @ %s+0x%p" ,
					address , info.dli_fname , (demangled ? demangled : info.dli_sname),
											    (demangled ? demangled : info.dli_sname),
										        (unsigned int) ((char *)address - (char *)info.dli_saddr) );
			free(demangled);
			return ;
		}

		snprintf(outBuffer ,len , "0x%p @ %s @ %s @ %s:%d" ,
				address , info.dli_fname , (demangled ? demangled : info.dli_sname), filename , line );
		free(demangled);
		
		return;
	}
	
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

			if (len - writesize >= 1) {
				strncat_s(outBuffer + writesize, len - writesize, "\n", 1);
				writesize += 1;
			}

			if (len <= writesize) {
				break;
			}
		}
	}
};

LN_NAMESPACE_END

