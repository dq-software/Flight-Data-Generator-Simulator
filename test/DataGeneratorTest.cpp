#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <map>
#include <set>

// Include các file nguồn của dự án
// Lưu ý: Đường dẫn này phụ thuộc vào nơi bạn đứng khi gõ lệnh compile
// Ở đây giả định ta compile từ thư mục DataGenerator/
#include "../include/DataGenerator.hpp" 
#include "../include/Flight.hpp"

// --- LỚP TEST FIXTURE (Môi trường kiểm thử) ---
class FlightDataTest : public ::testing::Test {
protected:
    DataGenerator generator;
    std::vector<FlightMaster> masterData;
    int sampleSize = 100;

    // Hàm này chạy trước mỗi bài test
    void SetUp() override {
        // Sinh ra 100 chuyến bay mẫu để kiểm tra
        masterData = generator.generateMasterData(sampleSize);
    }
    
    // Hàm này chạy sau mỗi bài test (dọn dẹp nếu cần)
    void TearDown() override {
        masterData.clear();
    }
};

// --- BÀI TEST 1: KIỂM TRA TÍNH TOÀN VẸN DỮ LIỆU CƠ BẢN ---
TEST_F(FlightDataTest, CheckDataSizeAndStructure) {
    ASSERT_EQ(masterData.size(), sampleSize) << "So luong chuyen bay sinh ra khong du";
    
    for (const auto& flight : masterData) {
        EXPECT_FALSE(flight.flight_number.empty()) << "Flight Number khong duoc rong";
        EXPECT_FALSE(flight.airline_name.empty()) << "Ten hang bay khong duoc rong";
        EXPECT_GT(flight.duration_mins, 0) << "Thoi gian bay phai lon hon 0";
    }
}

// --- BÀI TEST 2: KIỂM TRA LOGIC SÂN BAY & THÀNH PHỐ (QUAN TRỌNG) ---
TEST_F(FlightDataTest, CheckAirportMapping) {
    // Bảng dữ liệu chuẩn (Truth Table) để so sánh
    std::map<std::string, std::string> expectedMap = {
        {"HAN", "Hà Nội"},
        {"SGN", "TP Hồ Chí Minh"},
        {"DAD", "Đà Nẵng"},
        {"PQC", "Phú Quốc"},
        {"CXR", "Nha Trang"},
        {"HPH", "Hải Phòng"},
        {"VCA", "Cần Thơ"},
        {"DLI", "Đà Lạt"},
        {"VDO", "Quảng Ninh"},
        {"VII", "Vinh"},
        {"HUI", "Huế"}
    };

    for (const auto& flight : masterData) {
        // 1. Kiểm tra Origin
        // Tìm xem code (VD: HAN) có trong bảng chuẩn không
        auto itOrigin = expectedMap.find(flight.origin_code);
        ASSERT_NE(itOrigin, expectedMap.end()) << "San bay di la: " << flight.origin_code << " khong nam trong danh sach ho tro";
        
        // Kiểm tra xem City sinh ra có khớp với Code không
        EXPECT_EQ(flight.origin_city, itOrigin->second) 
            << "Loi Logic: Code " << flight.origin_code << " nhung City lai la " << flight.origin_city;

        // 2. Kiểm tra Destination (tương tự)
        auto itDest = expectedMap.find(flight.dest_code);
        ASSERT_NE(itDest, expectedMap.end()) << "San bay den la: " << flight.dest_code << " khong nam trong danh sach ho tro";
        EXPECT_EQ(flight.dest_city, itDest->second)
            << "Loi Logic: Code " << flight.dest_code << " nhung City lai la " << flight.dest_city;
    }
}

// --- BÀI TEST 3: KIỂM TRA LOGIC TUYẾN BAY ---
TEST_F(FlightDataTest, CheckRouteLogic) {
    for (const auto& flight : masterData) {
        // Điểm đi phải khác điểm đến
        EXPECT_STRNE(flight.origin_code.c_str(), flight.dest_code.c_str()) 
            << "Loi nghiem trong: May bay bay tai cho (" << flight.origin_code << " -> " << flight.dest_code << ")";
    }
}

// --- BÀI TEST 4: KIỂM TRA HÃNG BAY ---
TEST_F(FlightDataTest, CheckAirlineValidity) {
    std::set<std::string> validAirlines = {
        "Vietjet Air", "Vietnam Airlines", "Bamboo Airways", "Vietravel Airlines"
    };

    for (const auto& flight : masterData) {
        bool isValid = validAirlines.find(flight.airline_name) != validAirlines.end();
        EXPECT_TRUE(isValid) << "Phat hien hang bay la: " << flight.airline_name;
        
        // Kiểm tra tiền tố Flight Number có khớp logic không (Optional)
        if (flight.airline_name == "Vietjet Air") {
            EXPECT_TRUE(flight.flight_number.find("VJ") == 0);
        } else if (flight.airline_name == "Vietnam Airlines") {
            EXPECT_TRUE(flight.flight_number.find("VN") == 0);
        }
    }
}

// --- BÀI TEST 5: KIỂM TRA DỮ LIỆU GIÁ (HISTORY) ---
TEST_F(FlightDataTest, CheckPriceHistoryLogic) {
    // Chỉ lấy 5 chuyến bay đầu để test lịch sử cho nhanh
    std::vector<FlightMaster> subset(masterData.begin(), masterData.begin() + 5);
    
    // Sinh lịch sử tháng 2/2024 (Năm nhuận -> 29 ngày)
    auto history = generator.generatePriceHistory(subset, 2, 2024);

    // Kiểm tra tổng số lượng: 5 máy bay * 29 ngày * 2 hạng ghế = 290 dòng
    EXPECT_EQ(history.size(), 5 * 29 * 2);

    for (const auto& item : history) {
        EXPECT_GT(item.total_fare, 0) << "Gia ve khong duoc bang 0 hoac am";
        EXPECT_GT(item.total_fare, item.base_fare) << "Gia tong phai lon hon gia goc (phai cong thue)";
        
        // Kiểm tra hạng ghế
        bool validClass = (item.seat_class == "Economy" || item.seat_class == "Business");
        EXPECT_TRUE(validClass) << "Hang ghe khong hop le: " << item.seat_class;
    }
}

// Hàm main của Google Test
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}