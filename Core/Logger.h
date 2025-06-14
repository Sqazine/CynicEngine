#pragma once
#include <iostream>
#include <sstream>
#include <string_view>
#include <string>
#include <cassert>
#include <cstdarg>
#include "Marco.h"
namespace CynicEngine
{
    namespace Logger
    {
        enum class Kind
        {
            INFO,
            WARN,
            ERROR
        };

        inline void Output(OSTREAM &os, STRING s)
        {
            os << s;
        }

        template <typename T, typename... Args>
        inline void Output(OSTREAM &os, STRING s, const T &next, const Args &...args)
        {
            auto index = s.find_first_of(TEXT("{}"));
            if (index == STRING::npos)
                Output(os, s);
            else
            {
                STRING_STREAM sstr;
                sstr << next;
                s.replace(index, 2, sstr.str());
                sstr.clear();
                Output(os, s, args...);
            }
        }

        template <typename... Args>
        inline void Println(const STRING &s, const Args &...args)
        {
            Output(COUT, s + TEXT("\n"), args...);
        }

        template <typename... Args>
        inline void Print(const STRING &s, const Args &...args)
        {
            Output(COUT, s, args...);
        }

        template <typename... Args>
        void Log(Kind logKind, const STRING &fmt, const Args &...args)
        {
            switch (logKind)
            {
            case Kind::ERROR:
                Println(TEXT("\033[31m[ERROR]:") + fmt + TEXT("\033[0m"), args...);
                break;
            case Kind::WARN:
                Println(TEXT("\033[33m[WARN]:") + fmt + TEXT("\033[0m"), args...);
                break;
            case Kind::INFO:
                Println(TEXT("\033[32m[INFO]:") + fmt + TEXT("\033[0m"), args...);
                break;
            default:
                break;
            }
        }
    }
#define CYNIC_ENGINE_LOG_ERROR(fmt, ...)                                                \
    do                                                                                  \
    {                                                                                   \
        CynicEngine::Logger::Log(CynicEngine::Logger::Kind::ERROR, fmt, ##__VA_ARGS__); \
        assert(0);                                                                      \
    } while (false);
    
#define CYNIC_ENGINE_LOG_WARN(fmt, ...)                                                \
    do                                                                                 \
    {                                                                                  \
        CynicEngine::Logger::Log(CynicEngine::Logger::Kind::WARN, fmt, ##__VA_ARGS__); \
    } while (false);

#define CYNIC_ENGINE_LOG_INFO(fmt, ...)                                                \
    do                                                                                 \
    {                                                                                  \
        CynicEngine::Logger::Log(CynicEngine::Logger::Kind::INFO, fmt, ##__VA_ARGS__); \
    } while (false);
}