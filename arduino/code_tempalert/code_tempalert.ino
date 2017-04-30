#include<Adafruit_NeoPixel.h>
#include<Wire.h>
#include<lm75.h>
Adafruit_NeoPixel ColorLED = Adafruit_NeoPixel (1, 6, NEO_GRB + NEO_KHZ800);
TempI2C_LM75 termo = TempI2C_LM75(0x48, TempI2C_LM75::nine_bits);
void setup() {
  Serial.begin(9600);
  ColorLED.begin();

}
float hue = 0;
int rgb[3] = {0, 0, 0};
bool alert = false;
int index = 0;
void loop() {
  float temp = termo.getTemp() + 2; //误差调节
  Serial.print(temp);
  Serial.print(".C  ");
  hue = Temp2Hue(temp);
  Serial.print("hue=");
  Serial.print(hue);
  Hsv2Rgb(hue, 1, 1, rgb[0], rgb[1], rgb[2]);
  Serial.print("  grb=");
  Serial.print(rgb[0]);
  Serial.print(",");
  Serial.print(rgb[1]);
  Serial.print(",");
  Serial.println(rgb[2]);
  ColorLED.setPixelColor(0, ColorLED.Color(rgb[0], rgb[1], rgb[2]));
  ColorLED.show();
  if (temp >= 38.0)alert = true;//超过38度报警
  else alert = false;
  if (alert) {
    if (index >= 0 && index < 1200) {
      if (index < 600)tone(8, 1200 - index);
      else tone(8, index);
      index += 20;
    }
    else index = 0;
  }
  else noTone(8);
}
float Temp2Hue(float temp) {
  //人的正常体温为35.8-37.2°C
  float downlimit = 35.8, uplimit = 37.2;
  if (temp <= downlimit)temp = downlimit;
  else if (temp >= uplimit)temp = uplimit;
  return (uplimit - temp) / (uplimit - downlimit) * 240;
}
void Hsv2Rgb(float h, float s, float v, int &red, int &green, int&blue) {
  if (!(h >= 0 && h <= 360))return;
  if (!(s >= 0 && s <= 1))return;
  if (!(v >= 0 && v <= 1))return;
  float r = 0, g = 0, b = 0;
  int i = (int)(h / 60) % 6;
  float f = (h / 60) - i;
  float p = v * (1 - s);
  float q = v * (1 - f * s);
  float t = v * (1 - (1 - f) * s);
  switch (i) {
    case 0:
      r = v;
      g = t;
      b = p;
      break;
    case 1:
      r = q;
      g = v;
      b = p;
      break;
    case 2:
      r = p;
      g = v;
      b = t;
      break;
    case 3:
      r = p;
      g = q;
      b = v;
      break;
    case 4:
      r = t;
      g = p;
      b = v;
      break;
    case 5:
      r = v;
      g = p;
      b = q;
      break;
    default:
      break;
  }
  red = (int)(r * 255);
  green = (int)(g * 255);
  blue = (int)(b * 255);
  return;
}
