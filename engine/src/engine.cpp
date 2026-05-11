//
// Created by jds on 7.05.2026.
//

#include "engine/engine.h"


#include <vector>
#include <iostream>
#include <fstream>



int engine::parseTime(const std::string& time)
{
        int h = std::stoi(time.substr(0,2));
        int m = std::stoi(time.substr(3,2));
        int s = std::stoi(time.substr(6,2));
        return h * 3600 + m * 60 + s;
}



void loadStopTimes(
    const std::string& path,
    std::unordered_map<int, std::vector<engine::StopTime>>& departuresByStop,
    std::unordered_map<std::string, int>& tripIdMap,
    std::unordered_map<std::string, int>& stopIdMap)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open stop_times.txt");
    }

    std::string line;
    std::getline(file, line); // header

    while (std::getline(file, line))
    {
        size_t pos = 0;

        // trip_id
        size_t next = line.find(',', pos);
        std::string tripStr = line.substr(pos, next - pos);
        pos = next + 1;

        // arrival_time (skip)
        next = line.find(',', pos);
        pos = next + 1;

        // departure_time
        next = line.find(',', pos);
        std::string timeStr = line.substr(pos, next - pos);
        pos = next + 1;

        // stop_id
        next = line.find(',', pos);
        std::string stopStr = line.substr(pos, next - pos);
        pos = next + 1;

        // stop_sequence
        next = line.find(',', pos);
        std::string seqStr;

        if (next == std::string::npos)
            seqStr = line.substr(pos);
        else
            seqStr = line.substr(pos, next - pos);

        int stopSequence = std::stoi(seqStr);

        // mapowanie string → int
        int tripId = tripIdMap.try_emplace(tripStr, tripIdMap.size()).first->second;
        int stopId = stopIdMap.try_emplace(stopStr, stopIdMap.size()).first->second;

        int departureTime = engine::parseTime(timeStr);

        departuresByStop[stopId].push_back({
            departureTime,
            tripId,
            stopSequence
        });
    }

    std::cout << "Loaded stop_times" << std::endl;
}


void engine::ScheduleEngine::loadStopTimes(const std::string& path)
{

    engine::loadStopTimes(path, departuresByStop, tripIdMap, stopIdMap);
}

engine::TransportType engine::mapRouteType(int type)
{

    if (type == 3) return TransportType::BUS;
    if (type == 900) return TransportType::TRAM;

    return TransportType::OTHER;

}


void loadTrips(
    const std::string& path,
    std::unordered_map<int, engine::TripInfo>& trips,
    std::unordered_map<std::string, int>& tripIdMap,
    std::unordered_map<std::string, int>& routeIdMap)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open trips.txt");
    }

    std::string line;
    std::getline(file, line); // header

    while (std::getline(file, line))
    {
        size_t pos = 0;

        // route_id
        size_t next = line.find(',', pos);
        std::string routeStr = line.substr(pos, next - pos);
        pos = next + 1;

        // service_id (skip)
        next = line.find(',', pos);
        pos = next + 1;

        // trip_id
        next = line.find(',', pos);
        std::string tripStr = line.substr(pos, next - pos);
        pos = next + 1;

        // headsign (optional)
        std::string headsign;
        if (pos < line.size())
            headsign = line.substr(pos);

        int tripId = tripIdMap[tripStr];
        int routeId = routeIdMap.try_emplace(routeStr, routeIdMap.size()).first->second;

        trips[tripId] = {routeId, headsign};
    }

    std::cout << "Loaded trips ✅\n";
}

void engine::ScheduleEngine::loadTrips(const std::string& path)
{
    engine::loadTrips(path, trips, tripIdMap, routeIdMap);
}


