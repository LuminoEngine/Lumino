
#include <stdio.h>
#include <emscripten.h>

extern int main2();

static void ln_main_loop()
{
    printf("ln_main_loop.");
    ln::Engine::update();

    static int count = 0;
    printf("%d\n", count);
}


int main(int argc, char** argv)
{
    printf("run test.");
    ln::Engine::initialize();
    printf("initialized.");

	emscripten_set_main_loop(ln_main_loop, 0, true);

    // the next to emscripten_set_main_loop is unreachable.
    
	//printf("run test.");
    //main2();
	return 0;
}

