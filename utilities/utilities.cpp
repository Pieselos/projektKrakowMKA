//
// Created by jds on 9.05.2026.
//

#include "utilities.h"
#include <filesystem>
#include <chrono>
#include <iostream>
#include "miniz.h"



long utilities::hoursSince(const std::string& path) {
    namespace fs = std::filesystem;

    auto file_time = fs::last_write_time(path);

    auto file_sys = std::chrono::clock_cast<std::chrono::system_clock>(file_time);


    auto hours = std::chrono::duration_cast<std::chrono::hours>(
        std::chrono::system_clock::now() - file_sys
    ).count();
    return hours;
}

bool utilities::checkZipFileIntegrity(const std::string& path)
{
    mz_zip_archive zip{};

    if (!mz_zip_reader_init_file(&zip, path.c_str(),0))
        return false;


    int fileCount = mz_zip_reader_get_num_files(&zip);

    mz_zip_reader_end(&zip);

    return fileCount > 0;

}


bool utilities::unzipAll(const std::string& zipPath, const std::string& outputDir) {
    mz_zip_archive zip{};

    if (!mz_zip_reader_init_file(&zip, zipPath.c_str(), 0)) {
       throw std::runtime_error("Failed to open zip file");
    }

    int fileCount = mz_zip_reader_get_num_files(&zip);

    for (int i = 0; i < fileCount; i++) {
        mz_zip_archive_file_stat file_stat;

        if (!mz_zip_reader_file_stat(&zip, i, &file_stat)) {
            mz_zip_reader_end(&zip);
            throw std::runtime_error("Failed to read file stat");
        }

        std::string filename = file_stat.m_filename;
        std::string outPath = outputDir + "/" + filename;

        // tworzenie katalogów jeśli potrzeba
        std::filesystem::create_directories(
            std::filesystem::path(outPath).parent_path()
        );

        if (!mz_zip_reader_extract_to_file(&zip, i, outPath.c_str(), 0)) {
            mz_zip_reader_end(&zip);
            throw std::runtime_error("Failed to extract file");
        }

        std::cout << "Extracted: " << filename << "\n";
    }

    mz_zip_reader_end(&zip);
    return true;
}


std::string utilities::RouteTypeToString(engine::TransportType type)
{

    if (type == engine::TransportType::BUS) return "Bus";
    if (type == engine::TransportType::TRAM) return "Tramwaj";

    return "Other";

}

void utilities::displayDepartures(std::vector<engine::Departure>& departures)
{
    for (auto dep : departures)
       std::cout << dep << std::endl;
}


std::ostream& operator<<(std::ostream& os, const engine::Departure& obj)
{
    return os << obj.time << " " << obj.line << " " << obj.destination << " " << obj.type;
}


std::ostream& operator<<(std::ostream& os, const engine::TransportType type)
{
    return os << utilities::RouteTypeToString(type);
}


