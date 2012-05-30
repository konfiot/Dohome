#include <EtherCard.h>

#define DEBUG   0   // set to 1 to access the web interface without password
#define SERIAL  0   // set to 1 to show incoming requests and debug messages on serial port

int wol_id = -1;

// ethernet interface mac address - must be unique on your network
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };

static BufferFiller bfill;  // used as cursor while filling the buffer

byte Ethernet::buffer[1000];   // tcp/ip send and receive buffer

bool         etaled[54]    = {0};

byte         etabarled[54] = {0},
             pc_mac[][6]   = {{0x00,0x24,0x1D,0xE9,0x27,0xAA},
                              {0x00,0x1D,0x72,0xBF,0x59,0x6B}};

const byte   led_pins[]    = {13},
             barled_pins[] = {2 , 22},
             barled_size[] = {10, 20};
           
char pc_nom[][40] PROGMEM  = {"Ordinateur Rez-De-Chausse",
                             "Ordinateur Bureau"};
           
char okHeader[] PROGMEM = 
    "HTTP/1.0 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Pragma: no-cache\r\n"
    "Access-Control-Allow-Origin: *\r\n"
;

char okHeaderJSON[] PROGMEM = 
    "HTTP/1.0 200 OK\r\n"
    "Content-Type: application/json\r\n"
    "Pragma: no-cache\r\n"
    "Access-Control-Allow-Origin: *\r\n"
;

           
void setup(){
#if SERIAL
    Serial.begin(57600);
    Serial.println("\n[etherNode]");
#endif
    
    if (ether.begin(sizeof Ethernet::buffer, mymac) == 0){
#if SERIAL
      Serial.println( "Failed to access Ethernet controller");
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

  for (int i = 0 ; i < sizeof(led_pins) ; i++){
    pinMode(led_pins[i], OUTPUT);
  }
  
  for (int i = 0 ; i < sizeof(barled_pins) ; i++){
    for (int j = 0 ; j < barled_size[i] ; j++){
      pinMode(barled_pins[i]+j, OUTPUT);
    }
  }
}

static int getIntArg(const char* data, const char* key, int value =-1) {
    char temp[10];
    if (ether.findKeyVal(data + 7, temp, sizeof temp, key) > 0)
        value = atoi(temp);
    return value;
}


static char* getStrArg(const char* data, const char* key, int value =-1) {
    char temp[120];
    if (ether.findKeyVal(data + 7, temp, sizeof temp, key) > 0)
      return temp;
    else
      return NULL;
}


static void homePage(BufferFiller& buf) {
    buf.emit_p(PSTR("$F\r\n"
        "{"
        "\"l\":["), okHeaderJSON);
        
    for (int i = 0 ; i < sizeof(led_pins) ; i++){
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
    
    buf.emit_p(PSTR("],"));
    
    unsigned long t = millis() / (unsigned long)1000;
    /*word h = t / 3600;
    byte m = (t / 60) % 60;
    byte s = t % 60;*/
    buf.emit_p(PSTR(
        "\"up\":\"$D\"}"), t);
}

static void gouvled(const char* data, BufferFiller& buf) {
  if (data[6] == '?'){
    byte no = getIntArg(data, "n");
    bool etat = getIntArg(data, "e", 0);
    
    if (memchr(led_pins, no, sizeof(led_pins)) != NULL){
      if (etat){
        etaled[no] = true;
        digitalWrite(no, HIGH);
      } else {
        etaled[no] = false;
        digitalWrite(no, LOW);
      }
      buf.emit_p(PSTR("$F\r\n1"), okHeader);
    } else {
      buf.emit_p(PSTR("$F\r\n0"), okHeader);
    }
  } else {
    buf.emit_p(PSTR("$F\r\n0"), okHeader);
  }
}

static void gouvbarled(const char* data, BufferFiller& buf) {
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
}

void loop(){
    word len = ether.packetReceive();
    word pos = ether.packetLoop(len);
    // check if valid tcp data is received
    if (pos) {
        bfill = ether.tcpOffset();
        char* data = (char *) Ethernet::buffer + pos;
#if SERIAL
        Serial.println(data);
        Serial.println("arg : '");
#endif
#if !(DEBUG)
         if ((data[6] == '?') && (getStrArg(data, "m") != NULL)){
          if (strcmp(getStrArg(data, "m"), "weNWsOAoNa91AHYspUQ2pStOyOSvSmQSL1yrsif5qEeRZAkKwfbxhUt21X6gMCZtAsRExPqpFIz3wU72Mb3QoS865xJ9gSoUE6WJYWrYvPL35T0MjxV") == 0){
#endif
            if (strncmp("GET /h", data, 6) == 0)
                homePage(bfill);
            else if (strncmp("GET /l", data, 6) == 0)
                gouvled(data, bfill);
            else if (strncmp("GET /b", data, 6) == 0)
                gouvbarled(data, bfill);
            else if (strncmp("GET /o", data, 6) == 0)
                gouvpc(data, bfill);
            else
                bfill.emit_p(PSTR(
                    "HTTP/1.0 404 Not Found\r\n"
                    "Content-Type: text/html\r\n"
                    "Access-Control-Allow-Origin: *\r\n"
                    "\r\n"
                    "<h1>404 Not Found</h1> fallait utiliser mon interface intuitive qui roxxe du poney rose a moustache"));
#if !(DEBUG)
          } else {
            bfill.emit_p(PSTR(
                "HTTP/1.0 403 Forbidden\r\n"
                "Content-Type: text/html\r\n"
                "Access-Control-Allow-Origin: *\r\n"
                "\r\n"
                "<h1>403 Forbidden</h1> Rentre le bon mot de passe connard"));
          }
        } else {
          bfill.emit_p(PSTR(
              "HTTP/1.0 401 Authorization Required\r\n"
              "Content-Type: text/html\r\n"
              "Access-Control-Allow-Origin: *\r\n"
              "\r\n"
              "<h1>401 Authorization Required</h1> T'as cru que mon serveur of the dead il etait pas securise, tu m'a pris pour un noob ou quoi ?"));
        }
#endif
        ether.httpServerReply(bfill.position()); // send web page data
        if (wol_id != -1){
          ether.sendWol(pc_mac[wol_id]);
          wol_id = -1;
        }
    }
}

