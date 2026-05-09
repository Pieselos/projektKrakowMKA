#pragma once
#include <string>

namespace fetchTransitData
{
    class downloadMenager
    {
    public:
        static bool downloadFile(const std::string& url, const std::string& outputPath);
    };

}