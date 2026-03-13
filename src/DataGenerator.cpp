#include "../include/DataGenerator.hpp"
#include <iostream>

// --- Triển khai logic tạo Master Data ---
std::vector<FlightMaster> DataGenerator::generateMasterData(int totalFlights) {
    std::vector<FlightMaster> masters;
    
    for (int i = 1; i <= totalFlights; ++i) {
        FlightMaster f;
        
        // Random hãng bay
        auto airline = utils.getRandomElement(airlines);
        std::string code = airline.first;
        f.airline_name = airline.second;

        // Tạo flight_number (Số hiệu chuyến bay): VD: VJ1001, VN1002...
        f.flight_number = code + std::to_string(1000 + i);

        // Random tuyến bay (đảm bảo đi != đến)
        AirportInfo origin = utils.getRandomElement(airports);
        AirportInfo dest;
        do {
            dest = utils.getRandomElement(airports);
        } while (dest.code == origin.code);
        
        f.origin_airport = origin.airport;
        f.origin_city = origin.city;
        f.origin_code = origin.code;

        f.dest_airport = dest.airport;
        f.dest_city = dest.city;
        f.dest_code = dest.code;

        // Logic thời gian bay giả định
        // Dùng biến tạm để làm gọn code
        std::string from = f.origin_code;
        std::string to = f.dest_code;
        int mins = 0;
        
        // 1. Tuyến trục chính: Hà Nội (HAN) <-> HCM (SGN)
        // Thời gian thực tế ~2h10p (130p) đến 2h20p
        if ((from == "HAN" && to == "SGN") || 
            (from == "SGN" && to == "HAN")) {
            mins = utils.getInt(125, 145); 
        }

        // 2. Tuyến miền Trung: Hà Nội (HAN) <-> Đà Nẵng (DAD)
        // Thời gian thực tế ~1h20p
        else if ((from == "HAN" && to == "DAD") || 
            (from == "DAD" && to == "HAN")) {
            mins = utils.getInt(75, 90);
        }

        // 3. Tuyến ngắn: HCM (SGN) <-> Đà Lạt (DLI), Nha Trang (CXR), Phú Quốc (PQC)
        // Thời gian thực tế ~50p - 1h
        else if ((from == "SGN" && (to == "DLI" || to == "CXR" || to == "PQC")) ||
            ((from == "DLI" || from == "CXR" || from == "PQC") && to == "SGN")) {
            mins = utils.getInt(50, 65);
        }
        
        // 4. Tuyến dài khác: Hà Nội <-> Phú Quốc, Cần Thơ (xa tương đương đi HCM)
        else if ((from == "HAN" && (to == "PQC" || to == "VCA")) ||
            ((from == "PQC" || from == "VCA") && to == "HAN")) {
            mins = utils.getInt(125, 140);
        }

        // 5. Mặc định (Các tuyến còn lại)
        // Cho dao động từ 60 đến 100 phút
        else {
            mins = utils.getInt(60, 100);
        }

        f.duration_mins = mins;

        masters.push_back(f);
    }
    return masters;
}

// --- Triển khai logic tạo Price History (Big Data) ---
std::vector<FlightInstance> DataGenerator::generatePriceHistory(const std::vector<FlightMaster>& masters, int month, int year) {
    std::vector<FlightInstance> history;
    int daysInMonth = utils.getDaysInMonth(month, year); //Lấy số ngày trong tháng
    
    // Duyệt qua từng chiếc máy bay (Master)
    for (const auto& master : masters) {
        
        // Duyệt qua số ngày
        for (int day = 1; day <= daysInMonth; ++day) {
            
            // --- 1. Thiết lập thông tin cơ bản cho ngày hôm đó ---
            // 1. Tạo struct thời gian cho Giờ Đi (Departure)
            std::tm tmDep = {};
            tmDep.tm_year = year - 1900; // C++ tính năm từ 1900
            tmDep.tm_mon = month - 1;    // Tháng chạy từ 0-11
            tmDep.tm_mday = day;
            tmDep.tm_hour = utils.getInt(5, 22);
            tmDep.tm_min = utils.getInt(0, 59);
            tmDep.tm_sec = 0;
            tmDep.tm_isdst = -1; // Để hệ thống tự xác định

            // 2. Đổi Giờ Đi ra dạng Timestamp (Tổng số giây)
            std::time_t tsDep = std::mktime(&tmDep);

            // 3. Tính Giờ Đến (Arrival Timestamp) = Giờ Đi + (Số phút bay * 60)
            std::time_t tsArr = tsDep + (master.duration_mins * 60);

            // 4. Đổi ngược Timestamp thành Ngày/Giờ
            std::tm* tmArr = std::localtime(&tsArr);

            // 5. Tạo chuỗi format chuẩn ISO 8601 (YYYY-MM-DDTHH:MM:SS)
            char depatureBuf[30];
            std::strftime(depatureBuf, sizeof(depatureBuf), "%Y-%m-%dT%H:%M:%S", &tmDep);

            std::string departure = std::string(depatureBuf);

            char arrivalBuf[30];
            std::strftime(arrivalBuf, sizeof(arrivalBuf), "%Y-%m-%dT%H:%M:%S", tmArr);
                
            std::string arrival = std::string(arrivalBuf);

            // --- 2. Sinh dữ liệu cho 2 hạng ghế (Economy & Business) ---
            std::vector<std::string> classes = {"Economy", "Business"};
            
            // Tính giá gốc dựa trên thời gian bay (Logic: 5000đ/phút)
            long long standard_base = 500000 + (master.duration_mins * 5000);
            
            // Biến động ngẫu nhiên +/- 5% cho ngày hôm đó
            double daily_fluctuation = utils.getDouble(0.95, 1.05);
            long long daily_price = (long long)(standard_base * daily_fluctuation);

            for (const auto& seat_class : classes) {
                FlightInstance inst;
                
                // Flight Number phải khớp
                inst.flight_number = master.flight_number;

                // 2. [FIX QUAN TRỌNG] Copy thông tin ĐỊA ĐIỂM từ Master sang Instance
                // Nếu thiếu đoạn này, DB sẽ lưu chuỗi rỗng!
                inst.origin_city = master.origin_city;
                inst.origin_airport = master.origin_airport; // Hoặc origin_code tùy cấu trúc DB
                inst.destination_city = master.dest_city;
                inst.destination_airport = master.dest_airport;
                // Tạo ID duy nhất: FlightNum + Date + ClassCode
                if(day < 10) {
                    inst.flight_id = master.flight_number + "-0" + std::to_string(day) + "-" + seat_class.substr(0,1);
                } else {
                    inst.flight_id = master.flight_number + "-" + std::to_string(day) + "-" + seat_class.substr(0,1);
                }
                inst.departure_time = departure;
                inst.arrival_time = arrival;
                inst.seat_class = seat_class;

                // --- 3. Logic Giá ---
                long long final_base = daily_price;

                if (seat_class == "Business") {
                    // Mặc định: Thương gia đắt hơn 15-20%
                    double multiplier = utils.getDouble(1.15, 1.20);
                    
                    // Anomalies (Dữ liệu nhiễu): 1% cơ hội giá Thương gia = Thường
                    if (utils.getInt(1, 100) == 99) {
                        multiplier = 1.0; 
                    }
                    
                    final_base = (long long)(daily_price * multiplier);
                }

                inst.base_fare = final_base;
                inst.taxes_fees = (long long)(final_base * 0.1); // Thuế 10%
                inst.total_fare = inst.base_fare + inst.taxes_fees;

                history.push_back(inst);
            }
        }
    }
    return history;
}