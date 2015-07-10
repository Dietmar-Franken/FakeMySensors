/*
* FakeMySensors
*
* This skecth is intended to crate fake sensors which register and respond to the controller
* 
* Barduino 2015
*/

#include <SPI.h>
#include <MySensor.h>  

// SPI Pins
#define CE_PIN 9
#define CS_PIN 10

// Wait times
#define LONG_WAIT 500
#define SHORT_WAIT 50

// Define Sensors ids
/*      S_DOOR, S_MOTION, S_SMOKE, S_LIGHT, S_DIMMER, S_COVER, S_TEMP, S_HUM, S_BARO, S_WIND,
	S_RAIN, S_UV, S_WEIGHT, S_POWER, S_HEATER, S_DISTANCE, S_LIGHT_LEVEL, S_ARDUINO_NODE,
	S_ARDUINO_REPEATER_NODE, S_LOCK, S_IR, S_WATER, S_AIR_QUALITY, S_CUSTOM, S_DUST,
	S_SCENE_CONTROLLER
*/
#define ID_S_DOOR                     0
#define ID_S_MOTION                   1
#define ID_S_SMOKE                    2
#define ID_S_LIGHT                    3
#define ID_S_DIMMER                   4
#define ID_S_COVER                    5
#define ID_S_TEMP                     6
#define ID_S_HUM                      7
#define ID_S_BARO                     8
#define ID_S_WIND                     9
#define ID_S_RAIN                    10
#define ID_S_UV                      11
#define ID_S_WEIGHT                  12 
//#define ID_S_POWER                   13
//#define ID_S_HEATER                  14
//#define ID_S_DISTANCE                15
//#define ID_S_LIGHT_LEVEL             16 
////#define ID_S_ARDUINO_NODE            17
////#define ID_S_ARDUINO_REPEATER_NODE   18 
//#define ID_S_LOCK                    19
//#define ID_S_IR                      20
//#define ID_S_WATER                   21 
////#define ID_S_AIR_QUALITY             22 
//#define ID_S_CUSTOM                  23
//#define ID_S_DUST                    24
//#define ID_S_SCENE_CONTROLLER        25

// Global Vars
unsigned long SLEEP_TIME = 60000; // Sleep time between reads (in milliseconds)
boolean metric = true;
long randNumber;
//bool g_trigger;

// Instanciate MySersors Gateway
MySensor gw(CE_PIN,CS_PIN);

//Instanciate Messages objects
//MyMessage msgHum(ID_ID_HUM, V_HUM);
//MyMessage msgTemp(ID_ID_TEMP, V_TEMP);

MyMessage msg_S_TEMP(ID_S_TEMP,V_TEMP);
MyMessage msg_S_HUM(ID_S_HUM,V_HUM);
MyMessage msg_S_LIGHT(ID_S_LIGHT,V_LIGHT);
MyMessage msg_S_DIMMER(ID_S_DIMMER,V_DIMMER);
MyMessage msg_S_BARO_P(ID_S_BARO,V_PRESSURE);
MyMessage msg_S_BARO_F(ID_S_BARO,V_FORECAST);
MyMessage msg_S_RAIN_A(ID_S_RAIN,V_RAIN);
MyMessage msg_S_RAIN_R(ID_S_RAIN,V_RAINRATE);

MyMessage msg_S_WIND_S(ID_S_WIND,V_WIND);
MyMessage msg_S_WIND_G(ID_S_WIND,V_GUST);
MyMessage msg_S_WIND_D(ID_S_WIND,V_DIRECTION);

MyMessage msg_S_UV(ID_S_UV,V_UV);
MyMessage msg_S_WEIGHT(ID_S_WEIGHT,V_WEIGHT);
//MyMessage msg_S_DISTANCE(ID_S_DISTANCE,V_DISTANCE);
//MyMessage msg_S_POWER(ID_S_POWER,V_IMPEDANCE);
MyMessage msg_S_MOTION_A(ID_S_MOTION,V_ARMED);
MyMessage msg_S_MOTION_T(ID_S_MOTION,V_TRIPPED);
MyMessage msg_S_SMOKE(ID_S_SMOKE,V_TRIPPED);
//MyMessage msg_S_DOOR(ID_S_DOOR,V_TRIPPED);
//MyMessage msg_S_POWER_W(ID_S_POWER,V_WATT);
//MyMessage msg_S_POWER_K(ID_S_POWER,V_KWH);
//MyMessage msg_S_SCENE_CONTROLLER_N(ID_S_SCENE_CONTROLLER,V_SCENE_ON);
//MyMessage msg_S_SCENE_CONTROLLER_F(ID_S_SCENE_CONTROLLER,V_SCENE_OFF);
//MyMessage msg_S_HEATER(ID_S_HEATER,V_HEATER);
//MyMessage msg_S_HEATER_S(ID_S_HEATER,V_HEATER_SW);
//MyMessage msg_S_LIGHT_LEVEL(ID_S_LIGHT_LEVEL,V_LIGHT_LEVEL);
//MyMessage msg_S_CUSTOM_1(ID_S_CUSTOM,V_VAR1);
//MyMessage msg_S_CUSTOM_2(ID_S_CUSTOM,V_VAR2);
//MyMessage msg_S_CUSTOM_3(ID_S_CUSTOM,V_VAR3);
//MyMessage msg_S_CUSTOM_4(ID_S_CUSTOM,V_VAR4);
//MyMessage msg_S_CUSTOM_5(ID_S_CUSTOM,V_VAR5);
MyMessage msg_S_COVER_U(ID_S_COVER,V_UP);
MyMessage msg_S_COVER_D(ID_S_COVER,V_DOWN);
MyMessage msg_S_COVER_S(ID_S_COVER,V_STOP);
//MyMessage msg_S_IR_S(ID_S_IR,V_IR_SEND);
//MyMessage msg_S_IR_R(ID_S_IR,V_IR_RECEIVE);
//MyMessage msg_S_WATER_F(ID_S_WATER,V_FLOW);
//MyMessage msg_S_WATER_V(ID_S_WATER,V_VOLUME);
//MyMessage msg_S_LOCK(ID_S_LOCK,V_LOCK_STATUS);
//MyMessage msg_S_DUST(ID_S_DUST,V_DUST_LEVEL);
//MyMessage msg_S_POWER_V(ID_S_POWER,V_VOLTAGE);
//MyMessage msg_S_POWER_C(ID_S_POWER,V_CURRENT);



void setup()  
{ 
  // Random SEED
  randomSeed(analogRead(0));
  
  // Start the gateway
  Serial.println("Begin GW");
  gw.begin(incomingMessage,254);
  gw.wait(LONG_WAIT);
  
  // Send the Sketch Version Information to the Gateway
  Serial.println("Send Sketch Info");
  gw.sendSketchInfo("FakeMySensors", "0.1");
  gw.wait(LONG_WAIT);
  
  // Get controller configuration
  Serial.println("Get Config");
  metric = gw.getConfig().isMetric;
  gw.wait(LONG_WAIT);
  
  // Register all sensors to gw (they will be created as child devices)
  Serial.println("Presenting Nodes");

  gw.present(ID_S_DOOR,S_DOOR);
  gw.wait(SHORT_WAIT);
  
  gw.present(ID_S_MOTION,S_MOTION);
  gw.wait(SHORT_WAIT);
  
  gw.present(ID_S_SMOKE,S_SMOKE);
  gw.wait(SHORT_WAIT);
  
  gw.present(ID_S_LIGHT,S_LIGHT);
  gw.wait(SHORT_WAIT);
  
  gw.present(ID_S_DIMMER,S_DIMMER);
  gw.wait(SHORT_WAIT);
  
  gw.present(ID_S_COVER,S_COVER);
  gw.wait(SHORT_WAIT);
  
  gw.present(ID_S_TEMP,S_TEMP);
  gw.wait(SHORT_WAIT);
  
  gw.present(ID_S_HUM,S_HUM);
  gw.wait(SHORT_WAIT);
  
  gw.present(ID_S_BARO,S_BARO);
  gw.wait(SHORT_WAIT);
  
  gw.present(ID_S_WIND,S_WIND);
  gw.wait(SHORT_WAIT);
  
  gw.present(ID_S_RAIN,S_RAIN);
  gw.wait(SHORT_WAIT);
  
  gw.present(ID_S_UV,S_UV);
  gw.wait(SHORT_WAIT);
  
  gw.present(ID_S_WEIGHT,S_WEIGHT);
  gw.wait(SHORT_WAIT);
  
//  gw.present(ID_S_POWER,S_POWER);
//  gw.wait(SHORT_WAIT);
//  
//  gw.present(ID_S_HEATER,S_HEATER);
//  gw.wait(SHORT_WAIT);
//  
//  gw.present(ID_S_DISTANCE,S_DISTANCE);
//  gw.wait(SHORT_WAIT);
//  
//  gw.present(ID_S_LIGHT_LEVEL,S_LIGHT_LEVEL);
//  gw.wait(SHORT_WAIT);
//  
//  gw.present(ID_S_LOCK,S_LOCK);
//  gw.wait(SHORT_WAIT);
//  
//  gw.present(ID_S_IR,S_IR);
//  gw.wait(SHORT_WAIT);
//  
//  gw.present(ID_S_WATER,S_WATER);
//  gw.wait(SHORT_WAIT);
//  
//  gw.present(ID_S_CUSTOM,S_CUSTOM);
//  gw.wait(SHORT_WAIT);
//  
//  gw.present(ID_S_DUST,S_DUST);
//  gw.wait(SHORT_WAIT);
//  
//  gw.present(ID_S_SCENE_CONTROLLER,S_SCENE_CONTROLLER);
//  gw.wait(SHORT_WAIT);

}

void loop()      
{ 
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("#########################"); 
  randNumber=random(0,101);
  
  Serial.print("RandomNumber:");
  Serial.println(randNumber);
  // Send fake battery level
  Serial.println("Send Battery Level");
  gw.sendBatteryLevel(randNumber);
  gw.wait(LONG_WAIT);
  
  // Request time
  Serial.println("Request Time");
  gw.requestTime(receiveTime);
  gw.wait(LONG_WAIT);
  
  //Read Sensors
  door();
  motion();
  smoke();
  light();
  dimmer();
  cover();
  temp();
  hum();
  baro();
  wind();
  rain();
  uv();
  weight();
  
  gw.sendBatteryLevel(randNumber);
  gw.wait(SHORT_WAIT);
  gw.process();
  Serial.println("#########################");
  gw.sleep(SLEEP_TIME); //sleep a bit
}

// This is called when a new time value was received
void receiveTime(unsigned long controllerTime) {

  Serial.print("Time value received: ");
  Serial.println(controllerTime);

}

//void door(){}

void door(){

  Serial.print("Door is: " );
  
  if (randNumber <= 50) {
    Serial.println("Open");
    gw.send(msg_S_TEMP.set(1));
  } else {
    Serial.println("Closed");
    gw.send(msg_S_TEMP.set(0));
  }
  
}

void motion(){

  Serial.print("Motion is: " );
  
  if (randNumber <= 50) {
    Serial.println("Active");
    gw.send(msg_S_MOTION_T.set(1));
  } else {
    Serial.println("Quiet");
    gw.send(msg_S_MOTION_T.set(0));
  }
  
}

void smoke(){

  Serial.print("Smoke is: " );
  
  if (randNumber <= 50) {
    Serial.println("Active");
    gw.send(msg_S_SMOKE.set(1));
  } else {
    Serial.println("Quiet");
    gw.send(msg_S_SMOKE.set(0));
  }
  
}

void light(){

  bool isLightOn = gw.loadState(ID_S_LIGHT);
  
  Serial.print("Light is: " );
  
  if (isLightOn) {
    Serial.println("On");
    gw.send(msg_S_LIGHT.set(1));
  } else {
    Serial.println("Off");
    gw.send(msg_S_LIGHT.set(0));
  }
  
}

void dimmer(){

  int dimmerVal = gw.loadState(ID_S_DIMMER);
  
  Serial.print("Dimmer is set to: " );
  Serial.println(dimmerVal);
  
  gw.send(msg_S_DIMMER.set(dimmerVal));

}

void cover(){

  int coverVal = gw.loadState(ID_S_COVER);
  
  Serial.print("Cover is : " );
  
  switch (coverVal) {
    case 0:
      Serial.println("Opening");
      gw.send(msg_S_COVER_U.set(V_UP));
    break;
    case 1:
      Serial.println("Closing");
      gw.send(msg_S_COVER_D.set(V_DOWN));
    break;
    default:
      Serial.println("Idle");
      gw.send(msg_S_COVER_S.set(V_STOP));
  }
  
}

void temp(){
  
  Serial.print("Temperature is: " );
  Serial.println(map(randNumber,1,100,0,45));
  
  gw.send(msg_S_TEMP.set(map(randNumber,1,100,0,45)));
  
}

void hum(){
  
  Serial.print("Humitidty is: " );
  Serial.println(randNumber);
  
  gw.send(msg_S_HUM.set(randNumber));
  
}

void baro(){
  
  const char *weather[] = {"stable","sunny","cloudy","unstable","thunderstorm","unknown"};
  long pressure = map(randNumber,1,100,87000,108600);
  int forecast = map(randNumber,1,100,0,5);
  
  Serial.print("Atmosferic Pressure is: " );
  Serial.println(pressure);
  gw.send(msg_S_BARO_P.set(pressure));
  
  Serial.print("Weather forecast: " );
  Serial.println(weather[forecast]);
  gw.send(msg_S_BARO_F.set(weather[forecast]));
 
}

void wind(){
  
  Serial.print("Wind Speed is: " );
  Serial.println(randNumber);
  gw.send(msg_S_WIND_S.set(randNumber));
  
  Serial.print("Wind Gust is: " );
  Serial.println(randNumber+10);
  gw.send(msg_S_WIND_G.set(randNumber+10));
  
  Serial.print("Wind Direction is: " );
  Serial.println(map(randNumber,1,100,0,360));
  gw.send(msg_S_WIND_D.set(map(randNumber,1,100,0,360)));
  
}

void rain(){
  
  Serial.print("Rain ammount  is: " );
  Serial.println(randNumber);
  
  gw.send(msg_S_RAIN_A.set(randNumber));
  
  Serial.print("Rain rate  is: " );
  Serial.println(randNumber/60);
  
  gw.send(msg_S_RAIN_R.set(randNumber/60,1));

}

void uv(){
  
  Serial.print("Ultra Violet level is: " );
  Serial.println(map(randNumber,1,100,0,15));
  
  gw.send(msg_S_UV.set(map(randNumber,1,100,0,15)));

}

void weight(){
  
  Serial.print("Weight is: " );
  Serial.println(map(randNumber,1,100,0,150));
  
  gw.send(msg_S_WEIGHT.set(map(randNumber,1,100,0,150)));

}


void incomingMessage(const MyMessage &message) {
  switch (message.type) {
    case V_LIGHT:
          gw.saveState(ID_S_LIGHT, message.getBool());
          Serial.print("Incoming change for ID_S_LIGHT:");
          Serial.print(message.sensor);
          Serial.print(", New status: ");
          Serial.println(message.getBool());
    break;
    
    case V_DIMMER:
          if ((message.getInt()<0)||(message.getInt()>100)) {
            Serial.println( "V_DIMMER data invalid (should be 0..100)" );
            break;
          }
          gw.saveState(ID_S_DIMMER, message.getInt());
          Serial.print("Incoming change for ID_S_DIMMER:");
          Serial.print(message.sensor);
          Serial.print(", New status: ");
          Serial.println(message.getInt());
    break;
        
    case V_UP:
          gw.saveState(ID_S_COVER, 0);
          Serial.print("Incoming change for ID_S_COVER:");
          Serial.print(message.sensor);
          Serial.print(", New status: ");
          Serial.println("V_UP");
    break;
    
    case V_DOWN:
          gw.saveState(ID_S_COVER, 1);
          Serial.print("Incoming change for ID_S_COVER:");
          Serial.print(message.sensor);
          Serial.print(", New status: ");
          Serial.println("V_DOWN");
    break;
    
    case V_STOP:
          gw.saveState(ID_S_COVER, 2);
          Serial.print("Incoming change for ID_S_COVER:");
          Serial.print(message.sensor);
          Serial.print(", New status: ");
          Serial.println("V_STOP");
    break;

    default: 
      Serial.print("Unknown/UnImplemented message type:");
      Serial.println(message.type);
  }

}





