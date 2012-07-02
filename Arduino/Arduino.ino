#include <EtherCard.h>
#include <Wire.h>
#include "piece.h"
#include "sensor.h"
#include "actuator.h"
#include "led_actuator.h"
#include "pc_actuator.h"
#include "bmp085_sensor.h"
#include "defines.h"
#include "home.h"

BMP085_Sensor pression_in("Interieur");
Led_Actuator led13("13", 13);
byte mac[] = {0x00,0x24,0x1D,0xE9,0x27,0xAA};
byte ip[] = {192,168,0,12};
PC_Actuator pc("PC", mac, ip);
Piece rdc("RDC");
Home maison("Toussaint");

bool prepared = false;


// ethernet interface mac address - must be unique on your network
static byte mymac[] = { 0x74,0x42,0x13,0x37,0x30,0x31 };

static BufferFiller bfill;  // used as cursor while filling the buffer

byte Ethernet::buffer[1000];   // tcp/ip send and receive buffer

/*bool         etaled[54]    = {0};

byte         etabarled[54] = {0},
             pc_mac[][6]   = {{0x00,0x24,0x1D,0xE9,0x27,0xAA},
                              {0x00,0x1D,0x72,0xBF,0x59,0x6B}};

const byte   led_pins[]    = {13},
             barled_pins[] = {2 , 22},
             barled_size[] = {10, 20};
           
char pc_nom[][40] PROGMEM  = {"Ordinateur Rez-De-Chausse",
                              "Ordinateur Bureau"};*/
                             
           
char okHeader[] PROGMEM = 
    "HTTP/1.0 200 OK\r\n"
    "Access-Control-Allow-Origin: *\r\n"
;

char okHeaderJSON[] PROGMEM = 
    "HTTP/1.0 200 OK\r\n"
    "Content-Type: application/json\r\n"
    "Access-Control-Allow-Origin: *\r\n"
;

unsigned long timer;
           
void setup(){
    Wire.begin();
#if SERIAL
    Serial.begin(57600);
    Serial.println("[Dohome]");
#endif
    
    if (ether.begin(sizeof Ethernet::buffer, mymac) == 0){
#if SERIAL
      Serial.println("Failed to access Ethernet controller");
#endif
    }
    if (!ether.dhcpSetup()){
#if SERIAL
      Serial.println("DHCP failed");
#endif
    }
#if SERIAL
    ether.printIp("IP: ", ether.myip);
#endif

/*  for (int i = 0 ; i < sizeof(led_pins) ; i++){
    pinMode(led_pins[i], OUTPUT);
  }
  
  for (int i = 0 ; i < sizeof(barled_pins) ; i++){
    for (int j = 0 ; j < barled_size[i] ; j++){
      pinMode(barled_pins[i]+j, OUTPUT);
    }
  }*/
  
  rdc.addSensor(pression_in);
  rdc.addActuator(pc);
  rdc.addActuator(led13);
  
  maison.addPiece(rdc);
  timer = millis();
}

static int getIntArg(const char* data, const char* key, int value =-1) {
    char temp[10];
    if (ether.findKeyVal(data + 7, temp, sizeof temp, key) > 0)
        value = atoi(temp);
    return value;
}


static char* getStrArg(const char* data, const char* key) {
    char temp[120];
    if (ether.findKeyVal(data + 7, temp, sizeof temp, key) > 0)
      return temp;
    else
      return NULL;
}


static void homePage(BufferFiller& buf) {
    buf.emit_p(PSTR("$F\r\n"
        "{"
        /*"\"l\":["*/), okHeaderJSON);
        
/*    for (int i = 0 ; i < sizeof(led_pins) ; i++){
      buf.emit_p(PSTR("{\"n\":\"$D\",\"e\":\"$D\"}"), led_pins[i], etaled[led_pins[i]]);
      
      if((sizeof(led_pins) -i) > 1){
        buf.emit_p(PSTR(","));
      }
    }
    
    buf.emit_p(PSTR("],\"b\":["));
    
    for (int i = 0 ; i < sizeof(barled_pins) ; i++){
      buf.emit_p(PSTR("{\"n\":\"$D\",\"v\":\"$D\",\"m\":\"$D\"}"), barled_pins[i], etabarled[barled_pins[i]], barled_size[i]);
      
      if((sizeof(barled_pins) - i) > 1){
        buf.emit_p(PSTR(","));
      }
    }
    
    buf.emit_p(PSTR("],\"p\":["));
    
    for (int i = 0 ; i < (sizeof(pc_nom)/40) ; i++){
      buf.emit_p(PSTR("{\"n\":\"$D\",\"l\":\"$F\"}"), i, pc_nom[i]);
      if(((sizeof(pc_nom)/40) - i)> 1){
        buf.emit_p(PSTR(","));
      }
    }
    
    buf.emit_p(PSTR("],\"c\":["));*/
    
    maison.fillJSONData(buf);
    
    unsigned long t = millis() / (unsigned long)1000;
    buf.emit_p(PSTR(
        ",\"up\":$L}"), t);}

static void gouvact(const char* data, BufferFiller& buf) {
  if (data[6] == '?'){
    byte id_p = getIntArg(data, "p", 255);
    byte id_a = getIntArg(data, "a", 255);
    char *arg = getStrArg(data, "c");
    
    if (maison.prepare(arg, id_p, id_a)){
      prepared = true;
      buf.emit_p(PSTR("$F\r\n1"), okHeader);
    } else {
      buf.emit_p(PSTR("$F\r\n0"), okHeader);
    }
  } else {
    buf.emit_p(PSTR("$F\r\n0"), okHeader);
  }
}

/*static void gouvbarled(const char* data, BufferFiller& buf) {
  byte *indice;
  if (data[6] == '?'){
    byte no = getIntArg(data, "n");
    byte val = getIntArg(data, "v", 0);
    
    if ((indice = (byte*)memchr(barled_pins, no, sizeof(barled_pins))) != NULL){
      if (val <= barled_size[barled_pins-indice] && val >= 0){
        etabarled[no] = val;
        for (int i = 0 ; i < val ; i++){
          digitalWrite(no + i, HIGH);
        }
        for (int i = val ; i < barled_size[barled_pins-indice]; i++){
          digitalWrite(no + i, LOW);
        }
        buf.emit_p(PSTR("$F\r\n1"), okHeader, val);
      } else {
        buf.emit_p(PSTR("$F\r\n0"), okHeader);
      }
    } else {
      buf.emit_p(PSTR("$F\r\n0"), okHeader);
    }
  } else {
    buf.emit_p(PSTR("$F\r\n0"), okHeader);
  }
}

static void gouvpc(const char* data, BufferFiller& buf) {
  byte *indice;
  if (data[6] == '?'){
    byte id = getIntArg(data, "n");
    
    if (id < sizeof(pc_mac)){
      buf.emit_p(PSTR("$F\r\n1"), okHeader);
      wol_id = id;
    } else {
      buf.emit_p(PSTR("$F\r\n0"), okHeader);
    }
  } else {
    buf.emit_p(PSTR("$F\r\n0"), okHeader);
  }
}*/

void loop(){
    word len = ether.packetReceive();
    word pos = ether.packetLoop(len);
    // check if valid tcp data is received
    if (pos) {
        bfill = ether.tcpOffset();
        char* data = (char *) Ethernet::buffer + pos;
#if SERIAL
        Serial.println(data);
#endif
#if !(DEBUG)
         if ((data[6] == '?') && (getStrArg(data, "m") != NULL)){
          if (strcmp(getStrArg(data, "m"), PASSWORD) == 0){
#endif
            if (strncmp("GET /h", data, 6) == 0)
                homePage(bfill);
            else if (strncmp("GET /a", data, 6) == 0)
                gouvact(data, bfill);
/*            else if (strncmp("GET /b", data, 6) == 0)
                gouvbarled(data, bfill);
            else if (strncmp("GET /o", data, 6) == 0)
                gouvpc(data, bfill);*/
            else
                bfill.emit_p(PSTR(
                    "HTTP/1.0 404 Not Found\r\n"
                    "Access-Control-Allow-Origin: *\r\n"
                    "\r\n"));
#if !(DEBUG)
          } else {
            bfill.emit_p(PSTR(
                "HTTP/1.0 403 Forbidden\r\n"
                "Access-Control-Allow-Origin: *\r\n"
                "\r\n"));
          }
        } else {
          bfill.emit_p(PSTR(
              "HTTP/1.0 401 Authorization Required\r\n"
              "Access-Control-Allow-Origin: *\r\n"
              "\r\n"));
        }
#endif
        ether.httpServerReply(bfill.position()); // send web page data
        
        if (prepared){
          maison.exec();
          prepared = false;
        }
        
    } else if ((millis() - timer) >= 1000) {
#if SERIAL
      Serial.println("Refresh");
#endif
        timer = millis();
        maison.refresh();
#if SERIAL
      Serial.println("fin refresh");
#endif
    }
}

