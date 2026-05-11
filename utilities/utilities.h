//
// Created by jds on 9.05.2026.
//
#pragma once
#ifndef PROJEKTKRAKOWMKA_UTILITIES_H
#define PROJEKTKRAKOWMKA_UTILITIES_H
#include <string>
#include <vector>

#include "miniz.h"
#include "engine.h"

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
    void displayDepartures(std::vector<engine::Departure>& departures);
    std::string RouteTypeToString(engine::TransportType type);
}


std::ostream& operator<<(std::ostream& os, const engine::Departure& obj);
std::ostream& operator<<(std::ostream& os, const engine::TransportType type);


#endif //PROJEKTKRAKOWMKA_UTILITIES_H
