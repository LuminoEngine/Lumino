
#include "Common.h"
//#include "Utils.h"
#include "DXDevice.h"
//#include "Converter.h"
//
//#include <fstream>
/*
制限…
・VertexShader 型は使用不可。(hsls2glslfork が対応していない)
・pass の VertexShader に変数は設定できない。
例)VertexShader = (vsArray[CurNumBones]);
・アノテーション外の文字列は禁止
(文字列内の shared が消える可能性がある)

HLSL 言語構文
http://msdn.microsoft.com/ja-jp/library/bb509615%28v=vs.85%29.aspx



*/

int main(int argc, char* argv[])
{
#if 0
	std::string shaderSource;
	std::ofstream ofs("shader.txt");
	ofs << "test";


	if (argc < 2)
	{
		return 1;
	}

	DXDevice device;
	if (!device.initlaize())
	{
		printf("failed initialize DirectX.\n");
		device.finalize();
		return 1;
	}

	//Converter converter;
	//converter.convert( "D:/Proj/LightNote/Source/Core/Resource/Shader/SSBasic3D.fx", &device );

	/*if ( Hlsl2Glsl_Initialize() == 0 ) {
	printf( "failed Hlsl2Glsl_Initialize.\n" );
	return 1;
	}*/



	//converter.convert( "SSBasic2D.fx", &device );
	//converter.convert( "SSBasic3D.fx", &device );
	//converter.convert( "MirrorWater_low.fxm", &device );
	//converter.convert( "full.fx", &device );

	for (int i = 1; i < argc; ++i)
	{
		Converter converter;
		converter.convert(argv[i], &device);
	}

	//Hlsl2Glsl_Shutdown();
	device.finalize();
#endif
	return 0;
}

