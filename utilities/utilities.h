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
        badFileStructure
    };

    bool checkZipFileIntegrity(const std::string& path);
}




#endif //PROJEKTKRAKOWMKA_UTILITIES_H
