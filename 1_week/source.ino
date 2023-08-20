#include <ESP8266WebServer.h>
#include <ESP8266Wifi.h>

#define RAW_BUFFER_LENGTH 512
#define HTTP_REST_PORT 80
#define WIFI_RETRY_DELAY 500
#define MAX_WIFI_INIT_RETRY 50
const char* wifi_ssid = "haezoom_HVAC_IR";
const char* wifi_passwd = "p@ssw0rd1@";
// HTTP Rest server 생성
ESP8266WebServer http_rest_server(HTTP_REST_PORT);

// 로컬 네트워크 Wifi AP 연결 및 초기화
int init_wifi() {
  int retries = 0;

  Serial.println("Connecting to WiFi AP..........");

  WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_ssid, wifi_passwd);
  // 와이파이  접속이 성공할 때 까지 재접속 시도
  while ((WiFi.status() != WL_CONNECTED) 
          && (retries < MAX_WIFI_INIT_RETRY)) {
    retries++;
    delay(WIFI_RETRY_DELAY);
    Serial.print("#");
  }
  // 와이파이 연결 상태 리턴
  return WiFi.status(); 
}

void get_home() {
  http_rest_server.send(200, "text/html",
    "Welcome to the ESP8266 REST Web Server");
}

// HTTP Restful API 라우팅 설정
// Restful API 인터페이스에 메서드 할당
void config_rest_server_routing() {
  http_rest_server.on("/", HTTP_GET, get_home);  
}

void setup(void) {
   Serial.begin(115200);

  // 와이파이 연결 시도
  if (init_wifi() == WL_CONNECTED) {
    Serial.print("Connetted to ");
    Serial.print(wifi_ssid);
    Serial.print("--- IP: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.print("Error connecting to: ");
    Serial.println(wifi_ssid);
  }
           
}

void setup(void) {
   Serial.begin(115200);

  // 와이파이 연결 시도
  if (init_wifi() == WL_CONNECTED) {
    Serial.print("Connetted to ");
    Serial.print(wifi_ssid);
    Serial.print("--- IP: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.print("Error connecting to: ");
    Serial.println(wifi_ssid);
  }
  // HTTP Restful 라우팅 인터페이스 등록
  config_rest_server_routing();
  // HTTP Restful 서버 구동
  http_rest_server.begin();
           
}

void loop(void) {
	  // HTTP Restful Server 동작
  http_rest_server.handleClient();          
}