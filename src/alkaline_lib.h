#pragma once

#include <stdio.h>

// for malloc
#include <stdlib.h>

// for memset
#include <string.h>

// for file timestamps
#include <sys/stat.h>

// for working directory
#include <filesystem>

#ifdef _WIN32
#define DEBUG_BREAK() __debugbreak()
#endif

enum TextColor
{
    TEXT_COLOR_WHITE,
    TEXT_COLOR_BLACK,
    TEXT_COLOR_RED,
    TEXT_COLOR_GREEN,
    TEXT_COLOR_YELLOW,
    TEXT_COLOR_COUNT
};

using uint = unsigned int;
#define b8 char
#define BIT(x) 1 << (x)
#define KB(x) ((unsigned long long)1024 * x)
#define MB(x) ((unsigned long long)1024 * KB(x))
#define GB(x) ((unsigned long long)1024 * MB(x))

// ######################################################################################
//                                      LOGGING
// ######################################################################################

// TODO: add categories/channels to log for different systems
template <typename... Args>
void _log(char const *prefix, char const *msg, TextColor textColor, Args... args)
{
    static char const *TextColorTable[TEXT_COLOR_COUNT] =
        {
            "\x1b[37m", // TEXT_COLOR_WHITE
            "\x1b[30m", // TEXT_COLOR_BLACK
            "\x1b[31m", // TEXT_COLOR_RED
            "\x1b[32m", // TEXT_COLOR_GREEN
            "\x1b[33m", // TEXT_COLOR_YELLOW
        };

    char formatBuffer[8192] = {};
    sprintf(formatBuffer, "%s%s %s \033[0m", TextColorTable[textColor], prefix, msg);

    char textBuffer[8192] = {};
    sprintf(textBuffer, formatBuffer, args...);

    puts(textBuffer);
}

#define ALK_LOG(msg, ...) _log("LOG: ", msg, TEXT_COLOR_WHITE, ##__VA_ARGS__)
#define ALK_TRACE(msg, ...) _log("TRACE: ", msg, TEXT_COLOR_GREEN, ##__VA_ARGS__)
#define ALK_WARN(msg, ...) _log("WARN: ", msg, TEXT_COLOR_YELLOW, ##__VA_ARGS__)
#define ALK_ERROR(msg, ...) _log("ERROR: ", msg, TEXT_COLOR_RED, ##__VA_ARGS__)
#define ALK_SUCCESS(msg, ...) _log("[TEST][SUCCESS] ", msg, TEXT_COLOR_GREEN, ##__VA_ARGS__)
#define ALK_FAILURE(msg, ...) _log("[TEST][FAILURE] ", msg, TEXT_COLOR_RED, ##__VA_ARGS__)

#define ALK_ASSERT(x, msg, ...)            \
    {                                      \
        if (!(x))                          \
        {                                  \
            ALK_ERROR(msg, ##__VA_ARGS__); \
            DEBUG_BREAK();                 \
        }                                  \
    }

#define ALK_TEST(x, msg, ...)               \
    {                                       \
        if (x)                              \
        {                                   \
            ALK_SUCCESS(msg, ##__VA_ARGS__);\
        }                                   \
        else                                \
        {                                   \
            ALK_FAILURE(msg, ##__VA_ARGS__);\
        }                                   \
    }                                       \


// ######################################################################################
//                                      I / O
// ######################################################################################

inline std::filesystem::path get_working_directory()
{
    return std::filesystem::current_path();
}

inline long long get_timestamp(const char *file)
{
    struct stat file_stat = {};
    stat(file, &file_stat);
    return file_stat.st_mtime;
}