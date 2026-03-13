#ifndef _UTILS_H_
#define _UTILS_H_

#include <random>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>

class Utils {
private:
    std::mt19937 rng;

public:
    Utils() {
        std::random_device rd;
        rng.seed(rd());
    }

    int getInt(int min, int max) {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(rng);
    }

    double getDouble(double min, double max) {
        std::uniform_real_distribution<double> dist(min, max);
        return dist(rng);
    }

    // Hàm chọn phần tử ngẫu nhiên trong vector
    template <typename T>
    T getRandomElement(const std::vector<T>& list) {
        return list[getInt(0, list.size() - 1)];
    }

    // Hàm lấy số ngày trong tháng chính xác
    // Hàm bổ trợ: Kiểm tra năm nhuận
    bool isLeapYear(int n) {
        return ((n % 400 == 0) ||
                ((n % 4 == 0) && (n % 100 != 0)));
    }

    int getDaysInMonth(int month, int year) {
        switch (month) {
            case 4: case 6: case 9: case 11:
                return 30;
            case 2: 
                if (isLeapYear(year)) {
                    return 29;
                } else {
                    return 28;
                }
            default:
                return 31;
        }
    }

    // Hàm cộng ngày vào một ngày gốc
    // Input: 2025-11-01, cộng thêm 5 ngày -> Output: 2025-11-06
    std::string addDays(int startYear, int startMonth, int startDay, int daysToAdd) {
        std::tm date = {};
        date.tm_year = startYear - 1900;
        date.tm_mon = startMonth - 1;
        date.tm_mday = startDay;
        
        // Cộng giây (days * 24h * 60m * 60s)
        std::time_t timeParams = std::mktime(&date);
        timeParams += daysToAdd * 24 * 60 * 60;
        
        std::tm* newDate = std::localtime(&timeParams);
        
        char buffer[20];
        std::strftime(buffer, 20, "%Y-%m-%d", newDate);
        return std::string(buffer);
    }
};

#endif