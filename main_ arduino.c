#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <LiquidCrystal_I2C.h>      
#include <SoftwareSerial.h>
#include <ThreeWire.h>  
#include <RtcDS1302.h>
#include <DFPlayer_Mini_Mp3.h>
#include <stdlib.h>
#include <stdio.h>

#define countof(a) (sizeof(a) / sizeof(a[0]))

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

LiquidCrystal_I2C lcd(0x27, 20, 4);  
ThreeWire myWire(4,5,2); // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);
  
int count;
int p;
int np;
int th;
int re;
int s;
int ch;
int E;
  
double A [12];
double B;
double ovj;
double amb;
double temp;
double avg;
double avgth;
double Max;
double Min;

void setup() {
  Serial.begin(9600);
  mlx.begin();
  lcd.init();                                                   // lcd 초기화
  lcd.backlight();                                              // lcd 백라이트를 켠다
  Rtc.Begin();   
  pinMode(13,INPUT);                                            // 모터와 통신
    
  lcd.setCursor(0, 0);                                      
  lcd.print("                 ");
  lcd.setCursor(0, 1);                                   
  lcd.print("                 ");
  lcd.setCursor(0, 2);                                   
  lcd.print("                 ");
  lcd.setCursor(0, 3);                                   
  lcd.print("                 ");
 
  Rtc.Begin();
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);  
  if (!Rtc.IsDateTimeValid()) 
    {   Rtc.SetDateTime(compiled);   }
  if (Rtc.GetIsWriteProtected())
    {   Rtc.SetIsWriteProtected(false);    }
  if (!Rtc.GetIsRunning())
    {   Rtc.SetIsRunning(true);    }
    RtcDateTime now = Rtc.GetDateTime();
  if (now < compiled) 
    {   Rtc.SetDateTime(compiled);    }
  Serial.println("CLEARDATA");  Serial.println("LABLE,DATE,TIME,TEMP,P/NP");    // PLX_DAQ 속성 세팅
}

void loop() { 
  E=0;
  lcd.setCursor(0, 0);                                   
  lcd.print(" Please          ");              
  lcd.setCursor(0, 1);                                  
  lcd.print(" stand on        ");                
  lcd.setCursor(0, 2);                                  
  lcd.print(" the footprints  ");
  lcd.setCursor(0, 3);                                  
  lcd.print("                 ");
  
  if(digitalRead(13)==HIGH){                // 모터로의 신호 수신시 작동
    tone(12,1000,500);
    lcd.setCursor(0, 0);                                   
    lcd.print(" Please check    ");           
    lcd.setCursor(0, 1);                                  
    lcd.print(" the QR code     ");                  
    lcd.setCursor(0, 2);                                   
    lcd.print(" with the module ");
    lcd.setCursor(0, 3);                                  
    lcd.print("                 ");
    delay(8000);

    tone(12,1000,500);
    lcd.setCursor(0, 0);                                  
    lcd.print(" Please          ");
    lcd.setCursor(0, 1);                                  
    lcd.print(" look in         ");                       
    lcd.setCursor(0, 2);                                   
    lcd.print(" the mirror      ");  
    lcd.setCursor(0, 3);                                  
    lcd.print("                 ");
    delay(2000);
        do{ 
//  Serial.print("Object = "); Serial.print(mlx.readObjectTemp1()); Serial.println(" C");
//  Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempC()); Serial.println(" C");     //출력확인용
            ovj = mlx.readObjectTemp1();    // 대상온도
            amb = mlx.readAmbientTempC();   // 공기온도
            temp = ovj - amb;               // 온도 = 대상온도 - 공기온도
   
            if(temp > 0.5){                           
                A[count] = mlx.readObjectTemp1();   // 대상온도와 공기온도의 창가 0.5 이상일 경우 배열에 체온 추가
                A[count] += 10.0;
                B += mlx.readObjectTemp1();         // 체온 평균출력용
                B += 10.0;
                count++;
                delay(5);
                if(count==12){                      // 12개의 체온입력후 최대값과 최솟값구하기
                    Max = A[0];
                    Min = A[0];
                    for(int i = 1 ; i < 12 ; i++){
                        if(Max<A[i]) Max = A[i]; 
                        if(Min>A[i]) Min = A[i]; 
                     }
                    B -= Max;                        // 튀는값 제거 (최솟값과 최대값 제거)
                    B -= Min;
                    avg = B / 10;                    // 평균계산
                    count=0;                         // 카운터 초기화
   
//   Serial.print("avg = "); Serial.println(avg);
//   Serial.println(" ");
   
   
                    if(avg > 33.0 && avg < 37.5) {p++; th++; avgth += avg;}      // 정상 체온범위 확인 
                    else {np++; th++; avgth += avg;}
//   Serial.print("                                                         avgth = "); Serial.println(avgth);
//   Serial.print("                                                         p = "); Serial.println(p);
//   Serial.print("                                                         np = "); Serial.println(np);
//   Serial.print("                                                         th = "); Serial.println(th);
    
                    if(th == 3){                             // 3회에 걸쳐 확인
                        avg = avgth / 3;                   // 최종 체온 계산
//    Serial.print("                                                        avg = "); Serial.println(avg);
                        if(p > 1){                           // 패스가 2개 이상일경우 정상판정
                            tone(12,1000,500);
                            lcd.setCursor(1, 0);                             
                            lcd.print("    PASS       ");    
                            lcd.setCursor(0, 1);                                   
                            lcd.print("               ");
                            lcd.setCursor(0, 2);                                   
                            lcd.print("               ");          
                            lcd.setCursor(1, 3);                               
                            lcd.print(avg);                        
                            lcd.setCursor(7, 3);
                            lcd.print("C");

                            p=0;
                            np=0;
                            th=0;
                            avgth = 0;
                            re=0;
                            ch=1;
                            E=1;
                        }
                        else if(p==1){                       // 패스가 한개일경우 재측정
                        tone(12,1000,500);
                        lcd.setCursor(1, 0);                             
                        lcd.print("    return       ");  
                        lcd.setCursor(0, 1);                                   
                        lcd.print("                 "); 
                        lcd.setCursor(0, 2);                                   
                        lcd.print("                 ");             
                        lcd.setCursor(1, 3);                               
                        lcd.print(avg);                       
                        lcd.setCursor(7, 3);
                        lcd.print("C");
                        re++;
                        if(re==1){                             // 재측정두번일 경우 비정상판정
                            tone(12,1000,500);
                            lcd.setCursor(1, 0);                              
                            lcd.print("Non PASS         "); 
                            lcd.setCursor(0, 1);                                   
                            lcd.print("                 "); 
                            lcd.setCursor(0, 2);                                   
                            lcd.print("                 ");                
                            lcd.setCursor(1, 3);               
                            lcd.print(avg);                      
                            lcd.setCursor(7, 3);
                            lcd.print("C");
                            re=0;
                            ch=0;
                        }
                        }
                        p=0;
                        np=0;
                        th=0;
                        avgth = 0;
                        E=1;
                    }
                    else if (p==0) {                    // 패스가 0개일경우 비정상판정
                        tone(12,1000,500);
                        lcd.setCursor(1, 0);
                        lcd.print("Non PASS         ");
                        lcd.setCursor(0, 1);                                   
                        lcd.print("                 "); 
                        lcd.setCursor(0, 2);                                   
                        lcd.print("                 ");
                        lcd.setCursor(1, 3);
                        lcd.print(avg);
                        lcd.setCursor(7, 3);
                        lcd.print("C");

                        p=0;
                        np=0;
                        th=0;
                        avgth = 0;
                        re=0;
                        ch=0;
                        E=1;
                     }
                }
     count = 0; 
     B = 0;
     Max = 0;
     Min = 0;
     for(int i=0;i<12;i++){ A[i] = 0;}          // 초기화
    }   // if temp END
}while(E==0);   // 모터수신 END
    RtcDateTime now = Rtc.GetDateTime();
    Serial.print("DATA,");     
    printDateTime(now);
    Serial.print(",");         
    Serial.print(avg);
    Serial.print("C ,");
    if(ch==1)Serial.println("P");
    else if(ch==0) Serial.println("NP");
     count = 0; 
     B = 0;
     avg = 0;
     Max = 0;
     Min = 0;
     for(int i=0;i<12;i++){ A[i] = 0; }
     p=0;
     ch=0;
     E=0;
     delay(10000);
    lcd.setCursor(0, 0);                                   
    lcd.print("                 ");
    lcd.setCursor(0, 1);                                   
    lcd.print("                 ");
    lcd.setCursor(0, 2);                                   
    lcd.print("                 ");
    lcd.setCursor(0, 3);                                   
    lcd.print("                 ");
    digitalRead(13)== LOW;
    
  }   //13 END
}     //loop END

void printDateTime(const RtcDateTime& dt)
{
    char datestring[20];

    snprintf_P(datestring, 
            countof(datestring),
            PSTR("%02u/%02u,%02u:%02u"),
            dt.Month(),
            dt.Day(),
            dt.Hour(),
            dt.Minute());
    Serial.print(datestring);
}