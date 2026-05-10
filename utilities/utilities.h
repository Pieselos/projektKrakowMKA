//
// Created by jds on 9.05.2026.
//
#pragma once
#ifndef PROJEKTKRAKOWMKA_UTILITIES_H
#define PROJEKTKRAKOWMKA_UTILITIES_H
#include <string>
#include <vector>

#include "miniz.h"

namespace utilities
{
    long hoursSince(const std::string& path);
    enum class ErrorCodes
    {
        OK,
        fetchFilesError,
        badFileStructure,
        unzipError,
    };

    bool checkZipFileIntegrity(const std::string& path);
    bool unzipAll(const std::string& zipPath, const std::string& outputDir);

}




#endif //PROJEKTKRAKOWMKA_UTILITIES_H
