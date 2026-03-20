#include <stdio.h>
#include <vector>
#include <string>
#include <lumino.h>

std::vector<uint8_t> ReadAllBytes(const std::string& filePath);

int main() {
    LNInstance_Initialize();
    LNHandle window = LN_NULL_HANDLE;
    LNWindow_Create(640, 480, "Example", &window);

    std::vector<uint8_t> soundData = ReadAllBytes("D:/Music/momentum/02 - momentum.wav");
    LNHandle sound;
    LNSound_CreateFromData(soundData.data(), soundData.size(), &sound);

    while (true) {
        LNBool quit = LN_FALSE;
        LNInstance_ProcessEvents();
        LNInstance_ShouldQuit(&quit);
        if (quit) {
            break;
        }
    };

    LNObject_Release(sound);
    LNObject_Release(window);
    LNInstance_Terminate();
    return 0;
}

size_t GetFileSize(FILE* stream) {
    struct stat stbuf;
    int handle = fileno(stream);
    if (handle == 0) return 0;
    if (fstat(handle, &stbuf) == -1) return 0;
    return stbuf.st_size;
}

std::vector<uint8_t> ReadAllBytes(const std::string& filePath) {
    FILE* file = fopen(filePath.c_str(), "rb");
    if (!file) {
        printf("Error: %s\n", filePath.c_str());
        return {};
    }
    size_t size = GetFileSize(file);
    std::vector<uint8_t> buffer(size);
    fread(buffer.data(), 1, size, file);
    fclose(file);
    return buffer;
}
