#include <SoftwareSerial.h>
#include<String.h>

#define TX_PIN 8
#define RX_PIN 7

#define TIME_OUT 20

SoftwareSerial SIM900(RX_PIN, TX_PIN); // configure software serial port
unsigned long int hoursOn = 1;
unsigned long int minsOn = 0;
unsigned long int secondsOn = 0;
unsigned long int timeOn = (hoursOn*60*60 + minsOn*60 + secondsOn)*1000;
bool turnOn;
unsigned long start;
String prefix = "+39";
String number = "3476855385";

void setup()
{
  pinMode(13, OUTPUT);
  pinMode(9, OUTPUT);
  digitalWrite(13, LOW);
  SIM900.begin(9600);
  Serial.begin(9600);
  Serial.print("Start");
  SIM900power();   // power off GSM shield
  turnOn = false;
}
 
void SIM900power()
// software equivalent of pressing the GSM shield "power" button
{
  digitalWrite(9, LOW);
  delay(1000);
  digitalWrite(9, HIGH);
  delay(10000);
  SIM900.print("AT+CLIP=1\r"); // turn on caller ID notification
}
 
 
void loop()
{
  
  SearchCallIncoming();
  delay(500);
  ControlTurnOff();
}

void SearchCallIncoming()
{
	String dataIn;
	if(SIM900.available() >0)
	{
		delay(3000);
		dataIn = SIM900.readString();
		Serial.println(dataIn);
		if(dataIn.indexOf(number)!=-1)
		{
			if(dataIn.indexOf("RING")!=-1)
			{
				digitalWrite(13, HIGH);
				SIM900.println("ATH");   // hang up
				if(turnOn)
				{
					call(1);
					digitalWrite(13, LOW);
					turnOn = false;
				}
				else
				{
					call(2);
					digitalWrite(13, HIGH);
					turnOn = true;
					start = millis();					
				}
			  
			}
		}
	} 
}

void call(int timesCall)
{
	if(timesCall < 1)
		return;
	String dataIn;
	int numCall = 0;
	int timeOut = 0;
	while(1)
	{
		Serial.print("Call " + String(numCall) + " " + prefix + number);
		SIM900.println("ATD" + prefix + number);
		delay(100);
		while(!SIM900.available());
		delay(1000);
		dataIn = SIM900.readString();
		Serial.print(dataIn);
		if(dataIn.indexOf("OK")!=-1)
		{
			delay(5000);            // wait for 30 seconds...
			SIM900.println("ATH");   // hang up
			Serial.print("Call " + String(numCall) + " Done");
			numCall = numCall + 1;
			if(numCall == timesCall)
			{
				Serial.println(String(timesCall) + "Call Done");
				return;
			}
		}
		else
		{
			timeOut = timeOut + 1;
			if(timeOut == TIME_OUT)
			{
				Serial.println("TimeOut Number of Call");
				return;
			}
		}
		delay(5000);
		
	}
}


void ControlTurnOff()
{
	unsigned long int now = millis();
	if(turnOn)
	{
		if((now - start)>timeOn)
		{
			digitalWrite(13, LOW);
			call(2);
			Serial.println("Turn OFF end of time on");
		}
	}
}

