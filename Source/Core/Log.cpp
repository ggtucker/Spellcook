#include "Log.h"
#include <fstream>

CLog::CLog (uint32_t category, const char* fileName, uint32_t logOptions) :
    m_category{category},
    m_fileName{fileName},
    m_logOptions{logOptions} {
}

CLog::~CLog () {
    m_fileStream.close();
}

void CLog::EnsureLogFileIsOpen () {
    if (!m_fileStream.is_open()) {
        m_fileStream.open(m_fileName, std::ofstream::out | (m_logOptions & e_logOptionAppend ? std::ofstream::app : 0));
    }
}