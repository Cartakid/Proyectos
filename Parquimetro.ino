#include <SoftwareSerial.h>
#include <MFRC522.h>
#include <SPI.h>
#include<Time.h>
//#include <LiquidCrystal.h>

#define SS_PIN 10
#define RST_PIN 9
 
MFRC522 rfid(SS_PIN, RST_PIN); // instanciar la clase
MFRC522::MIFARE_Key key; 
//LiquidCrystal lcd(7, 6, 5, 4, 3, 2); //imprimir en display

int b=0, a=0, c=0;
String uidString;
unsigned long timea1=0, timea2=0; //variables de tiempo para llavero
unsigned long timec1=0, timec2=0; //variables de tiempo para tarjeta;
unsigned long dt1=0, dt2=0; //diferencia de tiempos
float ca=0, ct=0; //precio a pagar 
String z="Por favor pague Q";


void setup(){     
  SPI.begin(); // iniciar SPI bus
  rfid.PCD_Init(); // iniciar MFRC522 
  Serial.begin(9600); //iniciar monitor serial
  pinMode(8, OUTPUT); //LED tag
  pinMode(7, OUTPUT); //LED llavero
  //lcd.begin(16, 2);
  
}

void loop(){
  if(rfid.PICC_IsNewCardPresent()){
    rfid.PICC_ReadCardSerial();
    Serial.println(F(""));
    //lcd.print("Tag");
    MFRC522::PICC_Type Tipo = rfid.PICC_GetType(rfid.uid.sak); //obtener el tipo de rfid que utiliza
 
    uidString = String(rfid.uid.uidByte[0])+String(rfid.uid.uidByte[1])+String(rfid.uid.uidByte[2])+String(rfid.uid.uidByte[3]);
    int b=uidString.toInt();

    if(b==15243 && a==0){
      Serial.println("Acceso tag");//permite ingresar con el llavero
      //lcd.print("Acceso tag");//permite ingresar con el llavero
      digitalWrite(8, HIGH); //encender LED candado
      timea1=millis();
      delay(800);
      a=1;

    }else if(b==15243 && a==1){
      timea2=millis();
      dt1=timea2-timea1;
      ca=dt1*0.000009;
      String x=z+ca;
      Serial.println(x);//indica que el llavero debe de pagar  
      //lcd.print(x);//indica que el llavero debe de pagar  
      digitalWrite(8, LOW); //apagar LED candado
      delay(800);
      a=0;
 
    }else if(b!=15243 && c==0){
      Serial.println("Acceso correcto tarjeta");//permite ingresar con la tarjeta
      //lcd.print("Acceso tarjeta");
      digitalWrite(7, HIGH); //encender LED candado
      timec1= millis();
      delay(800);
      c=1;
  
    }else if(b!=15243 && c==1){
      long timec2= millis();
      dt2=timec2-timec1;
      ct=dt2*0.000009; 
      String y=z+ct;  
     Serial.println(y);//indica que la tarjeta debe de pagar 
      //lcd.print(y);
      digitalWrite(7, LOW); //apagar LED candado
      delay(800);
      c=0;
    }
    
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  
  }
}
