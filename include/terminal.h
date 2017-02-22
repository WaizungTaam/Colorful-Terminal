/* terminal.h
 *
 * 2017-02-21
 * waizungtaam@gmail.com
 * Reference: https://en.wikipedia.org/wiki/ANSI_escape_code#graphics
 *
 */

#ifndef TERMINAL_H
#define TERMINAL_H

#include <cstdio>
#include <iostream>

#if defined(__linux__)
    #define TERMINAL_OS_LINUX
#elif defined(_WIN32)
    #define TERMINAL_OS_WIN32
#else
    #define TERMINAL_OS_LINUX
#endif  /* #if defined(__linux__) */

#if defined(TERMINAL_OS_LINUX)
    #include <unistd.h>
#elif defined(TERMINAL_OS_WIN32)
    #include <io.h>
    #include <windows.h>
#endif  /* #if defined(TERMINAL_OS_LINUX) */

namespace terminal {

namespace internal {

std::FILE* std_stream(const std::ostream& stream) {
    if (&stream == &std::cout) {
        return stdout;
    } else if (&stream == &std::cerr || &stream == &std::clog) {
        return stderr;
    }
    return nullptr;
}

}  /* namespace internal */

#if defined(TERMINAL_OS_LINUX)

namespace internal {

inline bool is_atty(const std::ostream& stream) {
    return ::isatty(::fileno(std_stream(stream)));
}

std::ostream& set(std::ostream& stream, int param) {
    if (is_attty(stream)) {
        stream << "\033[" << param << "m";
    }
    return stream;
}

}  /* namespace internal */

inline std::ostream& reset(std::ostream& stream) {
    return internal::set(stream, 0);
}
inline std::ostream& bold(std::ostream& stream) {
    return internal::set(stream, 1);
}
inline std::ostream& faint(std::ostream& stream) {
    return internal::set(stream, 2);
}
inline std::ostream& underline(std::ostream& stream) {
    return internal::set(stream, 4);
}
inline std::ostream& blink(std::ostream& stream) {
    return internal::set(stream, 5);
}
inline std::ostream& reverse(std::ostream& stream) {
    return internal::set(stream, 7);
}
inline std::ostream& conceal(std::ostream& stream) {
    return internal::set(stream, 8);
}

namespace fg {

inline std::ostream& black(std::ostream& stream) {
    return internal::set(stream, 30);
}
inline std::ostream& red(std::ostream& stream) {
    return internal::set(stream, 31);
}
inline std::ostream& green(std::ostream& stream) {
    return internal::set(stream, 32);
}
inline std::ostream& yellow(std::ostream& stream) {
    return internal::set(stream, 33);
}
inline std::ostream& blue(std::ostream& stream) {
    return internal::set(stream, 34);
}
inline std::ostream& magenta(std::ostream& stream) {
    return internal::set(stream, 35);
}
inline std::ostream& cyan(std::ostream& stream) {
    return internal::set(stream, 36);
}
inline std::ostream& white(std::ostream& stream) {
    return internal::set(stream, 37);
}

}  /* namespace fg */

namespace bg {

inline std::ostream& black(std::ostream& stream) {
    return internal::set(stream, 40);
}
inline std::ostream& red(std::ostream& stream) {
    return internal::set(stream, 41);
}
inline std::ostream& green(std::ostream& stream) {
    return internal::set(stream, 42);
}
inline std::ostream& yellow(std::ostream& stream) {
    return internal::set(stream, 43);
}
inline std::ostream& blue(std::ostream& stream) {
    return internal::set(stream, 44);
}
inline std::ostream& magenta(std::ostream& stream) {
    return internal::set(stream, 45);
}
inline std::ostream& cyan(std::ostream& stream) {
    return internal::set(stream, 46);
}
inline std::ostream& white(std::ostream& stream) {
    return internal::set(stream, 47);
}

}  /* namespace bg */

#elif defined(TERMINAL_OS_WIN32)

namespace internal {

inline bool is_atty(const std::ostream& stream) {
    return ::_isatty(::_fileno(std_stream(stream)));
}

void set(const std::ostream& stream, int foreground, int background) {
    if (is_atty(stream)) {
        HANDLE handle = INVALID_HANDLE_VALUE;
        if (&stream == &std::cout) {
            handle = GetStdHandle(STD_OUTPUT_HANDLE);
        } else if (&stream == &std::cerr || &stream == &std::clog) {
            handle = GetStdHandle(STD_ERROR_HANDLE);
        }
        CONSOLE_SCREEN_BUFFER_INFO info;
        if (!GetConsoleScreenBufferInfo(handle, &info)) {
            return;
        }
        static WORD default_attr = 0;
        if (default_attr == 0) {
            default_attr = info.wAttributes;
        }
        if (foreground == -1 && background == -1) {
            info.wAttributes = default_attr;
        }
        if (foreground != -1) {
            info.wAttributes &= ~(info.wAttributes & 0x0F);
            info.wAttributes |= static_cast<WORD>(foreground);
        }
        if (background != -1) {
            info.wAttributes &= !(info.wAttributes & 0xF0);
            info.wAttributes |= static_cast<WORD>(background);
        }
        SetConsoleTextAttribute(handle, info.wAttributes);
    }
}

}  /* namespace internal */

inline std::ostream& reset(std::ostream& stream) {
    internal::set(stream, -1, -1);
    return stream;
}
inline std::ostream& bold(std::ostream& stream) {
    return stream;
}
inline std::ostream& faint(std::ostream& stream) {
    return stream;
}
inline std::ostream& underline(std::ostream& stream) {
    return stream;
}
inline std::ostream& blink(std::ostream& stream) {
    return stream;
}
inline std::ostream& reverse(std::ostream& stream) {
    return stream;
}
inline std::ostream& conceal(std::ostream& stream) {
    return stream;
}

namespace fg {

inline std::ostream& black(std::ostream& stream) {
    internal::set(stream, 0, -1);
    return stream;
}
inline std::ostream& red(std::ostream& stream) {
    internal::set(stream, FOREGROUND_RED, -1);
    return stream;
}
inline std::ostream& green(std::ostream& stream) {
    internal::set(stream, FOREGROUND_GREEN, -1);
    return stream;
}
inline std::ostream& yellow(std::ostream& stream) {
    internal::set(stream, FOREGROUND_RED | FOREGROUND_GREEN , -1);
    return stream;
}
inline std::ostream& blue(std::ostream& stream) {
    internal::set(stream, FOREGROUND_BLUE, -1);
    return stream;
}
inline std::ostream& magenta(std::ostream& stream) {
    internal::set(stream, FOREGROUND_RED | FOREGROUND_BLUE, -1);
    return stream;
}
inline std::ostream& cyan(std::ostream& stream) {
    internal::set(stream,FOREGROUND_GREEN | FOREGROUND_BLUE, -1);
    return stream;
}
inline std::ostream& white(std::ostream& stream) {
    internal::set(stream, 
        FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE, -1);
    return stream;
}

}  /* namespace fg */

namespace bg {

inline std::ostream& black(std::ostream& stream) {
    internal::set(stream, -1, 0);
    return stream;
}
inline std::ostream& red(std::ostream& stream) {
    internal::set(stream, -1, BACKGROUND_RED);
    return stream;
}
inline std::ostream& green(std::ostream& stream) {
    internal::set(stream, -1, BACKGROUND_GREEN);
    return stream;
}
inline std::ostream& yellow(std::ostream& stream) {
    internal::set(stream, -1, BACKGROUND_RED | BACKGROUND_GREEN);
    return stream;
}
inline std::ostream& blue(std::ostream& stream) {
    internal::set(stream, -1, BACKGROUND_BLUE);
    return stream;
}
inline std::ostream& magenta(std::ostream& stream) {
    internal::set(stream, -1, BACKGROUND_RED | BACKGROUND_BLUE);
    return stream;
}
inline std::ostream& cyan(std::ostream& stream) {
    internal::set(stream, -1, BACKGROUND_GREEN | BACKGROUND_BLUE);
    return stream;
}
inline std::ostream& white(std::ostream& stream) {
    internal::set(stream, 
        -1, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
    return stream;
}

}  /* namespace bg */

#endif  /* #if defined(TERMINAL_OS_LINUX) */

}  /* namespace terminal */

#undef TERMINAL_OS_LINUX
#undef TERMINAL_OS_WIN32

#endif  /* TERMINAL_H */

