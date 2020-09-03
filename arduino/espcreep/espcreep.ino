/*
==========================================
============= UDP SERVO CAM ==============
==========================================
TODO:
  ~STOP FEATURE - stop drive motors if no UDP for x seconds
  ~3rd SERVO
==========================================
STREAMING CODE FROM - https://github.com/RuiSantosdotme/ESP32-CAM-Arduino-IDE
EXAMPLE COMMANDS:
  echo -n "5,4700,4900,0,0" >/dev/udp/192.168.1.72/1234       SET MOTORS WITH RAW PWM
  echo -n "8,128,0,0,0" >/dev/udp/192.168.1.72/1234           SET FLASH BRIGHTNESS 0-999999
  http://192.168.1.72:80                                      CAMERA STREAM
========================================*/
#include "esp_camera.h"
#include <WiFi.h>
#include <WiFiUdp.h>
#include "img_converters.h"
#include "Arduino.h"
#include "fb_gfx.h"
#include "soc/soc.h" //disable brownout problems
#include "soc/rtc_cntl_reg.h"  //disable brownout problems
#include "esp_http_server.h"
#include "AsyncUDP.h"

bool debugoutput = true;    //---DEBUG (SERIAL) OUTPUT ON\OFF
//bool debugoutput = false;

//===UDP VARS
unsigned int localUDPPort = 1234;   //---LOCAL UDP PORT TO LISTEN TO
char udpbuffer[255];        //---INCOMMING UDP PACKED BUFFER
int udpcmd[5];              //---UDP COMMAND ARRAY
String udpstring;

//===CAMERA VARS
#define PART_BOUNDARY "123456789000000000000987654321" //---FOR CAMERA
static const char* _STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char* _STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char* _STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";
httpd_handle_t stream_httpd = NULL;

//===UDP OBJECT
AsyncUDP udp;

//==============================================
//================FUNCTION TESTING==============
//==============================================

//==============================================
//=================MAIN FUNCTIONS===============
//==============================================
void setup() {
  //======ESP32 STUFF======
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
  
  //======SETUP SERIAL======
  if (debugoutput){
    Serial.begin(115200); delay(1000);
    dbo("|----------GOOD NEWS, SUCKERS!------------|\n");
  }
  
  //======SETUP GPIO======
  setupGPIO();
  
  //======SETUP WIFI========
  WiFiConnectToAP();
  
  //======SETUP UDP=======
  if(udp.listen(localUDPPort)) {
    Serial.print("UDP Listening on IP: ");
    Serial.println(WiFi.localIP());
    udp.onPacket([](AsyncUDPPacket packet) {
        udpstring = String( (char*) packet.data());
        parseUDP(udpstring);
        execUDP();        
        packet.printf("Got %u bytes of data", packet.length());
    });
  }

  //======SETUP CAMERA======
  if (WiFi.status() == WL_CONNECTED){
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = 5;
    config.pin_d1 = 18;
    config.pin_d2 = 19;
    config.pin_d3 = 21;
    config.pin_d4 = 36;
    config.pin_d5 = 39;
    config.pin_d6 = 34;
    config.pin_d7 = 35;
    config.pin_xclk = 0;
    config.pin_pclk = 22;
    config.pin_vsync = 25;
    config.pin_href = 23;
    config.pin_sscb_sda = 26;
    config.pin_sscb_scl = 27;
    config.pin_pwdn = 32;
    config.pin_reset = -1;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;
    if(psramFound()){
      //FRAMESIZE_SVGA[Y], FRAMESIZE_VGA[Y], FRAMESIZE_CIF[N], FRAMESIZE_QVGA[N], FRAMESIZE_HQVGA[N], FRAMESIZE_QQVGA[N]
      config.frame_size = FRAMESIZE_VGA;
      config.jpeg_quality = 10;
      config.fb_count = 2;
   } else {
      //config.frame_size = FRAMESIZE_SVGA;
      config.frame_size = FRAMESIZE_VGA;
      config.jpeg_quality = 20;
      config.fb_count = 1;
    }
  
    //---INITIALIZE CAMERA
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
      dbo("Camera init failed\n");
      return;
    }

    //---START CAMERA WEB SERVER WEB
    startCameraServer();
  }
}//---END SETUP LOOP

//========================================
//=================LOOP===================
//========================================
void loop(){

}
