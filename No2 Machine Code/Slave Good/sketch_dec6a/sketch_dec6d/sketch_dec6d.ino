
#include <SoftwareSerial.h>   //Software Serial Port

#define RxD         6
#define TxD         7


#define DEBUG_ENABLED  1

SoftwareSerial blueToothSerial(RxD,TxD);


void setup()

{
    Serial.begin(9600);
    pinMode(RxD, INPUT);
    pinMode(TxD, OUTPUT);
   
    
    setupBlueToothConnection();
}

void loop()
{
    char recvChar;
    
    while(1)
    {
        if(blueToothSerial.available())
        {//check if there's any data sent from the remote bluetooth shield
            recvChar = blueToothSerial.read();
            
            
            if(recvChar == '1')
            {
              Serial.print("1");
              digitalWrite(3, HIGH);
               
            }
            else if(recvChar == '0')
            {
              Serial.print("0");
              digitalWrite(3, LOW);
          
            }
        }
    }
}




/***************************************************************************
 * Function Name: setupBlueToothConnection
 * Description:  initilizing bluetooth connction
 * Parameters: 
 * Return: 
***************************************************************************/
void setupBlueToothConnection()
{	

	
	
	blueToothSerial.begin(9600);  
	
	blueToothSerial.print("AT");
	delay(400); 

	blueToothSerial.print("AT+DEFAULT");             // Restore all setup value to factory setup
	delay(2000); 
	
	blueToothSerial.print("AT+NAMESeeedBTSlave");    // set the bluetooth name as "SeeedBTSlave" ,the length of bluetooth name must less than 12 characters.
	delay(400);
	
    blueToothSerial.print("AT+PIN0000");             // set the pair code to connect 
	delay(400);
	
	blueToothSerial.print("AT+AUTH1");             //
    delay(400);    

    blueToothSerial.flush();

}
