#include <Adafruit_NeoPixel.h>
int light_state; //定义一个整数变量light_state
int sound_state; //定义一个整数变量sound_state
Adafruit_NeoPixel ColorLED = Adafruit_NeoPixel(1, 6, NEO_GRB + NEO_KHZ800);
void setup()
{
  pinMode(A0, INPUT); //设置光敏传感器引脚为输入状态
  pinMode(A2, INPUT); //设置麦克风引脚为输入状态
  ColorLED.begin();
  Serial.begin(9600); //设置串口波特率为9600
}
void loop()
{
  light_state = analogRead(A0); //将光敏传感器引脚的值赋给变量state
  sound_state = analogRead(A2); //将麦克风引脚的值赋给变量state
  if (light_state < 200 && sound_state > 600) {
    ColorLED.setPixelColor(0, ColorLED.Color(255, 255, 255));
    ColorLED.show();
    delay(2000);
  }
  else {
    ColorLED.setPixelColor(0, ColorLED.Color(0, 0, 0));
    ColorLED.show();
  }
  Serial.print("light_state:");
  Serial.print(light_state);
  Serial.print(".....sound_state:");
  Serial.println(sound_state);
  delay(100); //延时100毫秒打印
}
