#pragma once

#include <string_view>
#include <charconv>
#include <system_error>
#include <version>
#include <random>


#if defined(__cpp_lib_print)
    #define USE_PRINT 1
    #include <print>
#else
    #define USE_PRINT 0
    extern "C" inline int printf(const char* format, ...);
#endif

//colors
#define ESC "\033["
#define RED "31m"
#define RESET "\033[0m"

namespace num {
    // std::string version
    inline bool is_n(const std::string& s) {
        if (s.empty()) return false;
        double dummy; // Usamos double para que sirva para enteros y decimales
        auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), dummy);
        return ec == std::errc{} && ptr == s.data() + s.size();
    }

    // char* version (C-style string)
    inline bool is_n(const char* s) {
        if (!s || s[0] == '\0') return false;
        double dummy;
        auto [ptr, ec] = std::from_chars(s, s + std::strlen(s), dummy);
        return ec == std::errc{} && (*ptr == '\0');
    }

    // double to char* (C-style string)
    inline char* to_char(double val, char* buf = nullptr) {
        static thread_local char internal_buf[32];
        if (!buf) {
            buf = internal_buf;
        }
        size_t len{ sizeof(buf) };
        // len - 1 to avoid '\0'
        auto [ptr, ec] = std::to_chars(buf, buf + len - 1, val);

        if (ec == std::errc{}) {
            *ptr = '\0'; 
            return buf;
        }
        return nullptr;
    }

    inline std::mt19937& get_gen() {
        static std::random_device rd;

        static std::mt19937 gen(rd());

        return gen;
    }
    
    inline int randi_range(const int& min, const int& max) {
        std::uniform_int_distribution<> dis(min, max);

        return dis(get_gen());

    }

    inline double randf_range(const double& min, const double& max) {
        std::uniform_real_distribution<double> dis(min, max);
        return dis(get_gen());
    }

    inline double randfn() {
        std::uniform_real_distribution<double> dis(0, 1);

        return dis(get_gen());
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

