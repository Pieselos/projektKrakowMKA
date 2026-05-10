//
// Created by jds on 9.05.2026.
//

#include "utilities.h"
#include <filesystem>
#include <chrono>
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