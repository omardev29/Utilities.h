#pragma once

#include <string_view>
#include <charconv>
#include <system_error>
#include <version>


#if defined(__cpp_lib_print)
    #define USE_PRINT 1
#else
    #define USE_PRINT 0
    extern "C" inline int printf(const char* format, ...);
#endif

//colores
#define ESC "\033["
#define RED "31m"
#define RESET "\033[0m"

namespace num {
    // Versi�n para std::string
    inline bool is_n(const std::string& s) {
        if (s.empty()) return false;
        double dummy; // Usamos double para que sirva para enteros y decimales
        auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), dummy);
        return ec == std::errc{} && ptr == s.data() + s.size();
    }

    // Versi�n para char* (C-style string)
    inline bool is_n(const char* s) {
        if (!s || s[0] == '\0') return false;
        double dummy;
        auto [ptr, ec] = std::from_chars(s, s + std::strlen(s), dummy);
        return ec == std::errc{} && (*ptr == '\0');
    }

    // Convertir double a char* (C-style string)
    // Retorna el puntero al buffer si tiene �xito, o nullptr si falla
    inline char* to_char(double val, char* buf = nullptr) {
        static thread_local char internal_buf[32];
        if (!buf) {
            buf = internal_buf;
        }
        // antes ped�a un size_t len como otro par�metro
        size_t len{ sizeof(buf) };
        // len - 1 para dejar espacio al terminador nulo '\0'
        auto [ptr, ec] = std::to_chars(buf, buf + len - 1, val);

        if (ec == std::errc{}) {
            *ptr = '\0'; // std::to_chars NO a�ade el null-terminator
            return buf;
        }
        return nullptr;
    }
}

namespace logs {
    inline void err(const std::string& x) {
        #if USE_PRINT
            std::println("{}{}{}{}", ESC, RED, x, RESET);
        #else
            printf("%s%s%s%s\n", ESC, RED, x.c_str(), RESET);
        #endif
    }

    inline void err(const char* x) {
        #if USE_PRINT
            std::println("{}{}{}{}", ESC, RED, x, RESET);
        #else
            printf("%s%s%s%s\n", ESC, RED, x, RESET);
        #endif
    }
}

