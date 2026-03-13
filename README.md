# Flight Data Generator & Simulator

Chương trình mô phỏng và tạo dữ liệu hàng không giả lập (Big Data) phục vụ cho việc phân tích dữ liệu, kiểm thử hệ thống đặt vé máy bay.  

*(Đây là module cốt lõi do mình phát triển trong dự án nhóm Smart Flight Price Optimizer, tập trung vào việc xử lý logic nghiệp vụ và kiểm thử tự động).*

## 📌 Tính năng nổi bật
1. **Tạo Master Data (`flights.json`):** Sinh danh sách các tuyến bay cố định với thông tin hãng bay, sân bay đi/đến, thời gian bay dự kiến.
2. **Tạo Transaction Data (`price_history.json`):** Sinh lịch sử biến động giá vé theo ngày, giờ, hạng ghế (Economy/Business) cho từng chuyến bay.
3. **Logic nghiệp vụ thực tế:**  
    - Tính toán thời gian bay thực tế theo khoảng cách địa lý (VD: HN-HCM bay lâu hơn HN-Đà Nẵng).
    - Tự động ánh xạ Mã sân bay (HAN) sang Tên thành phố (Hà Nội).
    - Xử lý năm nhuận, số ngày trong tháng chuẩn xác.
4. **Kiểm thử tự động & bắt lỗi (GTest):** Không chỉ tạo dữ liệu chuẩn, hệ thống còn bao gồm các kịch bản sinh "dữ liệu xấu" (Bad Data) để kiểm tra sức chịu đựng của hệ thống trước các đầu vào không hợp lệ.


## Cấu trúc
- **include/**: Chứa các file header (.hpp), định nghĩa cấu trúc dữ liệu (`Flight.hpp`) và các công cụ hỗ trợ (`Utils.hpp`).
- **src/**: Chứa mã nguồn logic chính (`DataGenerator.cpp`) và chương trình chạy (`main.cpp`).
- **test/**: Chứa các bài kiểm thử tự động (`DataGeneratorTest.cpp`) sử dụng Google Test.


## Hướng dẫn Cài đặt & Chạy

### 1. Yêu cầu hệ thống
- `Google Test` (Chỉ cần thiết nếu muốn chạy file test).

### 2. Cách chạy chương trình tạo dữ liệu (Data Generator)

Mở Terminal tại thư mục `Smart-Flight-Price-Optimizer`, chạy lệnh sau:

```bash
# Biên dịch
g++ tools/DataGenerator/src/main.cpp tools/DataGenerator/src/DataGenerator.cpp -o generator -I include -std=c++23

# Chạy chương trình
./generator
```

### 3. Cách chạy chương trình kiểm thử (Data Generator Test)

Mở Terminal và chạy lệnh sau:  

```bash
# Biên dịch
g++ DataGenerator/test/DataGeneratorTest.cpp tools/DataGenerator/src/DataGenerator.cpp -o run_tests -I include -std=c++23 -lgtest -lgtest_main -pthread

# Chạy chương trình
./run_tests
```

### 4. Cách chạy chương trình tạo dữ liệu xấu (Bad Data Generator)
Mở Terminal và chạy lệnh sau:  

```bash
# Biên dịch
g++ DataGenerator/test/BadDataGenerator.cpp -o bad_data

# Chạy chương trình
./bad_data
```
