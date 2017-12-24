#pragma once

#include <cassert>
#include "Log.h"

#ifdef DEBUG
#define Assert_(x) assert(x)
#define LOG_ENABLED
#else
#define Assert_(x)
#endif

enum class ELogCategory {
    Render
};

#ifdef LOG_ENABLED
#define DeclareLog_(logName) CLog s_log##logName
#define DefineLog_(logName, category, fileName, options) CLog s_log##logName(static_cast<uint32_t>(category), fileName, options)
#define DefineLogStatic_(logName, category, fileName, options) static DefineLog_(logName, category, fileName, options)
#define LogFatal_(logName, ...) s_log##logName.Log(ELogLevel::Fatal, __VA_ARGS__)
#define LogError_(logName, ...) s_log##logName.Log(ELogLevel::Error, __VA_ARGS__)
#define LogWarn_(logName, ...) s_log##logName.Log(ELogLevel::Warn, __VA_ARGS__)
#define LogInfo_(logName, ...) s_log##logName.Log(ELogLevel::Info, __VA_ARGS__)
#define LogDebug_(logName, ...) s_log##logName.Log(ELogLevel::Debug, __VA_ARGS__)
#else
#define DeclareLog_(logName)
#define DefineLog_(logName, fileName)
#define DefineLogStatic_(logName, fileName)
#define LogFatal_(logName, ...)
#define LogError_(logName, ...)
#define LogWarn_(logName, ...)
#define LogInfo_(logName, ...)
#define LogDebug_(logName, ...)
#endif