#pragma once
#include <unordered_map>

class FxcCommand
{
public:
    void execute(const ln::Path& inputFile);

private:
    struct ShaderCode
    {
        std::string glslCode;
        //std::vector<uint32_t> spirvCode;
    };

    std::unordered_map<std::string, ShaderCode> m_vertexShaderCodeMap;
    std::unordered_map<std::string, ShaderCode> m_pixelShaderCodeMap;
};
