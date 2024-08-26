#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// Thay thế các thông số này bằng thông tin WiFi và máy chủ MQTT của bạn
const char* ssid = "Khac...";           // Thay thế bằng SSID WiFi của bạn
const char* password = "hoithangBE";   // Thay thế bằng mật khẩu WiFi của bạn
const char* mqtt_server = "192.168.1.174";    // Thay thế bằng địa chỉ IP của máy chủ MQTT

// Cảm biến DHT11
#define DHTPIN D4    // Chân kết nối với chân data của DHT11
#define DHTTYPE DHT11   // Loại cảm biến sử dụng

// Tạo một thể hiện của lớp DHT
DHT dht(DHTPIN, DHTTYPE);

// Tạo một thể hiện của WiFiClient và PubSubClient
WiFiClient espClient;
PubSubClient client(espClient);

// Kết nối đến WiFi
void setup_wifi() {  
  
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Đang kết nối tới WiFi ");
  Serial.println(ssid);
  Serial.println("Kết nối thành công");
  Serial.print("Địa chỉ IP: ");
  Serial.println(WiFi.localIP());
}

// Hàm callback khi nhận được tin nhắn (không sử dụng trong ví dụ này)
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Tin nhắn đến [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

// Kết nối lại với máy chủ MQTT nếu bị mất kết nối
void reconnect() {
  while (!client.connected()) {
    Serial.print("Đang cố kết nối với MQTT... ");
    if (client.connect("ESP8266Client")) {
      Serial.println("đã kết nối");
      // Đăng ký các chủ đề nếu cần
    } else {
      Serial.print("thất bại, rc=");
      Serial.print(client.state());
      Serial.println(" thử lại sau 5 giây");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  // Khởi tạo cảm biến DHT
  dht.begin();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Đọc nhiệt độ và độ ẩm từ cảm biến DHT11
  float temperature = dht.readTemperature(); // Đọc nhiệt độ tính bằng Celsius
  float humidity = dht.readHumidity();       // Đọc độ ẩm tính bằng phần trăm

  // Kiểm tra nếu đọc dữ liệu bị lỗi và thoát sớm (để thử lại)
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Không đọc được từ cảm biến DHT!");
    return;
  }

  // Chuẩn bị dữ liệu gửi đi
  String device_id = "e50d6085-2aba-48e9-b1c3-73c673e414be"; // Thiết bị mặc định: Thiết bị mẫu
  String payload = "{\"temperature\": " + String(temperature, 1) + ", \"humidity\": " + String(humidity, 1) + ", \"device_id\": \"" + device_id + "\"}";

  // Đăng dữ liệu lên chủ đề
  client.publish("sensor/data", payload.c_str());

  // Chờ 10 giây trước khi gửi dữ liệu tiếp theo
  delay(10000);
}
