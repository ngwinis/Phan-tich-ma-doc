# Báo cáo phân tích mã độc Trojan/Stealer

## 0. Tổng quan

Gần đây mình phát hiện 1 loại mã độc trojan giả danh phần mềm cài đặt công cụ Adobe Photoshop. Đặc điểm chung của các dạng trojan này thường sẽ yêu cầu người dùng tắt Windows Defender rồi chạy file dưới quyền Administrator. Mẫu mã độc này cũng thế, nó sẽ yêu cầu chạy file `Setup.exe` với quyền Admin. Sau khi chạy sẽ không hiển thị giao diện cài đặt mà nó sẽ âm thầm tải 1 số file về và đồng thời khi đã có quyền Admin, nó dễ dàng sửa các **registry key** và làm hỏng 1 số chức năng của hệ thống, cụ thể trong trường hợp này là tính năng **Update Windows**. Đồng thời với đó, nó thêm 1 registry key để cho phép **chạy các file `.exe`** và lấy đường dẫn tới các file được tải về trước đó để **thiết lập cơ chế persistent**. Mục đích cuối cùng của trojan này chỉ là **đánh cắp mật khẩu tài khoản mạng xã hội lưu trong trình duyệt người dùng**. Chi tiết quá trình tấn công mình sẽ trình bày đầy đủ hơn ở dưới phần **Phân tích động**.

Các file thực thi liên quan và mã hash sha256 tương ứng:
|file|Hash|
|--|--|
|Setup.exe|b27f17dec9e4cfa81853201b1118a66881e4beb372fe65cc432fbed616ece077|
|falcon.exe|f9f366e8e91758d75eea9ee894b3ca936d7614fd7b6e07342bb5a19deb9c152d|
|summer.exe|94465293b5c291da3fb2cf0eb3c6d995a4735921d876736cf9abae624dc1f4be|

Dưới đây là quy trình mình đưa ra để phân tích mã độc này:

## 1. Chuẩn bị môi trường

- Môi trường cách li được sử dụng ở đây gồm:
  - Sandbox [any.run](any.run)
  - Máy ảo VMWare
- Công cụ hỗ trợ: Wireshark, Procmon, Regshot, Process Explorer, Ghidra/IDA Pro

## 2. Phân tích tĩnh

### 2.1. Xác định file:

- Công cụ sử dụng: DIE, sha256sum
- File `Setup.exe`:

  ![alt text](images/trojan001.png)

- File `falcon.exe`:

  ![alt text](images/trojan002.png)

- File `summer.exe`:

  ![alt text](images/trojan003.png)

- Kết luận: Cả 3 file này đều là file thực thi trên Windows với định dạng file PE64, trong đó:
  - File `Setup.exe` là file thực thi khá rõ ràng
  - File `falcon.exe` và `summer.exe` là file thực thi đã bị pack bởi trình **packer Themida/Winlicense(3.XX)**
- Mã hash như đã đề cập ở mục **0. Tổng quan**

### 2.2. Các Strings:

- Xem trong bảng Strings của IDA thì thấy có 1 vài string rất đáng ngờ, cụ thể, trong bảng string này:
    - Có các đoạn mã **Nodejs** rất dài, có lẽ đây là toàn bộ 1 chương trình nodejs được nhúng vào trong file `Setup.exe`:

        ![alt text](images/trojan004.png)
    
    - Có 1 vài đoạn chèn kí tự `\t` (Tab) khá dài, (có lẽ là để giấu đi đoạn mã độc chính) cùng với 2 thông báo **"Node.js QUIC Server"** và **"There are pending queries"** có thể đoán đây là 1 log thông báo kết nối tới C2 server

        ![alt text](images/trojan005.png)
    
    - Ngoài ra còn có các string đáng ngờ khác như sau:

        ![alt text](images/trojan006.png)

        ![alt text](images/trojan007.png)

        ![alt text](images/trojan008.png)

        ![alt text](images/trojan009.png)

        ![alt text](images/trojan010.png)

        ![alt text](images/trojan011.png)

### 2.3. Các hàm/thư viện được import
- Thư viện **ADVAPI32**: cung cấp các hàm tương tác với các dịch vụ của hệ thống và Windows Registry:

    ![alt text](images/trojan015.png)

- Thư viện **IPHLPAPI**: cung cấp các hàm hỗ trợ việc kết nối Internet:

    ![alt text](images/trojan016.png)

- Một số hàm trong **kernel32.dll** liên quan đến việc tạo tạo và ghi file:

    ![alt text](images/trojan017.png)

- Một số hàm trong **kernel32.dll** liên quan đến việc tạo tiến trình mới và thực thi một số đoạn mã:

    ![alt text](images/trojan018.png)

- Một số hàm trong **kernel32.dll** liên quan đến việc xoá file:

    ![alt text](images/trojan019.png)

- Một số hàm trong **kernel32.dll** dùng để tìm kiếm các file có trong thư mục được chỉ định:

    ![alt text](images/trojan020.png)

### 2.4. Các hành vi


## 3. Phân tích động

