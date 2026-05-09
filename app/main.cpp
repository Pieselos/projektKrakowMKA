#include <iostream>
#include "engine/engine.h"
#include "fetchTransitData.h"
#include <chrono>
#include <thread>
#include <filesystem>
#include "utilities.h"
#include <exception>

int main() {
    std::cout << "======= Start =========" << std::endl;
    std::vector<std::string> fileNamesVector = {"GTFS_KRK_A.zip","GTFS_KRK_T.zip"};
    //TODO: Wywalić wszystkie ustanienia typu nazwy plików/url do jakiegoś config.h
    int maxTriesCount = 5;
    bool downloadFlag = false;
    for (auto& s : fileNamesVector)
    {
        downloadFlag = false;
        if (!std::filesystem::exists("../data/"+s) || utilities::hoursSince("../data/"+s) > 24)
            for (int i=0; i<maxTriesCount; i++)
            {
                std::cout << "Trying to download file " << s << " Try " << i+1 << "/ " << maxTriesCount << std::endl;
                if (fetchTransitData::downloadMenager::downloadFile("https://gtfs.ztp.krakow.pl/"+s, "../data/"+s))
                {
                    std::cout << "File " << s << "downloaded successfully" << std::endl;
                    downloadFlag = true;
                    break;

                }else
                {
                    std::cout << "Download failed" << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(5));
                }
            }
        else
        {
            std::cout << "File " << s << " already exist" << std::endl;
        }
        try
            {
            if (!std::filesystem::exists("../data/" + s))
            {
                throw std::runtime_error("Failed to download file " + s);
            }
        }catch (std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            return static_cast<int>(utilities::ErrorCodes::fetchFilesError);
        }

    }
}
