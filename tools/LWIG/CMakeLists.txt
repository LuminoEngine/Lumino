
set(LN_SOURCES
	LWIG.PCH.cpp
	LWIG.PCH.h
	Global.h
	Main.cpp
	OutputBuffer.cpp
	OutputBuffer.h
	Parser.cpp
	Parser.h
	SymbolDatabase.cpp
	SymbolDatabase.h
	DotNet/DotNetPInvokeLibGenerator.h
	DotNet/DotNetClassLibGenerator.cpp
	DotNet/DotNetClassLibGenerator.h
	DotNet/DotNetCommon.cpp
	DotNet/DotNetCommon.h
	DotNet/DotNetPInvokeLibGenerator.cpp
	WrapperIF/WrapperIFGenerator.h
	WrapperIF/WrapperIFGenerator.cpp
)

source_group("Souece"	FILES ${LN_SOURCES})

include_directories(
	"${PROJECT_SOURCE_DIR}/external/Lumino.Core/include"
	"${PROJECT_SOURCE_DIR}/external/Fluorite/include"
)

add_executable(LWIG ${LN_SOURCES})
target_link_libraries(LWIG Lumino_Static)
target_link_libraries(LWIG Fluorite)
ln_add_pch(LWIG "LWIG.PCH.h" "LWIG.PCH.cpp")

