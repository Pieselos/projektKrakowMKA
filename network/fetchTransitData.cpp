#include "fetchTransitData.h"
#include <curl/curl.h>
#include <fstream>

static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    std::ofstream* out = static_cast<std::ofstream*>(userp);
    out->write(static_cast<char*>(contents), size * nmemb);
    return size * nmemb;
}

bool fetchTransitData::downloadMenager::downloadFile(const std::string& url, const std::string& outputPath) {
    CURL* curl = curl_easy_init();
    if (!curl) return false;

    std::ofstream file(outputPath, std::ios::binary);
    if (!file) return false;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &file);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    return res == CURLE_OK;
}