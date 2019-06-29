
#include "../Core/SymbolDatabase.hpp"

#define TEST_ROOT u"D:/Proj/LN/Lumino/"

int main(int argc, char** argv)
{
    ln::List<ln::Path> files =
    {
        TEST_ROOT "include/LuminoCore/Math/Vector2.hpp",
        TEST_ROOT "include/LuminoCore/Math/Vector3.hpp",
        TEST_ROOT "include/LuminoCore/Math/Vector4.hpp",
        TEST_ROOT "include/LuminoCore/Math/Quaternion.hpp",
        TEST_ROOT "include/LuminoCore/Math/Matrix.hpp",
        TEST_ROOT "include/LuminoEngine/Engine/Application.hpp",
        //u"C:/Proj/GitHub/Lumino/src/LuminoEngine/include/Lumino/Graphics/Texture.hpp",
    };

	
	auto diag = ln::makeObject<ln::DiagnosticsManager>();
    SymbolDatabase db(diag);

	return 0;
}
