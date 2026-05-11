//
// Created by jds on 7.05.2026.
//

#ifndef PROJEKTKRAKOWMKA_ENGINE_H
#define PROJEKTKRAKOWMKA_ENGINE_H
#include "dll_export.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <unordered_set>
#include <chrono>
#include <sstream>
#include <iomanip>

#define BIN_VERSION 1
#define BIN_MAGIC 0x47465453  // "GTFS"

namespace engine
{

    enum class TransportType {
        BUS,
        TRAM,
        OTHER
    };

    struct RouteInfo {
        int type; // 3 / 900
        std::string shortName; // "52"
    };

    struct StopTime {
        int departureTime;
        int tripId;
        int stopSequence;
    };

    struct TripInfo {
        int routeId;
        std::string headsign;
        std::string serviceId;
    };

    struct StopInfo {
        std::string name;
        double lat;
        double lon;
    };

    struct BinHeader {
        uint32_t magic;
        uint32_t version;
    };






    TransportType mapRouteType(int type);

    class ScheduleEngine {
    private:
        std::unordered_map<int, std::vector<StopTime>> departuresByStop;
        std::unordered_map<std::string, int> tripIdMap;
        std::unordered_map<std::string, int> stopIdMap;
        std::unordered_map<std::string, int> routeIdMap;
        std::unordered_map<int, TripInfo> trips;
        std::unordered_map<int, RouteInfo> routes;
        std::unordered_map<int, StopInfo> stops;
        std::unordered_set<std::string> activeServices;

    public:
        void loadStopTimes(const std::string& path);
        void loadTrips(const std::string& path);
        void loadStops(const std::string& path);
        void loadRoutes(const std::string& path);
        void loadCalendarDates(const std::string& path);
        bool saveBinary(const std::string& path);
        bool loadBinary(const std::string& path);
    };


    int parseTime(const std::string& time);


    void loadStopTimes(
        const std::string& path,
        std::unordered_map<int, std::vector<StopTime>>& departuresByStop,
        std::unordered_map<std::string, int>& tripIdMap,
        std::unordered_map<std::string, int>& stopIdMap);


    void loadTrips(
    const std::string& path,
    std::unordered_map<int, TripInfo>& trips,
    std::unordered_map<std::string, int>& tripIdMap,
    std::unordered_map<std::string, int>& routeIdMap);

    void loadStops(
    const std::string& path,
    std::unordered_map<int, StopInfo>& stops,
    std::unordered_map<std::string, int>& stopIdMap);


    void loadRoutes(
    const std::string& path,
    std::unordered_map<int, RouteInfo>& routes,
    std::unordered_map<std::string, int>& routeIdMap
    );

    void loadCalendarDates(
    const std::string& path,
    std::unordered_set<std::string>& activeServices
    );



    bool loadBinary(
        const std::string& path,
        std::unordered_map<int, std::vector<engine::StopTime>>& departuresByStop,
        std::unordered_map<int, engine::TripInfo>& trips,
        std::unordered_map<int, engine::RouteInfo>& routes);

    bool saveBinary(
       const std::string& path,
       std::unordered_map<int, std::vector<engine::StopTime>>& departuresByStop,
       std::unordered_map<int, engine::TripInfo>& trips,
       std::unordered_map<int, engine::RouteInfo>& routes);



}



#endif //PROJEKTKRAKOWMKA_ENGINE_H
