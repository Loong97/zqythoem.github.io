#include "JQ6500.h"
#include <Adafruit_NeoPixel.h>  //调用LED彩灯的库文件

//Core和CoreUSB默认使用软串口
SoftwareSerial mySerial(2, 3); // RX, TX
JQ6500 Audio(&mySerial);     //音频控制类实例,使用软串口（2，3）
Adafruit_NeoPixel ColorLED = Adafruit_NeoPixel(2, 8, NEO_GRB + NEO_KHZ800);

//Core+默认使用Serial1
//JQ6500 Audio(&Serial1);

uint8_t musicVol = 1000;             //初始音量20
uint8_t musicMode = MODE_ALL;      //初始播放模式
boolean music_status = false;      //歌曲播放状态
uint16_t fileNum = 0;               //查询当前播放歌曲编号
uint16_t fileNumCache = 0;          //播放歌曲编号缓存
uint16_t totalNum = 0;              //歌曲总数量
uint16_t totalTime = 0;             //当前播放歌曲总时间
uint16_t playTime = 0;              //当前播放歌曲播放时间
uint16_t key = 800;
uint16_t keyCache = 800;
int noteNum=0;
int noteNum_max=10;
String nameCache = "";
int note[10];
int key_GPIO[6]={0,2,4,12,11,10};
int chimemp3[18]={1,3,5,7,9,11,
            13,15,17,19,21,23,
            25,27,29,31,33,35
};
int scale=0;
void note_init();
void setup()
{
    //设置彩灯初始状态

  Serial.begin(9600);
  for(int i=0;i<6;i++){
      pinMode(key_GPIO[i],INPUT);
  }
  Audio.init(DEVICE_TF, musicMode, musicVol);   //播放控制类初始化，设置播放设备为TF卡

  delay(1000);
  totalNum = Audio.queryTF();         //读取TF卡总文件数量
  Audio.choose(1);                  //默认从第一首曲目开始
  Audio.play();                    //暂停
  Serial.println(totalNum);
  noteNum=10;
    ColorLED.begin();
 ColorLED.setPixelColor(0, ColorLED.Color(0,128, 0));  //设置彩灯颜色为红色

}

void loop()
{
  int key_press=404;
  if(noteNum==noteNum_max){
      note_init();
      return;
  }
  for(int i=0;i<6;i++){
    if(digitalRead(key_GPIO[i])==0){
      key_press=i;
      break;
    }
  }
  Serial.print("key_press");
  Serial.println(key_press);
  if(key_press!=404){
    
    if(key_press+scale*6==note[noteNum]){
      noteNum++;
      ColorLED.setPixelColor(0, ColorLED.Color(0, 128, 0));  //设置彩灯颜色为红色
      ColorLED.setPixelColor(1, ColorLED.Color(0, 128, 0));  //设置彩灯颜色为红色
      ColorLED.show();
       Serial.println("true");

    }
    else{
      ColorLED.setPixelColor(0, ColorLED.Color(128, 0, 0));  //设置彩灯颜色为红色
      ColorLED.setPixelColor(1, ColorLED.Color(128, 0, 0));  //设置彩灯颜色为红色
      ColorLED.show();
       Serial.println("false");

    }
     
    Audio.choose(chimemp3[key_press+scale*6]);
    Audio.play();
    delay(2000);
    Audio.pause();
    Serial.print("note:");
   Serial.print(noteNum);
   Serial.print(note[noteNum]);

    
  }
  


//  keyCache = key;
//  key = analogRead(A6);               //获得按键指令
//
//  if (key > 500)
//  {
//    if (keyCache < 50)              //按键按下
//    {
//      music_status = !music_status; //播放或暂停
//      music_status ? Audio.play() : Audio.pause();
//    }
//    else if (keyCache > 50 && keyCache < 100)     //按键左拨
//    {
//      Audio.next();
//    }
//    else if (keyCache > 100 && keyCache < 200)    //按键右拨
//    {
//      Audio.prev();
//    }
//  }

  fileNum = Audio.queryTFFile();        //查询当前播放歌曲的编号
  if (fileNum != fileNumCache) {      //发现播放歌曲更换
    fileNumCache = fileNum;
    nameCache = Audio.queryName();    //查询当前播放歌曲名称
    totalTime = Audio.queryTotalTime();     //查询当前播放歌曲总时间
  }
  playTime = Audio.queryPlayTime();       //查询当前播放歌曲播放时间
//  Serial.print("FileNum:");
//  Serial.print(fileNum);
//  Serial.print(", FileName:");
//  Serial.print(nameCache);
//  Serial.print(", TotleTime:");
//  Serial.print(totalTime);
//  Serial.print(", PlayTime:");
//  Serial.println(playTime);
//   Serial.print(", music_status:");
//  Serial.println(music_status);
  delay(100);
}
void note_init(){
    for(int i=0;i<noteNum_max;i++){
      note[i]=rand()%6;
      Audio.choose(note[i]*2-1);
      Audio.play();
      delay(2000);
      Audio.pause();
    }
    noteNum=0;
}

