#pragma once

#include "CoreTypes.h"
#include <fstream>
#include <sstream>

enum ELogOption {
    e_logOptionAppend = 1 << 0
};

enum class ELogLevel {
    Fatal,
    Error,
    Warn,
    Info,
    Debug
};

class CLog {
public:
    CLog (uint32_t category, const char* fileName, uint32_t logOptions);

    virtual ~CLog ();

    template <typename... TArgs>
    void Log (ELogLevel logLevel, TArgs&&... args) {
        EnsureLogFileIsOpen();
        std::ostringstream oss;
        InternalLog(oss, std::forward<TArgs>(args)...);
        m_fileStream << oss.str() << std::endl;
    }

private:
    uint32_t m_category{};
    const char* m_fileName{};
    uint32_t m_logOptions{};
    std::ofstream m_fileStream{};

    void EnsureLogFileIsOpen ();

    template <typename T>
    static void InternalLog (std::ostream& out, T&& t) { out << t; }

    template <typename T, typename... TArgs>
    static void InternalLog (std::ostream& out, T&& t, TArgs&&... args) { InternalLog(out, t); InternalLog(out, std::forward<TArgs>(args)...); }
};

