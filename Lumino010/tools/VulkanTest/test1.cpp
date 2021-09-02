#include <Windows.h>
#include <stdio.h>

int main()
{
    HMODULE hModule = ::LoadLibraryW(L"vulkan-1.dll");
    
    if (hModule) {
        printf("Vulkan Found!\n");
    }
    else {
        printf("Vulkan -- not found\n");
    }
    
    return 0;
}

