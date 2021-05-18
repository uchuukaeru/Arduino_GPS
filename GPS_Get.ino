#include<SoftwareSerial.h>
#include<TinyGPS++.h>
#include<SPI.h>
#include<SD.h>
const int chipSelect=4;//魔法の数字(?)
int lper=0;//終了フラグ用変数
#define DEVNUM "dev00"//デバイス識別番号：dev00 - 試作・試験機
#define FILEPATH "/GPSdata_dev00.csv"

TinyGPSPlus gps;
SoftwareSerial mySerial(0,1);//シリアルの設定　0pin:RX 1pin:TX

void setup() {
  //pinMode(SS.OUTPUT);
  SD.begin(chipSelect);//初期化
  Serial.begin(57600);//モニターのシリアル通信速度
  //attachInterrupt(0,stop_loop,LOW);//割り込み処理の設定,3pin,プルダウン
  Serial.println("hi,user");
  Serial.println(DEVNUM);
  mySerial.begin(9800);//GPSのシリアル通信速度
  while(!mySerial){
    ;
  }
  Serial.println("Hello");
  mySerial.println("Hello,World?\n");
  //mySerial.print("%s",DEVNUM);
  mySerial.print("setup\n");
/*  while(1){//test.txtを捜索する
    Serial.println(SD.exists("/test.txt"));//test.txtの有無をモニタに表示
    if(SD.exists("/test.txt")==1)break;//test.txtが存在するなら終了 <== 生成に変更
    delay(1000);
  }*/
  File dataFile=SD.open(FILEPATH,FILE_WRITE);
    dataFile.print(DEVNUM);dataFile.print("\n");
  dataFile.close();
  Serial.println("GO!");
}
int i=0;

void loop() {
  char g=mySerial.read();//GPSからデータを受信する。
  gps.encode(g);//送られてきたデータを使いやすいように加工する。
  File dataFile=SD.open(FILEPATH,FILE_WRITE);//GPSdata.csvをwriteモードで展開。
  while(mySerial.available() > 0){//GPSからデータが送られているなら処理を行う。
    char c=mySerial.read();//GPSからデータを受信する。
    gps.encode(c);//送られてきたデータを使いやすいように加工する。
    if(gps.location.isUpdated()){
      //画面表示時間
      Serial.print(gps.date.year()); Serial.print("-"); Serial.print(gps.date.month()); Serial.print("-"); Serial.print(gps.date.day());
      Serial.print(","); Serial.print((gps.time.hour())); Serial.print(":"); Serial.print(gps.time.minute()); Serial.print(",");
      //SD書き込み時間
      dataFile.print(gps.date.year());dataFile.print("-"); dataFile.print(gps.date.month()); dataFile.print("-"); dataFile.print(gps.date.day());
      dataFile.print(","); dataFile.print((gps.time.hour())); dataFile.print(":"); dataFile.print(gps.time.minute()); Serial.print(",");
      //画面表示座標
      Serial.print(gps.location.lat(),6);Serial.print(",");Serial.print(gps.location.lng(),6);
      Serial.print("\n");
      //SD書き込み座標
      dataFile.print(gps.location.lat(),6); dataFile.print(","); dataFile.print(gps.location.lng(),6);
      dataFile.print("\n");
    }
  }
  dataFile.close();//GPSdata_dev00.csvを閉じる。
  if(lper==1){//終了フラグの判断
    while(1){
      Serial.println("STOP!!");
      delay(1000);
    }
  }
}

void stop_loop(){//終了フラグを立てる割り込み関数
  lper=1;
}

/*
5/26
位置情報がSDカードに書き込めない
＞SDカードのフォーマットの仕方？　違った　解決　File関数を開く位置がloop関数外だったため

*/
