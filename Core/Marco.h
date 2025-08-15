#ifndef CYNIC_ENGINE_BUILD_STATIC
#if defined(_WIN32) || defined(_WIN64)
#ifdef CYNIC_ENGINE_BUILD_DLL
#define CYNIC_ENGINE_API __declspec(dllexport)
#else
#define CYNIC_ENGINE_API __declspec(dllimport)
#endif
#else
#ifdef CYNIC_ENGINE_BUILD_DLL
#define CYNIC_ENGINE_API
#else
#define CYNIC_ENGINE_API
#endif
#endif
#else
#define CYNIC_ENGINE_API
#endif

#ifdef CYNIC_ENGINE_UTF8_ENCODE
#define STR(x) L##x
#define TEXT(x) STR(x)
#define TCHAR(x) STR(x)
#define CHAR_T wchar_t
#define STRING std::wstring
#define STRING_VIEW std::wstring_view
#define OSTREAM std::wostream
#define STRING_STREAM std::wstringstream
#define STD_IFSTREAM std::wifstream
#define CYNIC_ENGINE_TO_STRING(x) std::to_wstring(x)
#define COUT std::wcout
#define CIN std::wcin
#define STRCMP wcscmp
#else
#define STR(x) x
#define TEXT(x) STR(x)
#define TCHAR(x) STR(x)
#define CHAR_T char
#define STRING std::string
#define STRING_VIEW std::string_view
#define OSTREAM std::ostream
#define STRING_STREAM std::stringstream
#define STD_IFSTREAM std::ifstream
#define CYNIC_ENGINE_TO_STRING(x) std::to_string(x)
#define COUT std::cout
#define CIN std::cin
#define STRCMP strcmp
#endif

#define SAFE_DELETE(x)   \
    do                   \
    {                    \
        if (x)           \
        {                \
            delete x;    \
            x = nullptr; \
        }                \
    } while (false);