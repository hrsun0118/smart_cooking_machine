/*  Arduino TFT Tutorial
 *  Program made by Dejan Nedelkovski,
 *  www.HowToMechatronics.com
 */
 
/*  This program uses the UTFT and URTouch libraries
 *  made by Henning Karlsen. 
 *  You can find and download them at:
 *  www.RinkyDinkElectronics.com
 */


//==== For Motor
#include <Stepper.h>

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
const int VCC = 13;

Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);
int stepCount = 0;  // number of steps the motor has taken
// Note: xM is used as "analog sensor reading data"

//==== For TFT Touch Screen
#include <UTFT.h> 
#include <URTouch.h>

//==== Creating Objects
UTFT    myGLCD(ILI9341_16,38,39,40,41); //Parameters should be adjusted to your Display/Schield model
URTouch  myTouch(6, 5, 4, 3, 2);

//==== Defining Variables
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];

int x, y;

char currentPage, selectedUnit;

//Ultrasonic Sensor
const int trigPin = 11;
const int echoPin = 12;

long duration;
int distanceInch, distanceCm;

// RGB LEDs
const int redLed = 10;
const int greenLed = 9;
const int blueLed = 8;
int xW = 163;
int xO=163;
int xM=163;
int xT=163;

// Water/Oil On/Off buttons, ON/OFF pins & string literals
bool waterButton = false;
bool oilButton = false;
const int waterPin = 14;
const int oilPin = 15;
char on_str[3] = "ON";
char off_str[4] = "OFF";

// Temperature setups
#define RT0 10000   // Ω
#define B 3977      // K
#define Vsupply 5    //Supply voltage
#define R 10000  //R=10KΩ

float RT, VR, ln, T0, VRT;
const int tempPin = A0;
float tempReading = 0.0;
bool overheat_sign = false;


void setup() {
// Initial setup
  
  // FOR TFT TOUCH SCREEN
  myGLCD.InitLCD();
  myGLCD.clrScr();
  myTouch.InitTouch();
  myTouch.setPrecision(PREC_MEDIUM);

  // Defining Pin Modes
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(blueLed, OUTPUT);
  
  // Define Pin Modes for Water, oil
  pinMode(waterPin, OUTPUT);
  pinMode(oilPin, OUTPUT);

  // FOR MOTOR
  pinMode(VCC, OUTPUT); // VCC
  digitalWrite(VCC, HIGH);

  // For themistor
  Serial.begin(9600);
  T0 = 25 + 273.15;                 //Temperature T0 from datasheet, conversion from Celsius to kelvin
  pinMode(tempPin, INPUT);

  drawHomeScreen();  // Draws the Home Screen
  currentPage = '0'; // Indicates that we are at Home Screen
  selectedUnit = '0'; // Indicates the selected unit for the first example, cms or inches
}

void loop() {
  // FOR MOTOR
  int motorSpeed = map(xM, 163, 310, 0, 100);
  // set the motor speed:
  if (motorSpeed > 0) {
    myStepper.setSpeed(motorSpeed);
    // step 1/100 of a revolution:
    myStepper.step(stepsPerRevolution / 100);
  } 
  
  // FOR TFT Touchscreen
  // Home Screen
  if (currentPage == '0') {
    if (myTouch.dataAvailable()) {
      myTouch.read();
      x=myTouch.getX(); // X coordinate where the screen has been pressed
      y=myTouch.getY(); // Y coordinates where the screen has been pressed
      // If we press the Distance Sensor Button 
      if ((x>=35) && (x<=285) && (y>=90) && (y<=130)) {
        drawFrame(35, 90, 285, 130); // Custom Function -Highlighs the buttons when it's pressed
        currentPage = '1'; // Indicates that we are the first example
        myGLCD.clrScr(); // Clears the screen
        drawDistanceSensor(); // It is called only once, because in the next iteration of the loop, this above if statement will be false so this funtion won't be called. This function will draw the graphics of the first example.
      }
      // If we press the RGB LED Control Button 
      if ((x>=35) && (x<=285) && (y>=140) && (y<=180)) {
        drawFrame(35, 140, 285, 180);
        currentPage = '2';
        myGLCD.clrScr();
        drawLedControl();
      }  
    }
  }
  // Distance Sensor Example
  if (currentPage == '1') {    
      getDistance(); // Gets distance from the sensor and this function is repeatedly called while we are at the first example in order to print the lasest results from the distance sensor
      if (myTouch.dataAvailable()) {
        myTouch.read();
        x=myTouch.getX();
        y=myTouch.getY();
       
        // If we press the Centimeters Button
        if ((x>=10) && (x<=135) &&(y>=90) && (y<=163)) {
          selectedUnit = '0';
        }
        // If we press the Inches Button
        if ((x>=10) && (x<=135) &&(y>=173) && (y<=201)) {
          selectedUnit = '1';
        }
        // If we press the Back Button
        if ((x>=10) && (x<=60) &&(y>=10) && (y<=36)) {
          drawFrame(10, 10, 60, 36);
          currentPage = '0'; // Indicates we are at home screen
          myGLCD.clrScr();
          drawHomeScreen(); // Draws the home screen
        }
      }
  }
  
  // RGB LED Control - Motor Control
  if (currentPage == '2') {
    setLedColor();
    updateTemp();
    if (myTouch.dataAvailable()) {
        myTouch.read();
        x=myTouch.getX();
        y=myTouch.getY();
        
        //Back button
        if ((x>=10) && (x<=60) &&(y>=10) && (y<=36)) {
          drawFrame(10, 10, 60, 36);
          currentPage = '0';
          myGLCD.clrScr();
          drawHomeScreen();
//          // Turns the LED off
//          analogWrite(redLed, 0);
//          analogWrite(greenLed, 0);
//          analogWrite(blueLed, 0);
        }
    }
  }
}

// ====== Custom Funtions ======
// drawHomeScreen - Custom Function
void drawHomeScreen() {
  // Title
  myGLCD.setBackColor(0,0,0); // Sets the background color of the area where the text will be printed to black
  myGLCD.setColor(255, 255, 255); // Sets color to white
  myGLCD.setFont(BigFont); // Sets font to big
  char TechChef_str[16] = "TechChef Screen";
  myGLCD.print(TechChef_str, CENTER, 10); // Prints the string on the screen
  myGLCD.setColor(255, 0, 0); // Sets color to red
  myGLCD.drawLine(0,32,319,32); // Draws the red line
  myGLCD.setColor(255, 255, 255); // Sets color to white
  myGLCD.setFont(SmallFont); // Sets the font to small
  char Home_str[10] = "Home Page";
  myGLCD.print(Home_str, CENTER, 41); // Prints the string
  myGLCD.setFont(BigFont);
  char Select_str[17] = "Select Functions";
  myGLCD.print(Select_str, CENTER, 64);
  
  // Button - Distance Sensor
  myGLCD.setColor(16, 167, 103); // Sets green color
  myGLCD.fillRoundRect (35, 90, 285, 130); // Draws filled rounded rectangle
  myGLCD.setColor(255, 255, 255); // Sets color to white
  myGLCD.drawRoundRect (35, 90, 285, 130); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
  myGLCD.setFont(BigFont); // Sets the font to big
  myGLCD.setBackColor(16, 167, 103); // Sets the background color of the area where the text will be printed to green, same as the button
  char camera_str[11] = "RPI Camera";
  myGLCD.print(camera_str, CENTER, 102); // Prints the string
  
  // Button - RGB LED Control
  myGLCD.setColor(16, 167, 103);
  myGLCD.fillRoundRect (35, 140, 285, 180);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (35, 140, 285, 180);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(16, 167, 103);
  char controls_str[9] = "Controls";
  myGLCD.print(controls_str, CENTER, 152);
}

// Highlights the button when pressed
void drawFrame(int x1, int y1, int x2, int y2) {
  myGLCD.setColor(255, 0, 0);
  myGLCD.drawRoundRect (x1, y1, x2, y2);
  while (myTouch.dataAvailable())
    myTouch.read();
    myGLCD.setColor(255, 255, 255);
    myGLCD.drawRoundRect (x1, y1, x2, y2);
}
//====================================================
void drawDistanceSensor() {
  myGLCD.setColor(100, 155, 203);
  myGLCD.fillRoundRect (10, 10, 60, 36);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (10, 10, 60, 36);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(100, 155, 203);
  char arrow_str[3] = "<-";
  myGLCD.print(arrow_str, 18, 15);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.setFont(SmallFont);
  char back_str[18] = "Back to Main Menu";
  myGLCD.print(back_str, 70, 18);
  myGLCD.setFont(BigFont);
  char ultrasonic_str[18] = "Ultrasonic Sensor";
  myGLCD.print(ultrasonic_str, CENTER, 50);
  char hc_sr04_str[8] = "HC-SR04";
  myGLCD.print(hc_sr04_str, CENTER, 76);
  myGLCD.setColor(255, 0, 0);
  myGLCD.drawLine(0,100,319,100);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.setColor(255, 255, 255);
  myGLCD.setFont(SmallFont);
  char select_str[12] = "Select Unit";
  myGLCD.print(select_str, 10, 114);
  myGLCD.setFont(BigFont);
  char distance_str[10] = "Distance:";
  myGLCD.print(distance_str, 130, 120);
  myGLCD.setColor(223, 77, 55);
  myGLCD.fillRoundRect (10, 135, 90, 163);
  myGLCD.setColor(225, 255, 255);
  myGLCD.drawRoundRect (10, 135, 90, 163);
  myGLCD.setBackColor(223, 77, 55);
  myGLCD.setColor(255, 255, 255);
  char cm_str[3] = "cm";
  myGLCD.print(cm_str, 33, 140);
  myGLCD.setColor(223, 77, 55);
  myGLCD.fillRoundRect (10, 173, 90, 201);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (10, 173, 90, 201);
  myGLCD.setBackColor(223, 77, 55);
  myGLCD.setColor(255, 255, 255);
  char inch_str[5] = "inch";
  myGLCD.print(inch_str, 17, 180);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.setFont(SmallFont);
  char site_str[38] = "Source code at: HowToMechatronics.com";
  myGLCD.print(site_str, CENTER, 220);  
}
//====================================================
//===== getDistance - Custom Function
void getDistance() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distanceCm= duration*0.034/2;
  distanceInch= distanceCm/2.53;
  // Prints the distance in centimeters
  if (selectedUnit == '0' && distanceCm <=400) {
    myGLCD.setFont(SevenSegNumFont);
    myGLCD.setColor(0, 255, 0);
    myGLCD.setBackColor(0, 0, 0);
    myGLCD.printNumI(distanceCm,130, 145, 3,'0');
    myGLCD.setFont(BigFont);
    char cm_str[5] = "cm  ";
    myGLCD.print(cm_str, 235, 178);
  
  }
  // Prints the distance in inches
  if (selectedUnit == '1' && distanceCm <=160) {
    myGLCD.setFont(SevenSegNumFont);
    myGLCD.setColor(0, 255, 0);
    myGLCD.setBackColor(0, 0, 0);
    myGLCD.printNumI(distanceInch,130, 145, 3,'0');
    myGLCD.setFont(BigFont);
    char inch_str[5] = "inch";
    myGLCD.print(inch_str, 235, 178);
  } 
  delay(10);
}
//====================================================
void drawLedControl() {
  myGLCD.setColor(100, 155, 203);
  myGLCD.fillRoundRect (10, 10, 60, 36);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (10, 10, 60, 36);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(100, 155, 203);
  char arrow_str[3] = "<-";
  myGLCD.print(arrow_str,18, 15);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.setFont(SmallFont);
  char back_str[5] = "Back";
  myGLCD.print(back_str, 70, 18);

  // draw alarm
  char alarm_str[15] = "Overheat Alarm";     // [Potential Modification] - Font
  myGLCD.print(alarm_str, 160, 18); 
  myGLCD.drawCircle(295, 25, 17);
  
  myGLCD.setFont(BigFont);
  char techChef_controls_str[25] = "Cooking Controls";
  myGLCD.print(techChef_controls_str, CENTER, 50);
  char water_str[13] = "Water Button";
  myGLCD.print(water_str, 10, 95); 
  char oil_str[11] = "Oil Button";
  myGLCD.print(oil_str, 10, 135);
  char motor_str[10] = "Motor Bar";
  myGLCD.print(motor_str, 10, 175);
  char temp_str[11] = "Temp Read:";
  myGLCD.print(temp_str, 10, 215);
  myGLCD.setColor(255, 0, 0);
  myGLCD.drawLine(0,75,319,75); 
  
  // draw the Water & Oilbutton frame & fill
  myGLCD.setColor(0, 0, 255); // Sets blue color
  myGLCD.fillRoundRect(240, 88, 310, 118); // Draws filled rounded rectangle
  myGLCD.setColor(255, 255, 255); // Sets color to white
  myGLCD.drawRoundRect(240, 88, 310, 118); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
 
  // myGLCD.drawRect(240, 128, 310, 158); // Oil - Slider
  myGLCD.setColor(246,190,0); // Sets dark yellow color
  myGLCD.fillRoundRect(240, 128, 310, 158); // Draws filled rounded rectangle
  myGLCD.setColor(255, 255, 255); // Sets color to white
  myGLCD.drawRoundRect(240, 128, 310, 158); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame


  // draw the motor & temp bar - frame only
  myGLCD.drawRect(160, 178, 310, 188); // Motor - Slider
  myGLCD.drawLine(10,203,309,203);      // draw a line/divider - above is modifiable controls, below is non-modifiable display
  // myGLCD.drawRect(160, 218, 310, 228); // Temperature - Slider
}
//====================================================
//============= setLedColor() - Custom Funtion
void setLedColor() {
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x=myTouch.getX();
    y=myTouch.getY();  
    // Area of the Water button
    if( (x>=240) && (x<=310) && (y>=88) && (y<=118)) {
      drawFrame(240, 88, 310, 118); // Custom Function -Highlighs the buttons when it's pressed
      // write ON/OFF - toggle the waterButton
      if (waterButton == true) {
        waterButton = false;
        digitalWrite(waterPin, LOW);    // update waterPin output value to LOW - stop pumping water
        Serial.print("WaterPin = 14 status: 0\n");
        
      }
      else {
        waterButton = true;
        digitalWrite(waterPin, HIGH);   // update waterPin output value to HIGH - start pumping water
        Serial.print("WaterPin = 14 status: 1\n");
      }

      // Re-draw the Water button frame & fill
      myGLCD.setColor(0, 0, 255); // Sets blue color
      myGLCD.fillRoundRect(240, 88, 310, 118); // Draws filled rounded rectangle
      myGLCD.setColor(255, 255, 255); // Sets color to white
      myGLCD.drawRoundRect(240, 88, 310, 118); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame

    }      
    // Area of the Oil button
    if( (x>=240) && (x<=310) && (y>=128) && (y<=158)) {
      drawFrame(240, 128, 310, 158); // Custom Function -Highlighs the buttons when it's pressed
      // write ON/OFF - toggle the oilButton
      if (oilButton == true) {
        oilButton = false;
        digitalWrite(oilPin, LOW);    // update oilPin output value to LOW - stop pumping oil
        Serial.print("OilPin = 15 status: 0\n");
      }
      else {
        oilButton = true;
        digitalWrite(oilPin, HIGH);   // update oilPin output value to HIGH - start pumping oil
        Serial.print("OilPin = 15 status: 1\n");
      }

      // Re-draw the Oil button frame & fill
      myGLCD.setColor(246,190,0); // Sets dark yellow color
      myGLCD.fillRoundRect(240, 128, 310, 158); // Draws filled rounded rectangle
      myGLCD.setColor(255, 255, 255); // Sets color to white
      myGLCD.drawRoundRect(240, 128, 310, 158); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
    }
    // Area of the Motor slider
    if( (y>=170) && (y<=196)) {
      xM=x;
      if (xM<=163) {
        xM=163;
      }
      if (xM>=303){
        xM=303;
      }
    }
//    // Area of the Temperature slider
//    if( (y>=210) && (y<=236)) {
//      xT=x;
//      if (xT<=163) {
//        xT=163;
//      }
//      if (xT>=303){
//        xT=303;
//      }
//    }    
  }
  
//  // Sends PWM signal to the pins of the led - modify
//  analogWrite(redLed, xOC);
//  analogWrite(greenLed, xMC);
//  analogWrite(blueLed, xTC);
 
  // settings for printing literals
  myGLCD.setColor(255, 255, 255);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(0, 0, 255);

  // print ON/OFF waterButton
  if (waterButton) {
    myGLCD.print(on_str, 259, 95); // 240 - 310, 35
  }
  else {
    myGLCD.print(off_str, 252, 95);
  }

  // print ON/OFF oilButton
  myGLCD.setBackColor(246,190,0);
  if (oilButton) {
    myGLCD.print(on_str, 259, 135);
  }
  else {
    myGLCD.print(off_str, 252, 135);
  }

  // Draws the positioners
  myGLCD.setColor(255, 255, 255);
  myGLCD.fillRect(xM,179,(xM+4),187);
  myGLCD.setColor(165,42,42);   // color coding for Motor Speed bar - brown
  myGLCD.fillRect(163, 179, (xM-1), 187);
  myGLCD.setColor(0, 0, 0);
  myGLCD.fillRect((xM+5), 179, 309, 187);
  

//  myGLCD.setColor(255, 255, 255);
//  myGLCD.fillRect(xT,219,(xT+4),227);
//  myGLCD.setColor(175, 18, 129);   // color coding for Temperature bar - pinkish purple
//  myGLCD.fillRect(163, 219, (xT-1), 227);
//  myGLCD.setColor(0, 0, 0);
//  myGLCD.fillRect((xT+5), 219, 309, 227);
}

void updateTemp() {
  // Obtain temperature data
  VRT = analogRead(A0);              //Acquisition analog value of VRT
  VRT = (5.00 / 1023.00) * VRT;      //Conversion to voltage
  VR = Vsupply - VRT;
  RT = VRT / (VR / R);               //Resistance of RT
  ln = log(RT / RT0);
  tempReading = (1 / ((ln / B) + (1 / T0))); //Temperature from thermistor
  tempReading = tempReading - 273.15;                 //Conversion to Celsius
  String tempReading_str = String(tempReading, 2);
  String celsius_sign = " C";
  tempReading_str.concat(celsius_sign);

  // update temperature data on the TFT touch screen
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0,0,0); // Sets the background color of the area where the text will be printed to black
  myGLCD.setFont(BigFont);
  myGLCD.print(tempReading_str, 200, 215);

  check_overheat_alarm(tempReading);
}

void check_overheat_alarm(int tempReading) { 
  if (tempReading >= 60) {      // [Modification] The overhead safety alarm upper limit has to be modified to match real values, don't use 60
    if (!overheat_sign) {    // if previous overheat sign is turned OFF, turn it ON here & set overheat button to true
      myGLCD.setColor(255, 0, 0);
      myGLCD.fillCircle(295, 25, 17);
      myGLCD.setColor(255, 255, 255);
      myGLCD.drawCircle(295, 25, 17);
      overheat_sign == true;
    }  
  }
  else {
    if (overheat_sign) {    // if the previous overheat is already turned ON, turn it OFF here & set overheat button to false
      myGLCD.setColor(0, 0, 0);
      myGLCD.fillCircle(295, 25, 17);
      myGLCD.setColor(255, 255, 255);
      myGLCD.drawCircle(295, 25, 17);
      overheat_sign == false;
    }
  }   
}
