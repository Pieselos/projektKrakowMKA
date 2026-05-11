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

        // service_id
        next = line.find(',', pos);
        std::string serviceId = line.substr(pos, next - pos);
        pos = next + 1;

        // trip_id
        next = line.find(',', pos);
        std::string tripStr = line.substr(pos, next - pos);
        pos = next + 1;

        // headsign
        std::string headsign;
        if (pos < line.size())
            headsign = line.substr(pos);


        if (!tripIdMap.contains(tripStr)) {
            continue;
        }

        int tripId = tripIdMap[tripStr];

        int routeId = routeIdMap.try_emplace(routeStr, routeIdMap.size()).first->second;

        trips[tripId] = {routeId, headsign, serviceId};
    }

    std::cout << "Loaded trips" << std::endl;
}

void engine::ScheduleEngine::loadTrips(const std::string& path)
{
    engine::loadTrips(path, trips, tripIdMap, routeIdMap);
}


void engine::loadStops(
    const std::string& path,
    std::unordered_map<int, engine::StopInfo>& stops,
    std::unordered_map<std::string, int>& stopIdMap)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open stops.txt");
    }

    std::string line;
    std::getline(file, line); // header

    while (std::getline(file, line))
    {
        size_t pos = 0;

        // stop_id
        size_t next = line.find(',', pos);
        std::string stopStr = line.substr(pos, next - pos);
        pos = next + 1;

        // name
        next = line.find(',', pos);
        std::string name = line.substr(pos, next - pos);
        pos = next + 1;

        // lat
        next = line.find(',', pos);
        double lat = std::stod(line.substr(pos, next - pos));
        pos = next + 1;

        // lon
        double lon = std::stod(line.substr(pos));

        int stopId = stopIdMap.try_emplace(stopStr, stopIdMap.size()).first->second;

        stops[stopId] = {name, lat, lon};
    }

    std::cout << "Loaded stops" << std::endl;
}

void engine::ScheduleEngine::loadStops(const std::string& path)
{
    engine::loadStops(path, stops, stopIdMap);
}


void engine::loadRoutes(
    const std::string& path,
    std::unordered_map<int, engine::RouteInfo>& routes,
    std::unordered_map<std::string, int>& routeIdMap)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open routes.txt");
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

        // agency_id (skip)
        next = line.find(',', pos);
        pos = next + 1;

        // route_short_name (numer linii)
        next = line.find(',', pos);
        std::string shortName = line.substr(pos, next - pos);
        pos = next + 1;

        // route_long_name (skip)
        next = line.find(',', pos);
        pos = next + 1;

        // route_desc (skip)
        next = line.find(',', pos);
        pos = next + 1;

        // route_type
        next = line.find(',', pos);
        std::string typeStr = line.substr(pos, next - pos);

        int routeType = std::stoi(typeStr);

        // mapowanie string → int
        int routeId = routeIdMap.try_emplace(routeStr, routeIdMap.size()).first->second;

        routes[routeId] = {
            routeType,
            shortName
        };
    }

    std::cout << "Loaded routes" << std::endl;
}

void engine::ScheduleEngine::loadRoutes(const std::string& path)
{
    engine::loadRoutes(path, routes, routeIdMap);
}


std::string getTodayDate()
{
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);

    std::tm localTime{};
    localtime_r(&t, &localTime);

    std::ostringstream oss;
    oss << std::put_time(&localTime, "%Y%m%d");

    return oss.str();
}

void loadCalendarDates(
    const std::string& path,
    std::unordered_set<std::string>& activeServices)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open calendar_dates.txt");
    }

    std::string today = getTodayDate();

    std::string line;
    std::getline(file, line); // header

    while (std::getline(file, line))
    {
        size_t pos = 0;

        // service_id
        size_t next = line.find(',', pos);
        std::string serviceId = line.substr(pos, next - pos);
        pos = next + 1;

        // date
        next = line.find(',', pos);
        std::string dateStr = line.substr(pos, next - pos);
        pos = next + 1;

        // exception_type
        std::string typeStr = line.substr(pos);
        int type = std::stoi(typeStr);

        if (dateStr == today)
        {
            if (type == 1) {
                activeServices.insert(serviceId);
            }
            else if (type == 2) {
                activeServices.erase(serviceId);
            }
        }
    }

    std::cout << "Loaded calendar_dates " << std::endl;
}

void engine::ScheduleEngine::loadCalendarDates(const std::string& path)
{
    engine::loadCalendarDates(path, activeServices);
}


bool saveBinary(
    const std::string& path,
    const std::unordered_map<int, std::vector<engine::StopTime>>& departuresByStop,
    const std::unordered_map<int, engine::TripInfo>& trips,
    const std::unordered_map<int, engine::RouteInfo>& routes)
{
    std::ofstream out(path, std::ios::binary);
    if (!out) throw std::runtime_error("Cannot open binary file");

    // HEADER
    engine::BinHeader header{BIN_MAGIC, BIN_VERSION};
    out.write((char*)&header, sizeof(header));

    // STOP TIMES
    int stopCount = departuresByStop.size();
    out.write((char*)&stopCount, sizeof(stopCount));

    for (const auto& [stopId, vec] : departuresByStop)
    {
        out.write((char*)&stopId, sizeof(stopId));

        int size = vec.size();
        out.write((char*)&size, sizeof(size));

        out.write((char*)vec.data(), size * sizeof(engine::StopTime));
    }

    // TRIPS
    int tripCount = trips.size();
    out.write((char*)&tripCount, sizeof(tripCount));

    for (const auto& [tripId, t] : trips)
    {
        out.write((char*)&tripId, sizeof(tripId));
        out.write((char*)&t.routeId, sizeof(t.routeId));

        // headsign
        uint32_t len = t.headsign.size();
        out.write((char*)&len, sizeof(len));
        out.write(t.headsign.data(), len);

        // serviceId
        len = t.serviceId.size();
        out.write((char*)&len, sizeof(len));
        out.write(t.serviceId.data(), len);
    }

    // ROUTES
    int routeCount = routes.size();
    out.write((char*)&routeCount, sizeof(routeCount));

    for (const auto& [routeId, r] : routes)
    {
        out.write((char*)&routeId, sizeof(routeId));
        out.write((char*)&r.type, sizeof(r.type));

        uint32_t len = r.shortName.size();
        out.write((char*)&len, sizeof(len));
        out.write(r.shortName.data(), len);
    }

    std::cout << "Saved binary cache" << std::endl;
    return true;
}


bool loadBinary(
    const std::string& path,
    std::unordered_map<int, std::vector<engine::StopTime>>& departuresByStop,
    std::unordered_map<int, engine::TripInfo>& trips,
    std::unordered_map<int, engine::RouteInfo>& routes)
{
    std::ifstream in(path, std::ios::binary);
    if (!in) throw std::runtime_error("Cannot open binary file");

    //  HEADER
    engine::BinHeader header;
    in.read((char*)&header, sizeof(header));

    if (header.magic != BIN_MAGIC) {
        throw std::runtime_error("Invalid binary file");
    }

    if (header.version != BIN_VERSION) {
        throw std::runtime_error("Unsupported binary version");
    }

    // STOP TIMES
    int stopCount;
    in.read((char*)&stopCount, sizeof(stopCount));

    for (int i = 0; i < stopCount; i++)
    {
        int stopId;
        in.read((char*)&stopId, sizeof(stopId));

        int size;
        in.read((char*)&size, sizeof(size));

        std::vector<engine::StopTime> vec(size);
        in.read((char*)vec.data(), size * sizeof(engine::StopTime));

        departuresByStop[stopId] = std::move(vec);
    }

    // TRIPS
    int tripCount;
    in.read((char*)&tripCount, sizeof(tripCount));

    for (int i = 0; i < tripCount; i++)
    {
        int tripId;
        in.read((char*)&tripId, sizeof(tripId));

        engine::TripInfo t;

        in.read((char*)&t.routeId, sizeof(t.routeId));

        uint32_t len;

        // headsign
        in.read((char*)&len, sizeof(len));
        t.headsign.resize(len);
        in.read(t.headsign.data(), len);

        // serviceId
        in.read((char*)&len, sizeof(len));
        t.serviceId.resize(len);
        in.read(t.serviceId.data(), len);

        trips[tripId] = std::move(t);
    }

    // ROUTES
    int routeCount;
    in.read((char*)&routeCount, sizeof(routeCount));

    for (int i = 0; i < routeCount; i++)
    {
        int routeId;
        in.read((char*)&routeId, sizeof(routeId));

        engine::RouteInfo r;

        in.read((char*)&r.type, sizeof(r.type));

        uint32_t len;
        in.read((char*)&len, sizeof(len));
        r.shortName.resize(len);
        in.read(r.shortName.data(), len);

        routes[routeId] = std::move(r);
    }

    std::cout << "Loaded binary cache" << std::endl;
    return true;
}


bool ScheduleEngine::saveBinary(const std::string& path)
{
    return engine::saveBinary(path, departuresByStop, trips, routes);
}

bool ScheduleEngine::loadBinary(const std::string& path)
{
    return engine::loadBinary(path, departuresByStop, trips, routes);
}