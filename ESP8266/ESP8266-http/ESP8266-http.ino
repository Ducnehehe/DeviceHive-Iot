#include <ESP8266WiFi.h>
#include <DHT.h>
#include <ESP8266WebServer.h>

// Cấu hình cho cảm biến DHT11
#define DHTPIN D4          // Chân kết nối DHT11
#define DHTTYPE DHT11      // Loại cảm biến DHT
DHT dht(DHTPIN, DHTTYPE);

// Cấu hình thông tin Wi-Fi
const char* ssid = "Khac...";
const char* password = "hoithangBE";

// Khởi tạo server web trên cổng 80
ESP8266WebServer server(80);

const char* html = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>DHTT11</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      text-align: center;
      background-color: #f0f0f0;
      margin: 0;
      padding: 0;
    }

    header {
      background-color: #4CAF50;
      color: white;
      padding: 15px 0;
    }

    h1 {
      margin: 0;
      font-size: 2em;
    }

    #data {
      background-color: white;
      border-radius: 8px;
      box-shadow: 0 4px 8px rgba(0,0,0,0.2);
      display: inline-block;
      padding: 20px;
      margin-top: 20px;
      font-size: 1.2em;
    }

    #data p {
      margin: 10px 0;
    }

    #data span {
      font-weight: bold;
    }

    footer {
      margin-top: 20px;
      font-size: 0.8em;
      color: #666;
    }
  </style>
</head>
<body>
  <header>
    <h1>Thông số nhiệt độ và độ ẩm</h1>
  </header>

  <div id="data">
    <p>Nhiệt độ: <span id="temperature">Loading...</span>°C</p>
    <p>Độ ẩm: <span id="humidity">Loading...</span>%</p>
  </div>

  <footer>
    <p>&copy; 2024 DHT11 theo dõi nhiệt độ và độ ẩm.</p>
  </footer>

  <script>
    async function fetchData() {
      try {
        const response = await fetch('/data');
        if (!response.ok) throw new Error('Failed to fetch data');
        const data = await response.json();
        document.getElementById('temperature').innerText = data.temperature || 'No data';
        document.getElementById('humidity').innerText = data.humidity || 'No data';
      } catch (error) {
        console.error('Error fetching data:', error);
        document.getElementById('temperature').innerText = 'Error';
        document.getElementById('humidity').innerText = 'Error';
      }
    }
    setInterval(fetchData, 2000);
    fetchData();
  </script>
</body>
</html>
)rawliteral";

// Các hàm xử lý yêu cầu HTTP
void handleRoot() {
  server.send(200, "text/html", html);
}

void handleData() {
  // Đọc giá trị nhiệt độ và độ ẩm từ DHT11
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Tạo JSON để gửi cho client
  String json = "{";
  json += "\"temperature\":" + String(temperature) + ",";
  json += "\"humidity\":" + String(humidity);
  json += "}";

  server.send(200, "application/json", json);
}

void setup() {
  // Khởi tạo nối tiếp
  Serial.begin(115200);

  // Khởi động kết nối Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Đang kết nối WiFi...");
  }
  Serial.println("Kết nối thành công");

  // In địa chỉ IP ra Serial Monitor
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Khởi tạo cảm biến DHT
  dht.begin();
  delay(5000);

  // Cấu hình các route
  server.on("/", HTTP_GET, handleRoot);
  server.on("/data", HTTP_GET, handleData);

  // Bắt đầu server
  server.begin();
}

void loop() {
  // Xử lý các yêu cầu đến server
  server.handleClient();
}

