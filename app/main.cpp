#include <iostream>
#include "engine/engine.h"
#include "fetchTransitData.h"

int main() {
    std::cout << "=== TEST START ===\n";

    // ✅ TEST 1: ENGINE
    engine::ScheduleEngine engine;
    std::cout << "Engine created\n";

    // auto deps = engine.getDepartures();
    engine.getDepartures();
    // std::cout << "Engine returned: " << engine.getDepartures() << " departures\n";

    // ✅ TEST 2: NETWORK
    std::cout << "Downloading test file...\n";

    bool ok = fetchTransitData(
        "https://www.google.com",
        "../data/test.html"
    );

    if (ok) {
        std::cout << "Download OK ✅\n";
    } else {
        std::cout << "Download FAILED ❌\n";
    }

    std::cout << "=== TEST END ===\n";

    return 0;
}
