#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "../include/Flight.hpp"
#include "../include/json.hpp"

// Cấu hình hiển thị UTF-8 trên Windows
#ifdef _WIN32
    #include <windows.h>
#endif

using json = nlohmann::json;

class ChaosGenerator {
public:
    // --- BƯỚC 1: TẠO MASTER DATA VỚI CÁC TRƯỜNG HỢP "DỊ" ---
    std::vector<FlightMaster> generateChaosRoutes() {
        std::vector<FlightMaster> routes;

        // CASE 1: UI STRESS (Tên siêu dài + Ký tự lạ)
        FlightMaster longStringFlight;
        longStringFlight.flight_number = "HKVT_001";
        longStringFlight.airline_name = "Hãng Hàng Không Vũ Trụ Liên Ngân Hà Siêu Cấp Vip Pro Luxury Premium Limited Edition (Chi Nhánh Trái Đất)";
        longStringFlight.origin_airport = "Đột Phá Không Giới Hạn (DPK)";
        longStringFlight.origin_code = "DPK";
        longStringFlight.origin_city = "Thành Phố Không Giới Hạn Siêu Cấp Vượt Qua Các Rào Cản Công Nghệ, Vũ Trụ Và Thời Gian, Đưa Con Người Đến Mức Sống Tuyệt Đỉnh";
        longStringFlight.dest_airport = "Tương Lai Siêu Tốc Đỉnh Cao (STDC)";
        longStringFlight.dest_code = "STDC";
        longStringFlight.dest_city = "Thành Phố Tương Lai Tuyệt Kỹ Đỉnh Cao, Siêu Cấp Vip Pro Max Toàn Cầu Kết Nối Vũ Trụ, Tạo Nên Những Đột Phá Công Nghệ Khổng Lồ";
        longStringFlight.duration_mins = 999999;
        routes.push_back(longStringFlight);

        longStringFlight.flight_number = "HKDC_002";
        longStringFlight.airline_name = "Hãng Hàng Không Đỉnh Cao Toàn Cầu Với Công Nghệ Mới Nhất, Dịch Vụ Hạng Vip Và Hệ Thống Bay Siêu Tốc Đưa Bạn Vượt Qua Mọi Giới Hạn";
        longStringFlight.origin_airport = "Đột Phá Không Giới Hạn (DPK)";
        longStringFlight.origin_code = "DPK";
        longStringFlight.origin_city = "Thành Phố Không Giới Hạn Siêu Cấp Vượt Qua Các Rào Cản Công Nghệ, Vũ Trụ Và Thời Gian, Đưa Con Người Đến Mức Sống Tuyệt Đỉnh";
        longStringFlight.dest_airport = "Vũ Trụ Đột Phá (VTD)";
        longStringFlight.origin_code = "VTD";
        longStringFlight.dest_city = "Thành Phố Vũ Trụ Siêu Đỉnh Siêu Cấp, Đột Phá Công Nghệ Và Giải Pháp Hòa Bình Vĩnh Cửu Đưa Con Người Đến Một Thế Giới Mới Không Thể Tưởng Tượng";
        longStringFlight.duration_mins = 230;
        routes.push_back(longStringFlight);

        longStringFlight.flight_number = "HKSDC_003";
        longStringFlight.airline_name = "Hãng Hàng Không Siêu Đẳng Cấp Amazing Ultimate Tuyệt Kỹ Vũ Trụ Siêu Cấp Vip Pro Max Luxury Best Best Never Fail";
        longStringFlight.origin_airport = "Tương Lai Tươi Sáng (TLTS)";
        longStringFlight.origin_code = "TLTS";
        longStringFlight.origin_city = "Thành Phố Siêu Đẳng Cấp Tuyệt Đỉnh, Đầy Những Đột Phá Công Nghệ Và Vũ Trụ Đưa Bạn Đến Tương Lai Tươi Sáng Vượt Mọi Giới Hạn";
        longStringFlight.dest_airport = "Hòa Bình Siêu Cấp (HBSC)";
        longStringFlight.origin_code = "HBSC";
        longStringFlight.dest_city = "Thành Phố Hòa Bình Tuyệt Đỉnh Siêu Cấp Mới Với Công Nghệ Đột Phá Tạo Ra Một Tương Lai Cải Tiến Không Thể Cản Phá, Mở Cửa Vũ Trụ Cho Mọi Người";
        longStringFlight.duration_mins = 100;
        routes.push_back(longStringFlight);

        // CASE 2: Ký tự đặc biệt & SQL Injection giả lập (Security Test)
        FlightMaster injectionFlight;
        injectionFlight.flight_number = "HACK_002";
        injectionFlight.airline_name = "Captain America'); DROP TABLE Steve Rogers;--"; // Test xem hệ thống có bị lỗi SQL không
        injectionFlight.origin_code = "<script>alert('XSS Test')</script>"; // Test XSS
        injectionFlight.origin_city = "Ha Noi";
        injectionFlight.origin_airport = "Nội Bài (HAN)";
        injectionFlight.dest_code = "SGN";
        injectionFlight.dest_city = "HCM";
        injectionFlight.dest_airport = "Tân Sơn Nhất (SGN)";
        injectionFlight.duration_mins = -1;
        routes.push_back(injectionFlight);

        injectionFlight.flight_number = "HACK_005";
        injectionFlight.airline_name = "Iron Man @/? AIRLINE|"; // Test xem hệ thống có bị lỗi SQL không
        injectionFlight.origin_code = "<script>alert('hacked')</script>"; // Test XSS
        injectionFlight.origin_city = "HCM";
        injectionFlight.origin_airport = "Tân Sơn Nhất (SGN)";
        injectionFlight.dest_code = "HAN";
        injectionFlight.dest_city = "Ha Noi";
        injectionFlight.dest_airport = "Nội Bài (HAN)";
        injectionFlight.duration_mins = 130;
        routes.push_back(injectionFlight);

        // CASE 3: Tuyến bay "Ma" (Ghost Route) - Không có chuyến bay nào
        // Chúng ta tạo route ở đây, nhưng sẽ KHÔNG tạo dữ liệu giá cho nó ở bước 2.
        FlightMaster ghostFlight;
        ghostFlight.flight_number = "GHOST_404";
        ghostFlight.airline_name = "Ghost Airlines";
        ghostFlight.origin_code = "DAD";
        ghostFlight.origin_city = "Da Nang";
        ghostFlight.dest_code = "PQC";
        ghostFlight.dest_city = "Phu Quoc";
        ghostFlight.duration_mins = 0; // Thời gian bay = 0 (Vô lý)
        routes.push_back(ghostFlight);

        return routes;
    }

    // --- BƯỚC 2: TẠO LỊCH SỬ GIÁ VỚI DỮ LIỆU SAI LOGIC ---
    std::vector<FlightInstance> generateChaosHistory(const std::vector<FlightMaster>& routes) {
        std::vector<FlightInstance> history;

        for (const auto& route : routes) {
            // Nếu là chuyến bay "Ma" -> Bỏ qua
            if (route.flight_number == "GHOST_404") continue;

            // --- CASE A: GIÁ VÉ ÂM HOẶC BẰNG 0 (Validation Test) ---
            if (route.flight_number == "HKVT_001") {
                // Giá âm
                FlightInstance badPrice;
                badPrice.flight_number = route.flight_number;
                badPrice.flight_id = "HKVT_001-ERR-PRICE";
                badPrice.departure_time = "2025-11-01T08:00:00";
                badPrice.arrival_time = "2025-11-01T10:00:00";
                
                badPrice.base_fare = -500000;
                badPrice.taxes_fees = 0;      // Thuế = 0
                badPrice.total_fare = -500000;
                badPrice.seat_class = "Economy";
                history.push_back(badPrice);
            }

            if (route.flight_number == "HKDC_002") {
                // Giá 0 đồng
                FlightInstance zeroPrice;
                zeroPrice.flight_number = route.flight_number;
                zeroPrice.flight_id = "HKDC_002-ZERO-PRICE";
                zeroPrice.departure_time = "2025-11-02T08:00:00";
                zeroPrice.arrival_time = "2025-11-02T10:00:00";
                
                zeroPrice.base_fare = 0;
                zeroPrice.taxes_fees = 0;
                zeroPrice.total_fare = 0;
                zeroPrice.seat_class = "Economy";
                history.push_back(zeroPrice);
            }

            if (route.flight_number == "HKSDC_003") {
                // Giá cực lớn (Test tràn biến long long/integer)
                FlightInstance huge;
                huge.flight_number = route.flight_number;
                huge.flight_id = "HKSDC_003-HUGE-PRICE";
                huge.departure_time = "2025-11-02T08:00:00";

                huge.base_fare = 999999999999999; 
                huge.total_fare = 999999999999999;
                huge.seat_class = "Business";
                history.push_back(huge);
            }

            // --- CASE B: NGÀY THÁNG SAI ĐỊNH DẠNG / QUÁ KHỨ / TƯƠNG LAI XA ---
            if (route.flight_number == "HACK_002") {
                // Lỗi định dạng (Format Error)
                FlightInstance badFormat;
                badFormat.flight_number = route.flight_number;
                badFormat.flight_id = "HACK_002-BAD-DATE";
                badFormat.departure_time = "Ngày hôm qua"; // String rác
                badFormat.arrival_time = "2025/13/40";     // Sai format ISO
                badFormat.total_fare = 1000000;
                badFormat.seat_class = "Business";
                history.push_back(badFormat);

                // Quá khứ xa (Ancient Past)
                FlightInstance pastDate;
                pastDate.flight_number = route.flight_number;
                pastDate.flight_id = "HACK_002-PAST";
                pastDate.departure_time = "1900-01-01T00:00:00"; 
                pastDate.arrival_time = "1900-01-01T02:00:00";
                pastDate.total_fare = 1000000;
                pastDate.seat_class = "Economy";
                history.push_back(pastDate);

                 // Tương lai xa (Far Future)
                FlightInstance futureDate;
                futureDate.flight_number = route.flight_number;
                futureDate.flight_id = "HACK_002-FUTURE";
                futureDate.departure_time = "2099-12-31T23:59:59"; 
                futureDate.arrival_time = "2100-01-01T02:00:00"; // Bay xuyên thế kỷ
                futureDate.total_fare = 1000000;
                futureDate.seat_class = "Economy";
                history.push_back(futureDate);
            }

            // Test ngày tháng "loạn lạc"
            if (route.flight_number == "HACK_005") {
                // Ngày ở tương lai năm 3000
                FlightInstance future;
                future.flight_number = route.flight_number;
                future.flight_id = "HACK-005-MESS";
                future.departure_time = "3000-01-01T00:00:00"; 
                future.arrival_time = "2025-01-01T00:00:00"; // Đến trước khi đi
                future.total_fare = 1000;
                history.push_back(future);
            }
        }
        return history;
    }
};

int main() {
    #ifdef _WIN32
        SetConsoleOutputCP(65001);
    #endif

    std::cout << "--- WARNING: DANG TAO DU LIEU \"XAU\" (STRESS DATA) ---" << std::endl;
    std::cout << "File nay chua cac du lieu loi de test he thong!" << std::endl;

    ChaosGenerator chaosGen;

    // 1. Tạo Routes
    auto chaosRoutes = chaosGen.generateChaosRoutes();
    json j_routes = chaosRoutes;
    std::ofstream f1("flights_stress_test.json");
    f1 << j_routes.dump(4);
    f1.close();
    std::cout << "-> Da tao: flights_stress_test.json (Chua ten dai, SQL Injection, Ghost Route)" << std::endl;

    // 2. Tạo History
    auto chaosHistory = chaosGen.generateChaosHistory(chaosRoutes);
    json j_history = chaosHistory;
    std::ofstream f2("price_history_stress_test.json");
    f2 << j_history.dump(4);
    f2.close();
    std::cout << "-> Da tao: price_history_stress_test.json (Chua gia am, ngay sai, gia 0 dong, ngay thang sai dinh dang hoac ngay trong qua khu xa/tuong lai xa)" << std::endl;

    return 0;
}