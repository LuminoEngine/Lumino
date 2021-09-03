#include <stdio.h>
#include <emscripten/emscripten.h>

//int main(int argc, char ** argv) {
//    printf("Hello World\n");
//}

#ifdef __cplusplus
extern "C" {
#endif

extern void gogo_test();

void EMSCRIPTEN_KEEPALIVE myFunction(int argc, char ** argv) {
  printf("MyFunction Called\n");
  gogo_test();
}

#ifdef __cplusplus
}
#endif
