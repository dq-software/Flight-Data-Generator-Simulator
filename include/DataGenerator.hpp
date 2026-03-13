#ifndef _DATAGENERATOR_H_
#define _DATAGENERATOR_H_

#include <vector>
#include "Flight.hpp"
#include "Utils.hpp"

struct AirportInfo{
    std::string city;
    std::string airport;
    std::string code;
};

class DataGenerator {
private:
    Utils utils;
    // Dữ liệu mẫu cứng
    // Giả sử dùng 11 sân bay quốc tế tại Việt Nam
    std::vector<AirportInfo> airports = {
        {"TP Hồ Chí Minh", "Tân Sơn Nhất (SGN)", "SGN"},
        {"Hà Nội", "Nội Bài (HAN)", "HAN"},
        {"Đà Nẵng", "Đà Nẵng (DAD)", "DAD"},
        {"Hải Phòng", "Cát Bi (HPH)", "HPH"},
        {"Phú Quốc", "Phú Quốc (PQC)", "PQC"},
        {"Nha Trang", "Cam Ranh (CXR)", "CXR"},
        {"Đà Lạt", "Liên Khương (DLI)", "DLI"},
        {"Quảng Ninh", "Vân Đồn (VDO)", "VDO"},
        {"Vinh", "Vinh (VII)", "VII"},
        {"Huế", "Phú Bài (HUI)", "HUI"},
        {"Cần Thơ", "Cần Thơ (VCA)", "VCA"}
    };
    std::vector<std::pair<std::string, std::string>> airlines = {
        {"VJ", "Vietjet Air"}, {"VN", "Vietnam Airlines"}, {"QH", "Bamboo Airways"}, {"VU", "Vietravel Airlines"}
    };

public:
    // Bước 1: Tạo danh sách máy bay/tuyến bay cố định (danh bạ các chuyến bay)
    std::vector<FlightMaster> generateMasterData(int totalFlights);

    // Bước 2: Từ danh sách trên, sinh ra lịch sử giá 30 ngày
    std::vector<FlightInstance> generatePriceHistory(const std::vector<FlightMaster>& masters, int month, int year);
};

#endif