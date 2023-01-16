#include "seeed_bme680.h"
#include <Wire.h>
#include "rgb_lcd.h"
rgb_lcd lcd;

const int colorR = 100;
const int colorG = 0;
const int colorB = 0;



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
    lcd.begin(16, 2);
    lcd.setRGB(colorR, colorG, colorB);


    
    while (!Serial);
    Serial.println("Serial start!!!");
    delay(100);
    while (!bme680.init()) {
        Serial.println("bme680 init failed ! can't find device!");
        delay(10000);
    }
}

void loop() {
    if (bme680.read_sensor_data()) {
        Serial.println("Failed reading :(");
        return;
    }
    lcd.setCursor(0, 0);
    lcd.print("Temp ==>>");
    lcd.print(bme680.sensor_result_value.temperature);
    lcd.println(" C");

    lcd.setCursor(0, 1);
    lcd.print("Pres=>");
    lcd.print(bme680.sensor_result_value.pressure / 1000.0);
    lcd.println(" KPa");

    //lcd.setCursor(0, 2);  
    //lcd.print("humidity => ");
    //lcd.print(bme680.sensor_result_value.humidity);
    //cd.println(" %");

    //lcd.print("gas => ");
   // lcd.print(bme680.sensor_result_value.gas / 1000.0);
   // lcd.println(" Kohms"); 

    

    delay(2000);
}

