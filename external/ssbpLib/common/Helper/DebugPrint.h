#ifndef __DEBUG_PRINTF__
#define __DEBUG_PRINTF__

#include <string>

namespace ss
{

void DEBUG_PRINTF( const char* strFormat, ...   );

struct ThrowErrorMessage{
	std::string message;
	int	error_no;

	ThrowErrorMessage( int no , std::string str ){ error_no = no ; message = str; }
};


#define THROW_ERROR_MESSAGE(str) \
{\
THROW_ERROR_MESSAGE_MAIN( str , __FILE__ , __LINE__ );\
}\

void	THROW_ERROR_MESSAGE_MAIN( std::string str , char* fname , size_t line );

};

#endif