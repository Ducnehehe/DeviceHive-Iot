Cách sử dụng
1. Tải về 
  - Sử dụng câu lệnh để cài đặt:
  - Trong file docker-compose chạy 17 cái trừ cái 18 node để tránh xung đột lẫn nhau
  - docker-compose -f docker-compose.yml --profile all up -d
  - Sau khi chạy hết 17 cái, chạy cái số 18 để hoàn thành sử dụng lệnh
  - docker-compose -f docker-compose.yml --profile all up -d


2. Sau khi chạy thành công:
  - Truy cập URL: http://localhost/admin và tiến hành đăng nhập với name **dhadmin** và password **dhadmin_#911**
  - Sau khi login thành công sẽ vào được trang quản lí của Admin.

3. DeviceHive cung cấp các API swagger để có thể tương tác với hệ thống:
     -  Frontend Swagger:	http://hostname/api/swagger
     -  Auth Swagger:	http://hostname/auth/swagger
     -  Plugin Swagger:	http://hostname/plugin/swagger
     -  Grafana: http://hostname/grfana

4. Tải tablePlus tiến hành kết nối db 
    - Kiên quyết file của bạn phải có .env

5. Nhúng Code ESP8266 và thiết lập các cấu hình cần thiết. Sau đó kết nối Sensor DHT11 và gửi dữ liệu lên DeviceHive.
    - Dùng giao thức mqtt hoặc http để chạy
    - Dùng ESP8266 và DH11 

6. Sử dụng http://localhost/grafana để truy cập Dashboard và sử dụng các câu lệnh truy vấn để xem dữ liệu:
```sql
SELECT s.timestamp AS "time", 
       CONCAT(d.name, ' - ', 'temperature') AS metric, 
       s.temperature AS value
FROM sensor_data AS s
INNER JOIN device AS d ON s.device_name = d.name  -- Join on the correct foreign key relationship

UNION ALL

SELECT s.timestamp AS "time", 
       CONCAT(d.name, ' - ', 'humidity') AS metric, 
       s.humidity AS value
FROM sensor_data AS s
INNER JOIN device AS d ON s.device_name = d.name;  -- Join on the correct foreign key relationship
```
Và đây là những điều cần thiết mà các bạn có thể chạy được file trên nếu có vấn đề gì gặp phải hãy liên hệ SĐT 0344394856 hoặc email ducok123aa@gmail.com. 

Chúc các bạn thành công
#   D e v i c e H i v e - I o t  
 