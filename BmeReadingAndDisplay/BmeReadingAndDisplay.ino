#include "seeed_bme680.h"
#include <Wire.h>
#include "rgb_lcd.h"
#include <SoftwareSerial.h> 

#define RxD         6
#define TxD         7

SoftwareSerial blueToothSerial(RxD,TxD);

int pin = 4;
unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 3000;//sampe 30s ;
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;

rgb_lcd lcd;

const int colorR = 0;
const int colorG = 0;
const int colorB = 100;




#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10


#define IIC_ADDR  uint8_t(0x76)


/**  NOTE!!!!!!!!!!!!  Select the communication protocol correctly **/

Seeed_BME680 bme680(IIC_ADDR); /* IIC PROTOCOL */
//Seeed_BME680 bme680;             /* SPI PROTOCOL */
//Seeed_BME680 bme680(BME_CS, BME_MOSI, BME_MISO,  BME_SCK);/*SPI PROTOCOL*/

void setup() {
    Serial.begin(9600);
    pinMode(RxD, INPUT);
    pinMode(TxD, OUTPUT);
    
    setupBlueToothConnection();
    //wait 1s and flush the serial buffer
    delay(1000);
    Serial.flush();
    blueToothSerial.flush();
    lcd.begin(16, 2);
    lcd.setRGB(colorR, colorG, colorB);
    starttime = millis();


    
    while (!Serial);
    lcd.println("Reading Values:)");
    delay(100);
    while (!bme680.init()) {
        Serial.println("bme680 init failed ! can't find device!");
        delay(10000);
    }
}

void setupBlueToothConnection()
{

	
    blueToothSerial.begin(9600);  
	
	blueToothSerial.print("AT");
	delay(400); 
	
	blueToothSerial.print("AT+DEFAULT");             // Restore all setup value to factory setup
	delay(2000); 
	
	blueToothSerial.print("AT+NAMESeeedMaster");    // set the bluetooth name as "SeeedMaster" ,the length of bluetooth name must less than 12 characters.
	delay(400);
	
	blueToothSerial.print("AT+ROLEM");             // set the bluetooth work in slave mode
	delay(400); 
	
	
	blueToothSerial.print("AT+AUTH1");            
    delay(400);    
	
	blueToothSerial.print("AT+CLEAR");             // Clear connected device mac address
    delay(400);   
	
    blueToothSerial.flush();
	
	
}

void loop() {
    if (bme680.read_sensor_data()) {
        lcd.println("Failed reading :(");
        return;
    }

    lcd.noBlink();
    delay(2000);
    lcd.setCursor(0, 0);
    lcd.print("Temp ==>>");
    lcd.print(bme680.sensor_result_value.temperature);
    lcd.println(" C");

    lcd.setCursor(0, 1);
    lcd.print("Pres=>");
    lcd.print(bme680.sensor_result_value.pressure / 1000.0);
    lcd.println(" KPa");

    lcd.blink();
    delay(2000);  
    lcd.setCursor(0, 0);
    lcd.print("Humid => ");
    lcd.print(bme680.sensor_result_value.humidity);
    lcd.println(" %");

    blueToothSerial.print(bme680.sensor_result_value.humidity > 35 ? 1 : 0);
    Serial.print(bme680.sensor_result_value.humidity > 35 ? 1 : 0);

    lcd.setCursor(0, 1);
    lcd.print("Gas => ");
    lcd.print(bme680.sensor_result_value.gas / 1000.0);
    lcd.println("Kohms"); 

    lcd.blink();
    delay(2000);

      duration = pulseIn(pin, LOW);
    lowpulseoccupancy = lowpulseoccupancy+duration;
 
    if ((millis()-starttime) > sampletime_ms)//if the sampel time == 30s
    {
        ratio = lowpulseoccupancy/(sampletime_ms*10.0);  // Integer percentage 0=>100
        concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve
        lcd.setCursor(0, 0);
        lcd.print("Dust Level");
        lcd.setCursor(0, 1);
        lcd.print(lowpulseoccupancy);
        lcd.print(",");
        lcd.print(ratio);
        lcd.print(",");
        lcd.println(concentration);
        lowpulseoccupancy = 0;
        starttime = millis();
    }

    delay(2000);
}

