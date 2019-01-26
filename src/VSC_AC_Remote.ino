/*
 * Project VSC_AC_Remote
 * Description: This version of the code had been build on Visual Studio Code so that it can be uploaded and stored in GitHub
 * Author: Micheal C Brewer Jr
 * Date: 01/26/2019
 */

// This #include statement was automatically added by the Particle IDE.
#include <Particle_SI7021.h>
#include <ITEADLIB_Nextion.h>
#include <VCNL4010.h>
//#include <mqtt.h>
#include <math.h>

SI7021 sensor;

//MainPage Page Setup
NexProgressBar j100  = NexProgressBar(0, 8, "j100");
NexText t0 = NexText(0, 1, "t0");
NexText t2 = NexText(0, 9, "t2");
NexNumber n0 = NexNumber(0, 6, "n0");
NexNumber n1 = NexNumber(0, 7, "n1");
NexButton b0 = NexButton(0, 4, "b1");
NexButton b1 = NexButton(0, 5, "b0");

//AC Page Setup
//NexProgressBar j100  = NexProgressBar(0, 3, "j100");
//NexText t0 = NexText(1, 3, "t0");
NexNumber n100 = NexNumber(1, 7, "n100");
NexNumber n101 = NexNumber(1, 8, "n101");
NexButton b100 = NexButton(1, 6, "b100");
NexButton b101 = NexButton(1, 1, "b101");
NexButton b102 = NexButton(1, 2, "b102");

//Setting Page Setup
//NexProgressBar j0  = NexProgressBar(0, 3, "j0");
//NexText t0 = NexText(2, 4, "t0");
NexButton b200 = NexButton(2, 3, "b200");
NexButton b201 = NexButton(2, 1, "b201");
NexButton b202 = NexButton(2, 2, "b202");
NexButton b203 = NexButton(2, 5, "b203");

//Display Page Setup
//NexProgressBar j0  = NexProgressBar(0, 3, "j0");
//NexText t0 = NexText(3, 7, "t0");
NexNumber n300 = NexNumber(3, 3, "n300");
NexText t300 = NexText(3, 4, "t300");
NexButton b300 = NexButton(3, 2, "b300");
NexButton b301 = NexButton(3, 5, "b301");
NexButton b302 = NexButton(3, 2, "b302");

//DateTime Page Setup
//NexProgressBar j0  = NexProgressBar(0, 3, "j0");
//NexText t0 = NexText(4, 3, "t0");
NexNumber n400 = NexNumber(4, 2, "n400");
NexButton b400 = NexButton(4, 1, "b400");

USARTSerial & nexSerial = Serial1;

/*******************************************************************************
 IO mapping
*******************************************************************************/
// A0 : photoresistor
// A5 : power
int photoresistor = A0; // This is where your photoresistor is plugged in. The other side goes to the "power" pin (below).
int power = A5;
int lightlevel;

// The following values are the read values of the photoresistor
int Closedvalue=5; // This is the average value that the photoresistor reads while in the fridge.
int Openvalue=260; // This is the average value that the photoresistor reads when exposed to average lighting.
int Cutoff=((Closedvalue+Openvalue)/2);

String dspSSID = "None";
int temperature = 0;
int humidity = 0;

void setup()   {
    
  
  Serial.begin(115200);
  Serial1.print("baud=115200");
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.begin(115200);
  Serial.println("Si7021 test");
  Time.zone(-5);

  nexInit();
  sensor.begin();

NexTouch *nex_listen_list[] = {
  &b203,
  NULL
};


// photoresistor setup
  pinMode(photoresistor,INPUT);  // Our photoresistor pin is input (reading the photoresistor)
  pinMode(power,OUTPUT); // The pin powering the photoresistor is output (sending out consistent power)
  digitalWrite(power,HIGH);


  WiFi.setHostname("pcns-ac-remote");
  
  dspSSID = WiFi.SSID();
  
  Particle.variable("Office Temperature", &temperature, INT);
  Particle.variable("Office Humidity", &humidity, INT);
  Particle.variable("WiFi SSID", &dspSSID, STRING);


}



void loop() {


// temperature is an integer in hundredths
  float temperature = sensor.getFahrenheitHundredths();
  temperature = temperature / 100;


// humidity is an integer representing percent
  float humidity = sensor.getHumidityPercent();


  
  WiFiSignal sig = WiFi.RSSI();
  float strength = sig.getStrength();
  
 
  
  Particle.publish("WiFi SSID", String(dspSSID), 60, PRIVATE);
  Particle.publish("Office Temperature", String(temperature), 900,PRIVATE);
  Particle.publish("Office Humidity", String(humidity), 900, PRIVATE);
  Particle.publish("WiFi Strength", String(strength), 60, PRIVATE);
  Serial.print("Humi: "); Serial.println(humidity);
  Serial.print("Temp: "); Serial.println(temperature);

//Displaying on Nextion
  n0.setValue(temperature);
  n1.setValue(humidity);
  t0.setText(Time.format(Time.now(), TIME_FORMAT_DEFAULT));
  j100.setValue(strength);
  t2.setText(dspSSID);


}

/*******************************************************************************

 * Function Name  : getTime
 * Description    : returns the time in the following format: 14:42:31
                    TIME_FORMAT_ISO8601_FULL example: 2016-03-23T14:42:31-04:00
 * Return         : the time
 *******************************************************************************/

String getTime() {
  String timeNow = Time.format(Time.now(), TIME_FORMAT_DEFAULT);
  timeNow = timeNow.substring(11, timeNow.length()-6);
  return " " + timeNow;
}


