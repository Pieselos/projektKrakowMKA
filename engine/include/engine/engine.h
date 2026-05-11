//
// Created by jds on 7.05.2026.
//

#ifndef PROJEKTKRAKOWMKA_ENGINE_H
#define PROJEKTKRAKOWMKA_ENGINE_H
#include "dll_export.h"
#include <unordered_map>
#include <vector>
#include <string>



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

    public:
        void loadStopTimes(const std::string& path);
        void loadTrips(const std::string& path);

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




}



#endif //PROJEKTKRAKOWMKA_ENGINE_H
