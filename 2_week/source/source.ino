#include <ESP8266WiFi.h>
// ESP8266 Webserver 모듈
#include <ESP8266WebServer.h>

#define RAW_BUFFER_LENGTH 512
#define HTTP_REST_PORT 80
#define WIFI_RETRY_DELAY 500
#define MAX_WIFI_INIT_RETRY 50

#include "header.h"

// IR 원격 제어를 위한 모듈
#include <IRremote.hpp>

const char* wifi_ssid = "haezoom5F-3";
const char* wifi_passwd = "idnstory132";
// HTTP Rest server 생성
ESP8266WebServer http_rest_server(HTTP_REST_PORT);

// FCU 전원 off IR Pulse 신호값 
const uint16_t aircon_hj_off[] = {8938, 4368, 618, 524, 618, 1667, 619, 1669, 617, 1667, 619, 523, 619, 1667, 619, 1667, 619, 1671, 617, 1666, 618, 1668, 619, 524, 619, 1666, 619, 525, 618, 523, 618, 523, 619, 523, 619, 523, 619, 523, 619, 523, 619, 523, 619, 523, 619, 524, 618, 523, 619, 523, 619, 523, 619, 1667, 619, 523, 618, 523, 619, 523, 648, 494, 648, 495, 648, 493, 647, 1637, 648, 1637, 648, 494, 647, 1639, 647, 495, 646, 495, 647, 495, 646, 496, 645, 496, 644, 497, 643, 499, 591, 551, 591, 550, 591, 551, 591, 552, 590, 551, 591, 551, 590, 550, 591, 578, 564, 552, 590, 577, 570, 573, 565, 577, 563, 551, 590, 578, 564, 578, 564, 578, 564, 552, 590, 552, 595, 573, 564, 578, 564, 577, 564, 587, 555, 578, 589, 552, 590, 552, 590, 552, 590, 552, 593, 548, 590, 559, 583, 551, 590, 1695, 590, 1695, 590, 552, 590, 561, 582, 559, 585, 548, 590, 553, 589, 9877, 8885, 4392, 590, 550, 591, 1695, 590, 1694, 590, 1693, 591, 550, 591, 1694, 591, 1693, 591, 1694, 591, 1694, 591, 1694, 591, 550, 591, 1697, 588, 551, 591, 551, 591, 550, 591, 551, 591, 550, 591, 551, 591, 550, 591, 556, 586, 550, 592, 550, 590, 550, 591, 550, 591, 550, 591, 1694, 591, 550, 596, 546, 591, 551, 591, 550, 591, 550, 591, 551, 591, 1693, 591, 1693, 591, 550, 591, 1694, 591, 550, 591, 550, 591, 550, 591, 550, 591, 550, 591, 552, 590, 551, 590, 550, 592, 550, 590, 550, 591, 550, 591, 550, 591, 550, 591, 551, 591, 555, 587, 550, 591, 550, 591, 550, 591, 550, 591, 550, 591, 550, 591, 550, 591, 550, 591, 550, 591, 550, 591, 550, 591, 550, 591, 550, 591, 550, 591, 550, 591, 550, 591, 550, 592, 550, 592, 550, 603, 538, 592, 550, 590, 550, 591, 1693, 591, 1698, 586, 550, 591, 550, 591, 550, 591, 551, 591, 550, 592};  

// FCU 냉방 전원 on IR Pulse 신호값 
const uint16_t aircon_hj_on[] = {8926, 4409, 617, 526, 619, 1672, 619, 1672, 619, 1674, 618, 525, 618, 1672, 619, 1671, 619, 1672, 618, 1675, 615, 1672, 618, 526, 618, 1671, 618, 527, 618, 526, 618, 530, 614, 526, 618, 526, 618, 526, 618, 526, 618, 526, 618, 526, 618, 526, 618, 526, 618, 527, 619, 527, 617, 526, 618, 526, 618, 526, 618, 527, 618, 526, 618, 526, 618, 1675, 615, 527, 618, 1672, 618, 526, 618, 526, 618, 1672, 618, 526, 618, 528, 616, 527, 618, 526, 618, 526, 618, 527, 618, 526, 618, 527, 618, 526, 618, 526, 618, 527, 617, 526, 618, 526, 618, 526, 618, 526, 618, 526, 618, 526, 618, 526, 618, 527, 620, 524, 618, 526, 618, 526, 617, 526, 617, 527, 617, 526, 618, 526, 617, 526, 618, 528, 616, 526, 618, 526, 618, 526, 617, 527, 617, 526, 617, 526, 618, 526, 618, 1677, 614, 1671, 617, 526, 618, 1672, 617, 526, 618, 526, 618, 527, 625, 1664, 618, 9867, 8928, 4376, 616, 527, 616, 1672, 617, 1672, 617, 1673, 616, 528, 617, 1673, 616, 1672, 619, 1670, 617, 1673, 616, 1673, 617, 527, 616, 1672, 617, 528, 616, 528, 616, 527, 617, 526, 617, 528, 616, 527, 617, 527, 617, 527, 617, 530, 614, 527, 616, 527, 617, 528, 616, 527, 617, 527, 616, 528, 616, 527, 617, 528, 616, 528, 616, 528, 617, 1673, 616, 528, 616, 1673, 616, 528, 616, 528, 615, 1673, 615, 529, 615, 528, 616, 527, 616, 528, 616, 528, 616, 528, 616, 528, 618, 526, 615, 529, 615, 528, 616, 528, 616, 528, 616, 528, 616, 528, 616, 528, 616, 529, 615, 528, 615, 528, 616, 528, 615, 528, 616, 528, 616, 528, 616, 528, 616, 528, 616, 529, 615, 528, 615, 528, 616, 552, 591, 528, 615, 528, 615, 528, 616, 528, 615, 529, 617, 526, 615, 551, 592, 1696, 591, 1673, 615, 552, 591, 1675, 613, 528, 615, 530, 614, 529, 615, 1673, 615};  


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

void post_send_off() {    
    IrSender.sendRaw(aircon_hj_off, sizeof(aircon_hj_off) / sizeof(aircon_hj_off[0]), NEC_KHZ); 

    http_rest_server.send(200, "text/plain", "success");
}

void post_send_on() {    
    IrSender.sendRaw(aircon_hj_on, sizeof(aircon_hj_on) / sizeof(aircon_hj_on[0]), NEC_KHZ); 

    http_rest_server.send(200, "text/plain", "success");
}

// HTTP Restful API 라우팅 설정
// Restful API 인터페이스에 메서드 할당
void config_rest_server_routing() {
  http_rest_server.on("/", HTTP_GET, get_home);  
  http_rest_server.on("/on", HTTP_POST, post_send_on);  
  http_rest_server.on("/off", HTTP_POST, post_send_off);  
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
           
  // IR 트랜스미터 초기화
  IrSender.begin();
}

void loop(void) {
	  // HTTP Restful Server 동작
  http_rest_server.handleClient();          
}