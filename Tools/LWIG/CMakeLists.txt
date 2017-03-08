
set(LN_SOURCES
	LWIG.PCH.cpp
	LWIG.PCH.h
	Common.h
	Global.h
	Main.cpp
	OutputBuffer.cpp
	OutputBuffer.h
	Parser.cpp
	Parser.h
	SymbolDatabase.cpp
	SymbolDatabase.h
	DotNet/CSClassLibGenerator.cpp
	DotNet/CSClassLibGenerator.h
	DotNet/DotNetClassLibGenerator.cpp
	DotNet/DotNetClassLibGenerator.h
	DotNet/DotNetCommon.cpp
	DotNet/DotNetCommon.h
	DotNet/DotNetPInvokeLibGenerator.cpp
	DotNet/DotNetPInvokeLibGenerator.h
	WrapperIF/WrapperIFClassesGenerator.h
	WrapperIF/WrapperIFClassesGenerator.cpp
	WrapperIF/WrapperIFGenerator.h
	WrapperIF/WrapperIFGenerator.cpp
)

source_group("Souece"	FILES ${LN_SOURCES})

include_directories(
	"${PROJECT_SOURCE_DIR}/Source/LuminoCore/Include"
	"${PROJECT_SOURCE_DIR}/Source/Fluorite/Include"
)

add_executable(LWIG ${LN_SOURCES})
target_link_libraries(LWIG LuminoEngine)
target_link_libraries(LWIG Fluorite)
ln_add_pch(LWIG "LWIG.PCH.h" "LWIG.PCH.cpp")

