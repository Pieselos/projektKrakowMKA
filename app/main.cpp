#include <iostream>
#include "engine/engine.h"
#include "fetchTransitData.h"
#include <chrono>
#include <thread>
#include <filesystem>
#include "utilities.h"
#include <exception>
#include "config.h"

int main() {
    std::cout << "======= Start =========\n";

    std::vector<std::string> fileNamesVector = {
        "GTFS_KRK_A.zip",
        "GTFS_KRK_T.zip"
    };

    if (!std::filesystem::exists("../data/")) {
        std::cerr << "Bad file structure. Aborting program\n";
        return static_cast<int>(utilities::ErrorCodes::badFileStructure);
    }

    for (const auto& s : fileNamesVector)
    {
        bool downloadFlag = false;
        std::string path = "../data/" + s;

        if (!std::filesystem::exists(path) || utilities::hoursSince(path) > 24)
        {
            for (int i = 0; i < MAX_GTFS_DOWNLOAD_RETRY; i++)
            {
                std::cout << "Trying to download file "
                          << s << " Try " << i+1
                          << "/" << MAX_GTFS_DOWNLOAD_RETRY << "\n";

                if (fetchTransitData::downloadMenager::downloadFile(GTFS_URL + s, path))
                {
                    std::cout << "Checking zip file integrity: " << s << "\n";

                    if (utilities::checkZipFileIntegrity(path))
                    {
                        std::cout << "File " << s << " downloaded successfully ✅\n";
                        downloadFlag = true;
                        break;
                    }
                    else
                    {
                        std::cout << "File corrupted/empty. Removing...\n";
                        std::filesystem::remove(path);
                    }
                }
                else
                {
                    std::cout << "Download failed ❌\n";
                    std::this_thread::sleep_for(std::chrono::seconds(5));
                }
            }
        }
        else
        {
            std::cout << "File " << s << " already exists\n";

            if (utilities::checkZipFileIntegrity(path)) {
                downloadFlag = true;
            } else {
                std::cout << "Existing file corrupted. Removing...\n";
                std::filesystem::remove(path);
            }
        }

        // ✅ FINALNA WALIDACJA
        if (!downloadFlag ||
            !std::filesystem::exists(path) ||
            std::filesystem::file_size(path) == 0)
        {
            std::cerr << "Failed to download file " << s << "\n";
            return static_cast<int>(utilities::ErrorCodes::fetchFilesError);
        }
    }
}
