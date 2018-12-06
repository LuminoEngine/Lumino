#pragma once

class TestEnv
{
public:
	static void setup();
	static void teardown();

	static String LuminoCLI;
	static Path LuminoTestProjectDir;	// init に時間がかかる場合があるので、毎回 init する必要のないテストはここで行う
};
