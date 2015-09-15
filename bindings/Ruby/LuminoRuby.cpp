#include "LuminoRuby.h"

VALUE g_luminoModule;

// ŠÖ”–¼‚Í create_makefile ‚Åw’è‚µ‚½–¼‘O‚Æ‡‚í‚¹‚Ä‚¨‚©‚È‚¯‚ê‚Î‚È‚ç‚È‚¢
extern "C" void Init_Lumino()
{
	g_luminoModule = rb_define_module("Lumino");
	InitEnums();
	InitStructs();
}


