// Fill-in information from your Blynk Template here
#define BLYNK_TEMPLATE_ID "TMPL6vkyJNzAN"
#define BLYNK_DEVICE_NAME "GAS DET"

#define BLYNK_FIRMWARE_VERSION        "0.1.0"
#define BLYNK_PRINT Serial
#define APP_DEBUG
#define USE_NODE_MCU_BOARD
#include "BlynkEdgent.h"
#define buzzer D1
#define ledmq2 D0
#define firesensor D2
int mucCanhbao=200;
BlynkTimer timer;
int timerID1,timerID2;
int mq2_value;
int firesensor_value;
//int button=0; //D3
//boolean buttonState=HIGH;
boolean runMode=1;//Bật/tắt chế độ cảnh báo
boolean canhbaoState=0;
WidgetLED led(V0);

void setup()
{
  Serial.begin(115200);
  delay(100);
  //pinMode(button,INPUT_PULLUP);
  pinMode(buzzer,OUTPUT);
  pinMode(ledmq2,OUTPUT);
  digitalWrite(buzzer,LOW); //Tắt buzzer
  digitalWrite(ledmq2,LOW); //tat ledmq2
  BlynkEdgent.begin();
  timerID1 = timer.setInterval(1000L,handleTimerID1);
}

void loop() {
  BlynkEdgent.run();
  timer.run();
  //if(digitalRead(button)==LOW){
    //if(buttonState==HIGH){
     // buttonState=LOW;  
     // runMode=!runMode;
      Serial.println("Run mode: " + String(runMode));
      Blynk.virtualWrite(V4,runMode);
      delay(200);
    //}
 // }else{
   // buttonState=HIGH;
  //}
}
void handleTimerID1(){
  mq2_value = analogRead(A0);
  firesensor_value=digitalRead(firesensor);
  Blynk.virtualWrite(V1,mq2_value);
  if(led.getValue()) {
    led.off();
  } else {
    led.on();
  }
  if(runMode==1){
    if (firesensor_value==LOW) {
      if(canhbaoState==0){
        canhbaoState=1; 
        Blynk.logEvent("canh_bao", String("Cảnh báo có cháy!!"));
        digitalWrite(buzzer,HIGH); //delay(1000); digitalWrite(buzzer,LOW);
        digitalWrite(ledmq2,HIGH); //delay(3000); digitalWrite(ledmq2,LOW);
        timerID2 = timer.setTimeout(5000L,handleTimerID2);
      }
      Blynk.virtualWrite(V3,HIGH);
      Serial.println("CÓ CHÁY!");
      delay(3000);
      digitalWrite(buzzer,LOW);
      digitalWrite(ledmq2,LOW);
    } else
      if(mq2_value>mucCanhbao){
        if(canhbaoState==0){
          canhbaoState=1; 
          Blynk.logEvent("canh_bao", String("Cảnh báo! Khí gas=" + String(mq2_value)+" vượt quá mức cho phép!"));
          digitalWrite(buzzer,HIGH); //delay(1000); digitalWrite(buzzer,LOW);
          digitalWrite(ledmq2,HIGH); //delay(3000); digitalWrite(ledmq2,LOW);
          timerID2 = timer.setTimeout(5000L,handleTimerID2);
        }
        Blynk.virtualWrite(V3,HIGH);
        Serial.println("CÓ RÒ RỈ KHÍ GAS!");
        delay(3000);
        digitalWrite(buzzer,LOW);
        digitalWrite(ledmq2,LOW);

      }else{
        digitalWrite(buzzer,LOW);
        Blynk.virtualWrite(V3,LOW);
        Serial.println("Tình trạng: Bình thường");
      }
    }else{
      digitalWrite(buzzer,LOW);
      Blynk.virtualWrite(V3,LOW);
      Serial.println("Tình trạng: Bình thường");
    }
}
void handleTimerID2(){
  canhbaoState=0;
}
BLYNK_CONNECTED() {
  Blynk.syncVirtual(V2,V4);
}
BLYNK_WRITE(V2) {
  mucCanhbao = param.asInt();
}
BLYNK_WRITE(V4) {
  runMode = param.asInt();
}
