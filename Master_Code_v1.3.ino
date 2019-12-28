//  Proyek Akhir Program Studi Agroindustri
//  Politeknik Negeri Subang
//  Nama  : Muhammad Aliyawan Aris Dirgantara
//  NIM   : 10302014
//  
//  Tanggal mulai rancang : 14 Januari 2019
// 

//////////////////////////////////////////////////// 

// --> ******************************************************************* <-- //
// --> ******************************************************************* <-- //
// --> ******************************************************************* <-- //

// --> 1. Suhu Udara dan Kelembaban (DHT11)
  #include "dht.h"
  #define DHT11pin A0

  dht DHT;
  String temphum;

// --> 2. Kapasitas Tangki (Ultrasonik)
  #define trigpin 12
  #define echopin 11

  int duration, distance;

// --> 3. Lux meter (BH1750)
  #include <BH1750.h>

  BH1750 lightSensor;
  String light;

// --> 4. Nutrisi (TDS Meter)
  int R1= 650;
  int Ra= 200; //Resistance of powering Pins
  
  int ECPin=A1;
  int ECGround=A2;
  int ECPower =A3;
  
  int ppm =0;
  
  float PPMconversion=0.7;
  float TemperatureCoef = 0.019;
  
  float K=2;
  float Temperature=10;
  float EC=0;
  float EC25 =0;

  float raw= 0;
  float Vin= 5;
  float Vdrop= 0;
  float Rc= 0;
  float buffer=0;

  #include <OneWire.h>
  #include <DallasTemperature.h>
  #define ONE_WIRE_BUS 10 // Out      
  
  OneWire oneWire(ONE_WIRE_BUS);
  DallasTemperature sensors(&oneWire);
  
  const int TempProbePossitive =9;  // Power 
  const int TempProbeNegative=8;    // Negative 

// --> 6. LCD 83x47 (LCD 5110)
  #include<Wire.h>
  #include <LCD5110_Basic.h>

  LCD5110 myGLCD(3,4,5,7,6);
  extern uint8_t SmallFont[];

// --> 7. Pompa Nutrisi (Motor Pump)
  #define A_Mix 2 // A3
  #define B_Mix 13 // A4

// --> 8. Speaker Buzzer
  int buzz = 0;

// --> ******************************************************************* <-- //
// --> ******************************************************************* <-- //
// --> ******************************************************************* <-- //

void setup() 
{
// --> 1. Begin
  Wire.begin();
  Serial.begin(9600);
  lightSensor.begin();
  sensors.begin();

// --> 2. Input/ Output
  // Speaker Buzzer
    pinMode(buzz, OUTPUT);
    
    digitalWrite(buzz, LOW);

  // Nutrisi (TDS Meter)
    pinMode(ECPin,INPUT);
    pinMode(ECPower,OUTPUT);
    pinMode(ECGround,OUTPUT);
    pinMode(TempProbeNegative, OUTPUT );
    pinMode(TempProbePossitive, OUTPUT );
    
    digitalWrite(TempProbeNegative, LOW );
    digitalWrite(TempProbePossitive, HIGH );
    digitalWrite(ECGround,LOW);
    
    
    //*********Reading Temperature Of Solution *******************//
      sensors.requestTemperatures();
      Temperature=sensors.getTempCByIndex(0); 
     Temperature= 26;
    //************Estimates Resistance of Liquid ****************//
      digitalWrite(ECPower,HIGH);
      raw= analogRead(ECPin);
      raw= analogRead(ECPin);
      digitalWrite(ECPower,LOW);
  
    //***************** Converts to EC **************************//
      Vdrop= (Vin*raw)/1024.0;
      Rc=(Vdrop*R1)/(Vin-Vdrop);
      Rc=Rc-Ra; //acounting for Digital Pin Resitance
      EC = 1000/(Rc*K);
  
    //*************Compensating For Temperaure********************//
      EC25  =  EC/ (1+ TemperatureCoef*(Temperature-25.0));
      ppm=(EC25)*(PPMconversion*1000);

    // Pompa Nutrisi (Motor Pump)
      pinMode(A_Mix, OUTPUT);
      pinMode(B_Mix, OUTPUT);
    
      digitalWrite(A_Mix, HIGH);
      digitalWrite(B_Mix, HIGH);

// --> 3. Intro
  myGLCD.InitLCD();
  myGLCD.setContrast(60);
  myGLCD.setFont(SmallFont);

  myGLCD.print("Proyek Akhir", CENTER, 0);
  myGLCD.print("M Aliyawan A D", CENTER, 16);
  myGLCD.print("10302014", CENTER, 24);
  myGLCD.print("POLSUB", CENTER, 40);

  Serial.println("Proyek Akhir Program Studi Agroindustri");
  Serial.println("Politeknik Negeri Subang");
  Serial.print("Muhammad Aliyawan Aris Dirgantara");
  Serial.println("  ||  NIM : 10302014");
  Serial.println("Juli 2019");
  Serial.println("-------------------------");
  Serial.println(Temperature);

// --> 4. TDS Starting
  delay(100);

  R1=(R1+Ra);

// --> Delay
  delay(5000);
}

void loop() 
{
// --> 1. Clear LCD
  myGLCD.clrScr();

// --> 2. Nutrisi (TDS Meter)
   ppm = 1150;
  //*********Reading Temperature Of Solution *******************//
    sensors.requestTemperatures();
    Temperature=sensors.getTempCByIndex(0); 
    Temperature= 26;
  //************Estimates Resistance of Liquid ****************//
    digitalWrite(ECPower,HIGH);
    raw= analogRead(ECPin);
    raw= analogRead(ECPin);
    digitalWrite(ECPower,LOW);

  //***************** Converts to EC **************************//
    Vdrop= (Vin*raw)/1024.0;
    Rc=(Vdrop*R1)/(Vin-Vdrop);
    Rc=Rc-Ra; 
    EC = 1000/(Rc*K);

  //*************Compensating For Temperaure********************//
    EC25  =  EC/ (1+ TemperatureCoef*(Temperature-25.0));
    ppm=(EC25)*(PPMconversion*1000);
    
  
// --> 3. Suhu dan Kelembaban
  int chk = DHT.read11(DHT11pin);

// --> 4. Lux Meter
  float lux = lightSensor.readLightLevel();
  int lux_decimal;

  if (lux >= 100)
    {
      lux_decimal = 45;
    }

  else if (lux < 100)
    {
      lux_decimal = 48;
    }
  
// --> 5. Kapasitas Tangki (Ultrasonik) 
  int kapasitas;
  int kapasitas_decimal; // = 48 (< 100%)
  
  digitalWrite(trigpin, HIGH);
  delayMicroseconds(1000);  
  digitalWrite(trigpin, LOW);
   
  duration = pulseIn(echopin,HIGH);
  distance = ( duration / 2) / 29.1;

  if (distance <= 8)
    {
      kapasitas = 100;
      kapasitas_decimal = 45;
    }

  else if (kapasitas > 8 && distance <= 14)
    {
      kapasitas = 75;
      kapasitas_decimal = 48;
    }

  else if (distance > 14 && distance <=20)
    {
      kapasitas = 50;
      kapasitas_decimal = 48;
    }

  else if (distance > 20 && distance <= 26)
    {
      kapasitas = 25;
      kapasitas_decimal = 48;
    }

   else if (distance > 26)
    {
      kapasitas = 0;
      kapasitas_decimal = 48;
    }
    
// --> 6. Statement
    if (ppm > 1000)
      {
        Serial.println("<---------     Aliyawan Aris     --------->");
        // --> Suhu Udara
          Serial.print("Suhu.Udara  = ");
          Serial.print(DHT.temperature-3);
          Serial.println(" *C ");

        // --> Suhu Air
          Serial.print("Suhu.Air    = ");
          Serial.print((float)Temperature);
          Serial.println(" *C");

        // --> Kelembaban
          Serial.print("Kelembaban  = ");
          Serial.print(DHT.humidity);
          Serial.println(" %");

        // --> Lux
          Serial.print("Cahaya      = ");
          Serial.print(lux);
          Serial.println(" lux lx");

        // --> Kapasitas Tangki
          Serial.print("Jarak Air         = ");
          Serial.print(distance);
          Serial.println(" cm");
          Serial.print("Kapasitas Air     = ");
          Serial.print(kapasitas);
          Serial.println(" %");
        
        // --> Nutrisi
          Serial.print("Nutrisi     = ");
          Serial.print(ppm);
          Serial.println(" ppm");
        
        // --> Pompa Nutrisi
          Serial.println();
          Serial.print("***Pompa AB Mix");
          Serial.println("***  = ");
          Serial.println("---  ON  ---");
          Serial.println();
        
        // --> On LCD (Pengisian Nutrisi)
          myGLCD.clrScr();
          myGLCD.print("Pengisian",CENTER,0);
          myGLCD.print("AB MIX",CENTER,8);
          myGLCD.print("--- Aktif ---",CENTER,24);
          myGLCD.printNumI(ppm, 20,40);
          myGLCD.print("ppm", 45,40);

        // --> Aktifkan Pompa Nutrisi
          //tone(buzz, 14000, 300);
          
          digitalWrite(A_Mix, LOW);
          digitalWrite(B_Mix, LOW);
          delay(6000);
          //tone(buzz, 14000,300);

          digitalWrite(A_Mix,HIGH);
          digitalWrite(B_Mix,HIGH);
          delay(12000);
      }

     else if (ppm <= 1000)
      {
        Serial.println("<---------     Aliyawan Aris     --------->");
        
        // --> Matikan Pompa
          digitalWrite(A_Mix, HIGH);
          digitalWrite(B_Mix, HIGH);
        
        // --> Suhu Udara
          Serial.print("Suhu.Udara  = ");
          Serial.print(DHT.temperature-3);
          Serial.println(" *C ");

        // --> Suhu Air
          Serial.print("Suhu.Air    = ");
          Serial.print((float)Temperature);
          Serial.println(" *C");

        // --> Kelembaban
          Serial.print("Kelembaban  = ");
          Serial.print(DHT.humidity);
          Serial.println(" %");

        // --> Lux
          Serial.print("Cahaya      = ");
          Serial.print(lux);
          Serial.println(" lux lx");

        // --> Kapasitas Tangki
          Serial.print("Jarak Air         = ");
          Serial.print(distance);
          Serial.println(" cm");
          Serial.print("Kapasitas Air     = ");
          Serial.print(kapasitas);
          Serial.println(" %");
        
        // --> Nutrisi
          Serial.print("Nutrisi     = ");
          Serial.print(ppm);
          Serial.println(" ppm");
        
        // --> Pompa Nutrisi
          Serial.println();
          Serial.print("***Pompa AB Mix");
          Serial.println("***  = ");
          Serial.println("---  OFF  ---");
          Serial.println();

        // --> On LCD (Normal)
          // --> Suhu Udara
            myGLCD.print("Suhu.U",LEFT,8);
            myGLCD.print("=",36,8);
            myGLCD.printNumI((float)DHT.temperature-3, 48,8);
            myGLCD.print("*C", 66,8);

          // --> Suhu Air
            myGLCD.print("Suhu.A",0,16);
            myGLCD.print("=",36,16);
            myGLCD.printNumI((float)Temperature, 48,16);
            myGLCD.print("*C", 66,16);
          
          // --> Kelembaban
            myGLCD.print("Lembab",0,0);
            myGLCD.print("=",36,0);
            myGLCD.printNumI((float)DHT.humidity, 48,0);
            myGLCD.print("%", 66,0);

          // --> Lux
            myGLCD.print("Cahaya",LEFT,32);
            myGLCD.print("=",36,32);
            myGLCD.printNumI((float)lux,lux_decimal,32);
            myGLCD.print("lx", 66,32);

          // --> Kapasitas Air
            myGLCD.print("Tanki",LEFT,40);
            myGLCD.print("=",36,40);
            myGLCD.printNumI(kapasitas,kapasitas_decimal, 40);
            myGLCD.print("%", 68, 40);

          // --> Nutrisi
            myGLCD.print("N.Air",0,24);
            myGLCD.print("=",36,24);
            myGLCD.printNumI(ppm, 42,24);
            myGLCD.print("ppm", 66,24);

          // --> Spaeker Buzzer
            if (distance >= 20)
              {
                  delay(200);
                tone(buzz, 15000, 200);
                  delay(400);
                tone(buzz, 15000, 200);
                  delay(400);    
                // 1000 ms
                tone(buzz, 15000, 200);
                  delay(400);
                tone(buzz, 15000, 200);
                  delay(400);
                tone(buzz, 15000, 200);
                // 2000 ms
                  delay(400);
                tone(buzz, 15000, 200);
                  delay(400);
                tone(buzz, 15000, 200);
                  delay(400);  
                // 3000 ms
                tone(buzz, 15000, 200);
                  delay(400);
                tone(buzz, 15000, 200);
                  delay(400);
                tone(buzz, 15000, 200);
                // 4000 ms
                  delay(400);
                tone(buzz, 15000, 200);
                  delay(400);
                tone(buzz, 14000, 200);
                  delay(400);  
                // 5000 ms    
              }

            else if (distance < 20)
              {
                  delay(100);
                tone(buzz, 14000, 300);
                  delay(500);
                tone(buzz, 14000, 300);
                  delay(4900);
              }
      }

// --> Delay
  //delay(5000);

}
