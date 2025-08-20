#include "IO.h"
#include <fstream>
#include <sstream>
#include "Logger/Logger.h"
namespace CynicEngine
{
    std::string ReadFile(std::string_view path)
    {
        std::ifstream file;
        file.open(path.data(), std::ios::in | std::ios::binary);
        if (!file.is_open())
        {
#ifdef CYNIC_ENGINE_UTF8_ENCODE
            STRING utf8Path = Utf8::Decode(path.data());
            CYNIC_ENGINE_LOG_ERROR(TEXT("Failed to open file:{}"), utf8Path);
#else
            CYNIC_ENGINE_LOG_ERROR(TEXT("Failed to open file:{}"), path);
#endif
        }

        std::stringstream sstream;
        sstream << file.rdbuf();
        file.close();
        return sstream.str();
    }
}