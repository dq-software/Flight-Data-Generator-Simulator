#include <iostream>
#include <fstream>
#include "../include/DataGenerator.hpp"

// Cấu hình hiển thị UTF-8 trên Windows
#ifdef _WIN32
    #include <windows.h>
#endif

int main() {
    #ifdef _WIN32
        SetConsoleOutputCP(65001);
    #endif

    std::cout << "--- CHUONG TRINH TẠO DU LIEU BIG DATA GIA LAP ---" << std::endl;
    
    DataGenerator gen;
    int soLuongMayBay = 1500;

    // --- BƯỚC 1: TẠO FILE FLIGHTS.JSON ---
    std::cout << "1. Dang sinh 1500 chuyen bay co dinh (Master Data)..." << std::endl;
    auto masterData = gen.generateMasterData(soLuongMayBay);
    
    json j_masters = masterData;
    std::ofstream f1("data/flights.json");
    f1 << j_masters.dump(4);
    f1.close();
    std::cout << "=> Da xong 'flights.json'" << std::endl;

    // --- BƯỚC 2: TẠO FILE PRICE_HISTORY.JSON ---
    std::cout << "2. Dang sinh lich su gia 30 ngay (Transaction Data)..." << std::endl;
    std::cout << "   Du kien sinh: " << soLuongMayBay * 30 * 2 << " dong du lieu." << std::endl;
    
    auto historyData = gen.generatePriceHistory(masterData, 11, 2025); // Tháng 11/2025
    
    json j_history = historyData;
    std::ofstream f2("data/price_history.json");
    f2 << j_history.dump(4); 
    f2.close();
    
    std::cout << "=> Da xong 'price_history.json'" << std::endl;
    std::cout << "--- HOAN TAT ---" << std::endl;

    return 0;
}