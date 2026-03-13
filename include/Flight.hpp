#ifndef _FLIGHT_H_
#define _FLIGHT_H_

#include <string>
#include "json.hpp"

using json = nlohmann::json;

// --- Struct 1: Master Data (Dùng cho flights.json) ---
struct FlightMaster {
    std::string flight_number; // "Biển số" máy bay (VJ101)
    std::string airline_name;

    std::string origin_code;   // HAN
    std::string origin_city;
    std::string origin_airport;

    std::string dest_code;     // SGN
    std::string dest_city;
    std::string dest_airport;

    int duration_mins;         // Thời gian bay trung bình
};

// Ánh xạ sang JSON
// inline void to_json(json& j, const FlightMaster& f) {
//     j = json{
//         {"flight_number", f.flight_number},
//         {"airline_name", f.airline_name},
//         {"origin_city", f.origin_city},
//         {"origin_airport", f.origin_airport},
//         {"destination_city", f.dest_city},
//         {"destination_airport", f.dest_airport},
//         {"duration_minutes", f.duration_mins}
//     };
// }
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(FlightMaster, 
    flight_number, airline_name, 
    origin_city, origin_airport, 
    dest_city, dest_airport,
    duration_mins
)

// --- Struct 2: Transaction Data (Dùng cho price_history.json) ---
struct FlightInstance {
    std::string flight_number; // KHỚP VỚI Struct 1
    std::string flight_id;     // Mã định danh duy nhất cho chuyến bay cụ thể
    std::string departure_time;
    std::string arrival_time;
    long long base_fare;
    long long taxes_fees;
    long long total_fare;
    std::string seat_class;
    //Thêm vào thử
    std::string origin_city;
    std::string origin_airport;
    std::string destination_city;
    std::string destination_airport;
};

// Ánh xạ sang JSON
// inline void to_json(json& j, const FlightInstance& f) {
//     j = json{
//         {"flight_number", f.flight_number},
//         {"flight_id", f.flight_id},
//         {"departure_time", f.departure_time},
//         {"arrival_time", f.arrival_time},
//         {"base_fare", f.base_fare},
//         {"taxes_fees", f.taxes_fees},
//         {"total_fare", f.total_fare},
//         {"seat_class", f.seat_class}
//     };
// }
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(FlightInstance, 
    flight_id, flight_number, 
    origin_city, origin_airport, destination_city, destination_airport, 
    departure_time, arrival_time, seat_class, 
    base_fare, taxes_fees, total_fare
)

#endif