
#include "../Internal.h"
#include <Lumino/IO/DirectoryUtils.h>

LN_NAMESPACE_BEGIN
namespace detail {
	
	
	template<typename TChar>
	class GenericFileFinderImpl
		: public GenericFileFinderImplBase<TChar>
	{
	public:
		GenericFileFinderImpl(const GenericStringRef<TChar>& dirPath)
			: GenericFileFinderImplBase<TChar>(dirPath)
		{
			assert(0);
		}
		
		~GenericFileFinderImpl()
		{
			assert(0);
		}
		
		void initialize(const TChar* dirPath)
		{
			assert(0);
		}
		
		virtual bool next() override
		{
			assert(0);
			return false;
		}
	};
	


template<>
class GenericFileFinderImpl<char>
	: public GenericFileFinderImplBase<char>
{
public:
	GenericFileFinderImpl(const GenericStringRef<char>& dirPath)
		: GenericFileFinderImplBase<char>(dirPath)
		, m_dir(NULL)
	{
		StringA t;
		t.assignCStr(GenericFileFinderImplBase<char>::m_dirPath.c_str());
		initialize(t.c_str());
	}

	~GenericFileFinderImpl()
	{
		if (m_dir != NULL)
		{
			closedir(m_dir);
		}
	}

	void initialize(const char* dirPath)
	{
		m_dir = opendir(dirPath);
		LN_THROW(m_dir != NULL, IOException, dirPath);

		next();
	}

	virtual bool next() override
	{
		struct dirent* entry;
		do
		{
			entry = readdir(m_dir);
			if (entry)
			{
				GenericFileFinderImplBase<char>::setCurrentFileName(entry->d_name);
			}
			else
			{
				GenericFileFinderImplBase<char>::setCurrentFileName((char*)NULL);
				break;
			}
		} while (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0);

		return !GenericFileFinderImplBase<char>::getCurrent().isEmpty();
	}

private:
	DIR*	m_dir;
};

} // namespace detail
LN_NAMESPACE_END
