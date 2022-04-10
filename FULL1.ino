// Include the libraries
#include "ClosedCube_TCA9548A.h"
#include "RTClib.h"
#include <TM1637Display.h>
#include <LCD_I2C.h>
#include <Wire.h>

#define TCA9548A_I2C_ADDRESS	0x70
// Define the TM1637Display connections pins
#define CLK 27
#define DIO 26

// Create rtc and display object
RTC_DS3231 rtc;
TM1637Display display = TM1637Display(CLK, DIO);
ClosedCube::Wired::TCA9548A tca9548a;




LCD_I2C lcd(0x27, 16, 2); // Default address of most PCF8574 modules, change according
const int trigPin = 14;
const int echoPin = 12;
long duration;
float distanceCm;
float distanceInch;
// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701



void TCA9548A(uint8_t bus) //function of TCA9548A
{
  Wire.beginTransmission(0x70);  // TCA9548A address is 0x70
  Wire.write(1 << bus);          // send byte to select bus
  Wire.endTransmission();
}


void setup() {
	// Begin serial communication
 pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input  
	Serial.begin(115200);
 Wire.begin();
  TCA9548A(1);
    lcd.begin(); // If you are using more I2C devices using the Wire library use lcd.begin(false)
                 // this stop the library(LCD_I2C) from calling Wire.begin()
    lcd.backlight();
	// Check if RTC is connected correctly
   TCA9548A(7);
	if (! rtc.begin()) {
		Serial.println("Couldn't find RTC");
		while (1);
	}
	// Check if the RTC lost power and if so, set the time
	if (rtc.lostPower()) {
		Serial.println("RTC lost power, lets set the time!");
		// The following line sets the RTC to the date & time this sketch was compiled:
		rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
		// This line sets the RTC with an explicit date & time, for example to set
		// January 21, 2014 at 3am you would call:
		//rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
	}

	// Set the display brightness (0-7)
	display.setBrightness(5);
	
	// Clear the display
	display.clear();
}

void loop() {
	// Get current date and time
   TCA9548A(7);
	DateTime now = rtc.now();

	// Create time format to display
	int displaytime = (now.hour() * 100) + now.minute();

	// Display the current time in 24 hour format with leading zeros and a center colon enabled
	display.showNumberDecEx(displaytime, 0b11100000, true);
   TCA9548A(1);
 digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculate the distance
  distanceCm = duration * SOUND_SPEED/2;
  
  // Convert to inches
  distanceInch = distanceCm * CM_TO_INCH;
  
  // Prints the distance in the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  Serial.print("Distance (inch): ");
  Serial.println(distanceInch);
  // set cursor to first column, first row
  lcd.setCursor(0, 0);
  // print message
  lcd.print("Distance");
  // set cursor to first column, second row
  lcd.setCursor(0,1);
  lcd.print(distanceCm);
  lcd.print(" cm");

}