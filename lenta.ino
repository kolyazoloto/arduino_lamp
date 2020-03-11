#include <Adafruit_NeoPixel.h>
#include <Ticker.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

const char* ssid = "LALALA";
const char* password = "Kolyathegod";
const char* mqtt_server = "192.168.1.10";
WiFiClient espClient;
PubSubClient client(espClient);
#define LED_ROW 16
#define LED_COL 10
// Указываем, какое количество пикселей у нашей ленты.
#define LED_COUNT 160
#define BUTTON 15
// Указываем, к какому порту подключен вход ленты DIN.
#define LED_PIN 13
int mode = 0;
int color = 0;
int get10Num(char bukva){
  switch (bukva){
    case '0':
      return 0;
      break;
    case '1':
      return 1;
      break; 
    case '2':
      return 2;
      break;  
    case '3':
      return 3;
      break;  
    case '4':
      return 4;
      break;  
    case '5':
      return 5;
      break;  
    case '6':
      return 6;
      break;
     case '7':
      return 7;
      break;  
    case '8':
      return 8;
      break;  
    case '9':
      return 9;
      break;  
    case 'a':
      return 10;
      break;  
    case 'b':
      return 11;
      break;  
    case 'c':
      return 12;
      break;
    case 'd':
      return 13;
      break;  
    case 'e':
      return 14;
      break;
    case 'f':
      return 15;
      break;       
  }   
}
void callbackOnOff(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  char* payloadSTR = (char*)payload;
  int r = get10Num(payloadSTR[0])*16+get10Num(payloadSTR[1]);
  int g = get10Num(payloadSTR[2])*16+get10Num(payloadSTR[3]);
  int b = get10Num(payloadSTR[4])*16+get10Num(payloadSTR[5]);
  Serial.print(r);
  Serial.print(' ');
  Serial.print(g);
  Serial.print(' ');
  Serial.println(b);

}
// Создаем переменную strip для управления нашей лентой.
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
  
void on_lines(int r,int g,int b,int vverhILIvniz=0, int st = 0, int fin = LED_COL){
  for (int i = st; i < fin; i++)
    { 
   
      for (int j = 0; j < LED_ROW; j++){
        if (j % 2 == vverhILIvniz){
          strip.setPixelColor(j*10+i, strip.Color(r, g, b)); // Красный цвет.
        }
        else{
          strip.setPixelColor(j*10+(LED_COL-i-1), strip.Color(r, g, b));
        }
      }
      //strip.show();
      //delay(500);
    }
}
uint32_t rgbcolor;
void randbow_time(){
  
  for (uint16_t i=0;i < 65536; i += 128){
    if(mode != 2) return;
    rgbcolor = strip.ColorHSV(i);
    strip.fill(rgbcolor);
    strip.show();
    delay(50);
  }
}
void wave(int r,int g,int b,int mode = 0){
  int v1=0,v2 = 0;
  switch (mode){
  case 1:
    v1 = v2 = 1;
    break;
  case 2:
    v1 = 1;
    v2 = 0;
    break;
  case 3:
    v1 = 0;
    v2 = 1;
    break;
  }
  
  on_lines(r,g,b,v1);
  delay(1500);
  on_lines(0,0,0,v2);
  
}



void single_line(int r,int g,int b,int vverhILIvniz = 0,int tudasuda = 0){
  for (int i = 0; i < LED_COL; i++)
    { 

      for (int j = 0; j < LED_ROW; j++){
        if (j % 2 == vverhILIvniz){
          strip.setPixelColor(j*10+i, strip.Color(r, g, b)); // Красный цвет.
        }
        else{
          strip.setPixelColor(j*10+(LED_COL-i-1), strip.Color(r, g, b));
        }
      }
      //strip.show();
      delay(40);
      
      if (i > 0){
        for (int j = 0; j < LED_ROW; j++){
        if (j % 2 == vverhILIvniz){
          strip.setPixelColor(j*10+i-1, strip.Color(0, 0, 0)); // Красный цвет.
        }
        else{
          strip.setPixelColor(j*10+(LED_COL-i), strip.Color(0, 0, 0));
        }
        
      }
      
      }
      strip.show();
    }
    if (tudasuda == 0)
      for (int j = 0; j < LED_ROW; j++){
          if (j % 2 == vverhILIvniz){
            strip.setPixelColor(j*10+LED_COL-1, strip.Color(0, 0, 0)); // Красный цвет.
          }
          else{
            strip.setPixelColor(j*10, strip.Color(0, 0, 0));
          }
          strip.show();
    }
}

void tuda_suda(int r,int g,int b){
  single_line(r,g,b,0,1);
  single_line(r,g,b,1,1);
}

void up(int vverhILIvniz){
  uint16_t a[LED_COL];
  for (int j = 0; j < LED_ROW; j++){
    if (j % 2 == vverhILIvniz){
      a[j] = strip.getPixelColor(j*10); // Красный цвет.
    }
    else{
      a[j] = strip.getPixelColor(j*10+LED_COL-1);
    }
  }
  for (int i = 1; i < LED_COL; i++)
  { 
      for (int j = 0; j < LED_ROW; j++){
        if (j % 2 == vverhILIvniz){
          strip.setPixelColor(j*10+i-1, strip.getPixelColor(j*10+i)); // Красный цвет.
        }
        else{
          strip.setPixelColor(j*10+(LED_COL-i), strip.getPixelColor(j*10+(LED_COL-i-1)));
        }
      }
  }
  for (int j = 0; j < LED_ROW; j++){
    if (j % 2 == vverhILIvniz){
      strip.setPixelColor(j*10+LED_COL-1,a[j]); // Красный цвет.
    }
    else{
      strip.setPixelColor(j*10,a[j]);
    }
  }
  strip.show();
  delay(1500);
  
}
void kakyato_hueta(int r1,int g1,int b1,int r2,int g2,int b2){
  int r_scale = (r2-r1)/LED_COL;
  int g_scale = (g2-g1)/LED_COL;
  int b_scale = (b2-b1)/LED_COL;
  for (int i = 0; i < LED_COL; i++)
    { 
   
      for (int j = 0; j < LED_ROW; j++){
        if (j % 2 == 0){
          strip.setPixelColor(j*10+i, strip.Color(r1-=r_scale, g1-=g_scale, b1-=b_scale)); 
        }
        else{
          strip.setPixelColor(j*10+(LED_COL-i-1), strip.Color(r1-=r_scale, g1-=g_scale, b1-=b_scale));
        }
      }
    }  
}

void grad(int r1,int g1,int b1,int r2,int g2,int b2,int st = 0,int fin = LED_COL){
  int r_scale = (r2-r1)/(fin-st-1);
  int g_scale = (g2-g1)/(fin-st-1);
  int b_scale = (b2-b1)/(fin-st-1);
  for (int i = st; i < fin; i++)
    { 
   
      for (int j = 0; j < LED_ROW; j++){
        if (j % 2 == 0){
          strip.setPixelColor(j*10+i, strip.Color(r1+r_scale*(i-st), g1+g_scale*(i-st), b1+b_scale*(i-st))); 
        }
        else{
          strip.setPixelColor(j*10+(LED_COL-i-1), strip.Color(r1+r_scale*(i-st), g1+g_scale*(i-st), b1+b_scale*(i-st)));
        }
      }
    }  
}
int colorMode;
void grad_tudasuda(int r2,int g2,int b2,int r3,int g3,int b3,int sh)
{
  int r1_scale=(r2-r3)/sh,g1_scale=(g2-g3)/sh,b1_scale=(b2-b3)/sh;
  grad(r2,g2,b2,r3,g3,b3);
  strip.show();
  colorMode = color;
  delay(3000);
  for (int j = 0; j < sh; j++){
      if(mode != 3) return;
      if(color != colorMode) return;
      //тут изменение режима
     //if (aloha != 2){break;}
     grad(r2-r1_scale*j,g2-g1_scale*j,b2-b1_scale*j,r3+r1_scale*j,g3+g1_scale*j,b3+b1_scale*j);
     strip.show();
     
     delay(10);
  }
  grad(r3,g3,b3,r2,g2,b2);
  strip.show();
  delay(3000);
  for (int j = 0; j < sh; j++){
    if(mode != 3) return;
    if(color != colorMode) return;
     //if (aloha != 2){break;} // тут изменение режима
     grad(r3+r1_scale*j,g3+g1_scale*j,b3+b1_scale*j,r2-r1_scale*j,g2-g1_scale*j,b2-b1_scale*j);
     strip.show();
     delay(10);
  }
}
void tyhnet(uint16_t n_pix)
{
    double v;
    uint8_t r,g,b;
    uint16_t cvet = strip.getPixelColor(n_pix);
    r = cvet >> 16;
    g = cvet >> 8;
    b = cvet; 
    v=random(10,20);
    for(int i=1;i<=100/v;i++){
      strip.setPixelColor(n_pix,strip.Color(r*((100-v*i)/100),g*((100-v*i)/100),b*((100-v*i)/100)));
      strip.show();
    }
    strip.setPixelColor(n_pix,strip.Color(0,0,0));
    strip.show();
    
}
void rand_pix(int colorMod = 0)
{   
    //strip.clear();
    //strip.show();
    uint16_t n_pix[LED_COUNT];
    int cvet[6];
    if (colorMod == 0){
      cvet[0]=strip.Color(0,255,0);
      cvet[1]=strip.Color(0,0,255);
      cvet[2]=strip.Color(255,255,0);
      cvet[3]=strip.Color(255,0,255);
      cvet[4]=strip.Color(0,255,255);
      cvet[5]=strip.Color(255,0,0);
    }
    if (colorMod == 1){
      cvet[0]=strip.Color(255,255,255);
      cvet[1]=strip.Color(255,255,255);
      cvet[2]=strip.Color(255,255,255);
      cvet[3]=strip.Color(0,0,0);
      cvet[4]=strip.Color(0,0,0);
      cvet[5]=strip.Color(0,0,0);
    }
    if (colorMod == 2){
      cvet[0]=strip.Color(255,255,255);
      cvet[1]=strip.Color(255,255,255);
      cvet[2]=strip.Color(255,0,0);
      cvet[3]=strip.Color(255,0,0);
      cvet[4]=strip.Color(0,0,255);
      cvet[5]=strip.Color(0,0,255);
    }
    if (colorMod == 3){
      cvet[0]=strip.Color(255,255,0);
      cvet[1]=strip.Color(255,0,0);
      cvet[2]=strip.Color(255,100,0);
      cvet[3]=strip.Color(255,255,0);
      cvet[4]=strip.Color(255,0,0);
      cvet[5]=strip.Color(255,100,0);
    }
    if (colorMod == 4){
      cvet[0]=strip.Color(255,0,255);
      cvet[1]=strip.Color(255,0,255);
      cvet[2]=strip.Color(124,25,200);
      cvet[3]=strip.Color(124,25,200);
      cvet[4]=strip.Color(0,0,0);
      cvet[5]=strip.Color(0,0,0);
    }
    if (colorMod == 5){
      cvet[0]=strip.Color(255,100,0);
      cvet[1]=strip.Color(255,100,0);
      cvet[2]=strip.Color(255,100,0);
      cvet[3]=strip.Color(0,100,255);
      cvet[4]=strip.Color(0,100,255);
      cvet[5]=strip.Color(0,100,255);
    }
    int count;
    count = random(50,160);
    for(int i=0;i<count;i++)
    {
    n_pix[i] = random(0,LED_COUNT);
    if(strip.getPixelColor(n_pix[i])==0)
      strip.setPixelColor(n_pix[i],cvet[random(0,6)]);//strip.Color(random(0,255),random(0,255),random(0,255)));
    else
      strip.setPixelColor(n_pix[i],strip.Color(0,0,0));
    strip.show();
    //delay(20);
    }
    
    //for(int i = 0;i<count;i++)
      //tyhnet(n_pix[i]);
}
void burn(int h = 3,int colorMode = 0)
{
      //on_lines(0,0,0,0);
      //on_lines(255,0,0,0,0,h);
      for(int i=0;i<LED_ROW;i++)
      {
        strip.show();
        delay(2);
        int par_s = random(0,7);
        if (colorMode == 0) {
          for(int j=0;j<par_s+h;j++)
             if (i % 2 == 0)
                strip.setPixelColor(i*10+j, strip.Color(255-5*j,5*j,0)); 
             else
                strip.setPixelColor(i*10+(LED_COL-j-1), strip.Color(255-5*j,5*j,0));
          for(int j=par_s+h;j<LED_COL;j++)
             if (i % 2 == 0)
                strip.setPixelColor(i*10+j, strip.Color(0,0,0)); 
             else
                strip.setPixelColor(i*10+(LED_COL-j-1), strip.Color(0,0,0));
        }
        if (colorMode == 1) { // Синя зеленый
          for(int j=0;j<par_s+h;j++)
             if (i % 2 == 0)
                strip.setPixelColor(i*10+j, strip.Color(0,36*j,255-36*j)); 
             else
                strip.setPixelColor(i*10+(LED_COL-j-1), strip.Color(0,36*j,255-36*j));
          for(int j=par_s+h;j<LED_COL;j++)
             if (i % 2 == 0)  
                strip.setPixelColor(i*10+j, strip.Color(0,0,0)); 
             else
                strip.setPixelColor(i*10+(LED_COL-j-1), strip.Color(0,0,0));
        }
        
        if (colorMode == 2) { // 
          for(int j=0;j<par_s+h;j++)
             if (i % 2 == 0)
                strip.setPixelColor(i*10+j, strip.Color(36*j,255-36*j,0)); 
             else
                strip.setPixelColor(i*10+(LED_COL-j-1), strip.Color(36*j,255-36*j,0));
          for(int j=par_s+h;j<LED_COL;j++)
             if (i % 2 == 0)  
                strip.setPixelColor(i*10+j, strip.Color(255,0,0)); 
             else
                strip.setPixelColor(i*10+(LED_COL-j-1), strip.Color(255,0,0));
        }
        
        if (colorMode == 3) { // Синя зеленый
          for(int j=0;j<par_s+h;j++)
             if (i % 2 == 0)
                strip.setPixelColor(i*10+j, strip.Color(255,36*j,255-36*j)); 
             else
                strip.setPixelColor(i*10+(LED_COL-j-1), strip.Color(255,36*j,255-36*j));
          for(int j=par_s+h;j<LED_COL;j++)
             if (i % 2 == 0)  
                strip.setPixelColor(i*10+j, strip.Color(0,0,0)); 
             else
                strip.setPixelColor(i*10+(LED_COL-j-1), strip.Color(0,0,0));
        }
        if (colorMode == 4) { // Синя зеленый
          for(int j=0;j<par_s+h;j++)
             if (i % 2 == 0)
                strip.setPixelColor(i*10+j, strip.Color(36*j,36*j,100)); 
             else
                strip.setPixelColor(i*10+(LED_COL-j-1), strip.Color(36*j,36*j,100));
          for(int j=par_s+h;j<LED_COL;j++)
             if (i % 2 == 0)  
                strip.setPixelColor(i*10+j, strip.Color(0,0,0)); 
             else
                strip.setPixelColor(i*10+(LED_COL-j-1), strip.Color(0,0,0));
        }
        if (colorMode == 5) { // Синя зеленый
          for(int j=0;j<par_s+h;j++)
             if (i % 2 == 0)
                strip.setPixelColor(i*10+j, strip.Color(100,36*j,100)); 
             else
                strip.setPixelColor(i*10+(LED_COL-j-1), strip.Color(255,36*j,0));
          for(int j=par_s+h;j<LED_COL;j++)
             if (i % 2 == 0)  
                strip.setPixelColor(i*10+j, strip.Color(255,255,255)); 
             else
                strip.setPixelColor(i*10+(LED_COL-j-1), strip.Color(255,255,255));
        }
    }
        
}
int H[LED_ROW];
bool first_shit = true;
void burn2(int h = 3,int colorMode = 0)
{
      //on_lines(0,0,0,0);
      //on_lines(255,0,0,0,0,h);
      int par_s, par_n;
      if(first_shit)
      {
        for(int c=0;c<LED_ROW;c++)
          H[c]=h;
        first_shit = false;
      }
      for(int i=0;i<LED_ROW;i++)
      {
        strip.show();
        delay(2);
        do
        {
          par_s = random(1,4);
          par_n = random(-1,2);
        }while((H[i]+par_s*par_n)<h || (H[i]+par_s*par_n)>=7);
        H[i]+=par_s*par_n;
        if (colorMode == 0) {
          for(int j=0;j<H[i];j++)
             if (i % 2 == 0)
                strip.setPixelColor(i*10+j, strip.Color(255-5*j,5*j,0)); 
             else
                strip.setPixelColor(i*10+(LED_COL-j-1), strip.Color(255-5*j,5*j,0));
          for(int j=H[i];j<LED_COL;j++)
             if (i % 2 == 0)
                strip.setPixelColor(i*10+j, strip.Color(0,0,0)); 
             else
                strip.setPixelColor(i*10+(LED_COL-j-1), strip.Color(0,0,0));
        }
        if (colorMode == 1) { // Синя зеленый
          for(int j=0;j<H[i];j++)
             if (i % 2 == 0)
                strip.setPixelColor(i*10+j, strip.Color(0,36*j,255-36*j)); 
             else
                strip.setPixelColor(i*10+(LED_COL-j-1), strip.Color(0,36*j,255-36*j));
          for(int j=H[i];j<LED_COL;j++)
             if (i % 2 == 0)  
                strip.setPixelColor(i*10+j, strip.Color(0,0,0)); 
             else
                strip.setPixelColor(i*10+(LED_COL-j-1), strip.Color(0,0,0));
        }
        
        if (colorMode == 2) { // 
          for(int j=0;j<H[i];j++)
             if (i % 2 == 0)
                strip.setPixelColor(i*10+j, strip.Color(36*j,255-36*j,0)); 
             else
                strip.setPixelColor(i*10+(LED_COL-j-1), strip.Color(36*j,255-36*j,0));
          for(int j=H[i];j<LED_COL;j++)
             if (i % 2 == 0)  
                strip.setPixelColor(i*10+j, strip.Color(255,0,0)); 
             else
                strip.setPixelColor(i*10+(LED_COL-j-1), strip.Color(255,0,0));
        }
        
        if (colorMode == 3) { // Синя зеленый
          for(int j=0;j<H[i];j++)
             if (i % 2 == 0)
                strip.setPixelColor(i*10+j, strip.Color(255,36*j,255-36*j)); 
             else
                strip.setPixelColor(i*10+(LED_COL-j-1), strip.Color(255,36*j,255-36*j));
          for(int j=H[i];j<LED_COL;j++)
             if (i % 2 == 0)  
                strip.setPixelColor(i*10+j, strip.Color(0,0,0)); 
             else
                strip.setPixelColor(i*10+(LED_COL-j-1), strip.Color(0,0,0));
        }
        if (colorMode == 4) { // Синя зеленый
          for(int j=0;j<H[i];j++)
             if (i % 2 == 0)
                strip.setPixelColor(i*10+j, strip.Color(36*j,36*j,100)); 
             else
                strip.setPixelColor(i*10+(LED_COL-j-1), strip.Color(36*j,36*j,100));
          for(int j=H[i];j<LED_COL;j++)
             if (i % 2 == 0)  
                strip.setPixelColor(i*10+j, strip.Color(0,0,0)); 
             else
                strip.setPixelColor(i*10+(LED_COL-j-1), strip.Color(0,0,0));
        }
        if (colorMode == 5) { // Синя зеленый
          for(int j=0;j<H[i];j++)
             if (i % 2 == 0)
                strip.setPixelColor(i*10+j, strip.Color(100,36*j,100)); 
             else
                strip.setPixelColor(i*10+(LED_COL-j-1), strip.Color(255,36*j,0));
          for(int j=H[i];j<LED_COL;j++)
             if (i % 2 == 0)  
                strip.setPixelColor(i*10+j, strip.Color(255,255,255)); 
             else
                strip.setPixelColor(i*10+(LED_COL-j-1), strip.Color(255,255,255));
        }
    }
    //Serial.println();
        
}
int vrem = -1; 
uint16_t Raduga[15][3];
void init_lgbt()
{
    Raduga[0][0]=Raduga[1][0]=Raduga[2][0]=Raduga[3][0]=Raduga[4][0]=0;
    Raduga[0][2]=Raduga[1][2]=Raduga[2][2]=Raduga[3][2]=Raduga[4][2]=Raduga[5][2]=Raduga[6][2]=0;
    Raduga[7][0]=Raduga[8][0]=Raduga[9][0]=Raduga[10][0]=Raduga[11][0]=0;
    Raduga[12][1]=Raduga[13][1]=Raduga[14][1]=Raduga[15][1]=Raduga[10][2]=Raduga[11][2]=0;
    Raduga[0][1]=Raduga[1][1]=Raduga[2][1]=-4;
    Raduga[3][1]=Raduga[4][1]=-20;
    Raduga[5][0]=Raduga[6][0]=42;
    Raduga[5][1]=-15;
    Raduga[6][1]=-18;
    Raduga[7][1]=Raduga[8][1]=Raduga[9][1]=7;
    Raduga[7][2]=Raduga[8][2]=-28;
    Raduga[9][2]=-29;
    Raduga[10][1]=Raduga[11][1]=32;
    Raduga[12][0]=Raduga[13][0]=-11;
    Raduga[12][2]=7;
    Raduga[13][2]=8;
    Raduga[14][0]=-32;
    Raduga[15][0]=-30;
    Raduga[14][2]=Raduga[15][2]=35;
}
void lgbt_min()
{
    for(int i=0;i<LED_COL;i++)
    {
      strip.setPixelColor(i, strip.Color(252,0,0)); 
      strip.setPixelColor(10+i, strip.Color(252,12,0));
      strip.setPixelColor(20+i, strip.Color(252,24,0));
      strip.setPixelColor(30+i, strip.Color(252,36,0));
      strip.setPixelColor(40+i, strip.Color(252,96,0));
      strip.setPixelColor(50+i, strip.Color(252,156,0));
      strip.setPixelColor(60+i, strip.Color(126,201,0));
      strip.setPixelColor(70+i, strip.Color(0,255,0));
      strip.setPixelColor(80+i, strip.Color(0,234,84));
      strip.setPixelColor(90+i, strip.Color(0,213,168));
      strip.setPixelColor(100+i, strip.Color(0,192,255));
      strip.setPixelColor(110+i, strip.Color(0,96,255));
      strip.setPixelColor(120+i, strip.Color(0,0,255));
      strip.setPixelColor(130+i, strip.Color(33,0,234));
      strip.setPixelColor(140+i, strip.Color(66,0,210));
      strip.setPixelColor(150+i, strip.Color(162,0,105));  
    }
    strip.show();
    delay(10);
}
void lgbt()
{
    if(vrem == -1)
    {
      lgbt_min();
      init_lgbt();
      vrem++;
    }
    vrem++;
    uint16_t t;
    uint8_t r,g,b;
    uint16_t temp[3];
    for(int i=0;i<LED_COL;i++)
    {
      for(int j=1;j<LED_ROW;j++)
      {
        t = strip.getPixelColor(j*10+i);
        r = t >> 16;
        g = t >> 8;
        b = t; 
        strip.setPixelColor(j*10+i, strip.Color(r+Raduga[j-1][0],g+Raduga[j-1][1],b+Raduga[j-1][2]));
      }
        t = strip.getPixelColor(i);
        r = t >> 16;
        g = t >> 8;
        b = t; 
        strip.setPixelColor(i, strip.Color(r+Raduga[15][0],g+Raduga[15][1],b+Raduga[15][2]));
    }
    if(vrem == 3)
    {
      vrem = 0;
      temp[0]=Raduga[15][0]; 
      temp[1]=Raduga[15][1];
      temp[2]=Raduga[15][2];
      for(int v=LED_ROW-1;v>0;v--)
      {
          Raduga[v][0]=Raduga[v-1][0];
          Raduga[v][1]=Raduga[v-1][1];
          Raduga[v][2]=Raduga[v-1][2];
      }
      Raduga[0][0]=temp[0];
      Raduga[0][1]=temp[1];
      Raduga[0][2]=temp[2];
    }
    strip.show();
    delay(10);
}
int cyrc_color;
int color_mode1[16] = {0,4096,8192,12276,16368,20460,24552,28644,32736,36828,40920,45012,49104,53196,57288,61384};
int color_mode2[10] = {0,3500,10000,19660,26214,32768,39321,45875,52428,58982};   
void lgbt_kolya()
{
    if (color == 0){
      for (int k=0;k!=65536;k += 1024){
        if(mode != 6 || color != 0) return;
        for(int i=0;i<LED_ROW;i++)
        {
          cyrc_color = color_mode1[i] + k;
          if (cyrc_color > 65536) cyrc_color = cyrc_color - 65536;
          strip.fill(strip.ColorHSV(cyrc_color),10*i,10); 
        }
       strip.show();
       delay(50);
      }
    }
    //РЕЖИМ ОТ ЦЕНТРА
    if (color == 1){
      for (int k=0;k!=65536;k += 1024){
        if(mode != 6 || color != 1) return;
        for(int i=0;i<LED_COL;i++)
        {
          cyrc_color = color_mode2[i] + k;
          if (cyrc_color > 65536) cyrc_color = cyrc_color - 65536;
          for (int l=0;l<LED_COUNT;l += 10){ 
            strip.setPixelColor(i+l, strip.ColorHSV(cyrc_color)); 
          }  
        }
        strip.show();
        delay(50);
      }
    }
    if (color == 2){
      for (int k=0;k!=65536;k += 1024){
        if(mode != 6 || color != 2) return;
        for(int i=0;i<LED_COL;i++){
          cyrc_color = color_mode2[i] + k;
          if (cyrc_color > 65536) cyrc_color = cyrc_color - 65536;
          for (int j=0;j<LED_ROW;j++){
            if  (j % 2 == 0){
              strip.setPixelColor(j*10+i, strip.ColorHSV(cyrc_color)); 
            }
            else{
              strip.setPixelColor(j*10+(LED_COL-i-1), strip.ColorHSV(cyrc_color));
            }
          }  
        }
      strip.show();
      delay(50);
      }
    }
    if (color > 2) color = 0;  
}      

Ticker checkButton ;//сколько раз нажата клавиша
int buttonPushed = 0;
int prev_time;
int now_time;
bool state;

uint8_t brightness = 255;
bool brightness_reverse = 1;
bool long_pressed=0;

//analogWrite(15, brightness);

ICACHE_RAM_ATTR void interruptRisingFunc(){
  state = 1;
  long_pressed = 0;

  prev_time = millis();

  while(state = digitalRead(BUTTON)  == 1){
    now_time = millis();
    if ((now_time - prev_time) > 1000){

      if (brightness_reverse == 0) {brightness++;}
      else {brightness--;}
      if (brightness == 255 || brightness == 0) {
        brightness_reverse = !brightness_reverse;
        strip.setBrightness(brightness);
        strip.show();
        Serial.println (brightness);
        break;
      } //хз че такое но если ноль то не сразу обноляется
      strip.setBrightness(brightness);
      strip.show();
      Serial.println (brightness);
      //analogWrite(4, brightness);
      //stripOn = !stripOn;
      //long_pressed = 1;

    }
    
    delayMicroseconds(10000);
  }
  if ((now_time - prev_time) > 1000) long_pressed = 1;
  
  if (long_pressed == 0){
    if (buttonPushed == 0){
      checkButton.once(1,callback);
    }
    if (buttonPushed < 3){
      buttonPushed++;
    }
    Serial.println ("button pushed");
    Serial.println (buttonPushed);
  }
}



void callback(){
  if (buttonPushed == 1){
    mode++;
    if (mode == 7){
      mode = 0;  
    }
    vrem = -1;
    color = 0;
    first_shit = true;
    Serial.println ("mode");
    Serial.println (mode);
  }
  if (buttonPushed == 2){
    //mode++;
    //if (mode == 5){
    //  mode = 0;
    if (color<5){ 
      color++; 
    }
    else {color = 0;}
    Serial.println ("change color"); 
   }
    
    //Serial.println (mode);
  
  buttonPushed = 0; 
}
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.subscribe("homeassistant/light/ESP-3bd20b/rgb/set");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void setup()
{
  pinMode(BUTTON, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON), interruptRisingFunc, RISING);
  strip.begin();
  Serial.begin(115200);
  Serial.println("starting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  ArduinoOTA.setHostname("LAMP");
  ArduinoOTA.onStart([]() {
  Serial.println("Start");  //  "Начало OTA-апдейта"
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");  //  "Завершение OTA-апдейта"
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    //  "Ошибка при аутентификации"
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    //  "Ошибка при начале OTA-апдейта"
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    //  "Ошибка при подключении"
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    //  "Ошибка при получении данных"
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
    //  "Ошибка при завершении OTA-апдейта"
  });
  ArduinoOTA.begin();
  Serial.println("Ready");  //  "Готово"
  Serial.print("IP address: ");  //  "IP-адрес: "
  Serial.println(WiFi.localIP());
  client.setServer(mqtt_server, 1883);
  client.setCallback(callbackOnOff);
}

void loop()
 {  ArduinoOTA.handle();
    if (!client.connected()) {
     reconnect();
    }
    client.loop();
    if (mode == 1){
      if (color == 0){
          strip.fill(strip.Color(255, 109, 0));
          strip.show();
      }
      if (color == 1){
          strip.fill(strip.Color(255, 138, 18));
          strip.show();
      }
      if (color == 2){
          strip.fill(strip.Color(255, 161, 72));
          strip.show();
      }
      if (color == 3){
          strip.fill(strip.Color(255, 180, 107));
          strip.show();
      }
      if (color == 4){
          strip.fill(strip.Color(255, 196, 137));
          strip.show();
      }
      if (color == 5){
          strip.fill(strip.Color(255, 209, 163));
          strip.show();
      }
    }
    if (mode == 2){
      randbow_time();
    }
    if (mode == 3){
      if (color == 0){
        grad_tudasuda(0,255,0,0,0,255,255);
      }
      if (color == 1){
        grad_tudasuda(255,0,0,255,255,0,255);
      }
      if (color == 2){
        grad_tudasuda(255,255,255,0,0,0,255);
      }
      if (color == 3){
        grad_tudasuda(0,255,0,255,255,0,255);
      }
      if (color == 4){
        grad_tudasuda(255,0,255,124,25,200,255);
      }
      if (color == 5){
        grad_tudasuda(255,100,0,0,100,255,255);
      }
    }
    if (mode == 4){
      rand_pix(color);
    }
    if (mode == 5){
      burn2(1,color);
    }
    if (mode == 6){
      lgbt_kolya();
    }
    if (mode == 0){
      strip.clear();
      strip.show() ;
    }
  }













  
