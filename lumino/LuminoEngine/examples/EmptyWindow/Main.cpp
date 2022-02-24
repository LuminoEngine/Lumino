#include <LuminoEngine.hpp>
using namespace ln;

int main() {
    Engine::initialize();

    while (Engine::update()) {
    
    } 

    Engine::terminate();
}
