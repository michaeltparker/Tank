/*
 * This software combines functionality pulled from two primary sources
 * (1) Code related Arduino interaction with the SH5461AS Quad 7-segment display
 * The source was GitHub repository https://github.com/UniQHW/SH5461AS.git
 * and includes the following copyright:
 * Copyright (c) 2018 Patrick Pedersen <ctx.xda@gmail.com>
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * (2) Code related to the Arduinno interaction with the HC-SR04 Ultrasonic Sensor
 * The source was GitHub repository https://github.com/sparkfun/HC-SR04_UltrasonicSensor.git
 * and includes the following copywrite:
 * Copyright (c) 2016 SparkFun Electronics
 * Permission is hereby granted, free of charge, to any person obtaining a copy 
 * of this software and associated documentation files (the "Software"), to deal 
 * in the Software without restriction, including without limitation the rights 
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies 
 * of the Software, and to permit persons to whom the Software is furnished to do 
 * so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all 
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE
 *  
 * 
 * Description:
 *  Connect the ultrasonic sensor to the Arduino as per the
 *  hardware connections below. Run the sketch and open a serial
 *  monitor. The distance read from the sensor will be displayed
 *  in centimeters and inches.
 *
 * Hardware Connections used in this software:
 *  Arduino | HC-SR04 (Ultrasonic Sensor)
 *  -------------------
 *    5V    |   VCC     
 *    12    |   Trig     
 *    13    |   Echo     
 *    GND   |   GND
 * 
 *  Arduino | SH5461AS (Quad 7-Segment Display)
 *  -------------------    
 *    6     |   com1     
 *    9     |   com2 
 *    10    |   com3
 *    0     |   com4
 *    7     |   Seg a
 *    11    |   Seg b
 *    2     |   Seg c
 *    4     |   Seg d
 *    5     |   Seg e  
 *    8     |   Seg f
 *    1     |   Seg g
 *    3     |   DP (decimal point)
 *    
 * 
 */

#include <Arduino.h>

/* Include this library */
#include "Segment.h"
#include "Digit.h"
#include "Display.h"

// Pins for SH5461AS
const byte digit_pins[4] {6, 9, 10, 0}; // From com1 - com4
const byte segment_pins[7] {7, 11, 2, 4, 5, 8, 1}; // From a - g (abc...g)
const byte dp_pin = 3;

// Pins for HC-SR04
const int TRIG_PIN = 12; //updated from 7 in copied software
const int ECHO_PIN = 13; //Updated from 8 in copied software

// Anything over 400 cm (23200 us pulse) is "out of range"
const unsigned int MAX_DIST = 23200;

//Initiate Display class object
Display d(digit_pins, segment_pins, dp_pin);

void setup() {
  //Need to revist the use of this setup script
  for (int i = 0; i <= A5; i++) {
    pinMode(i, OUTPUT);
  }
   
  // The Trigger pin will tell the sensor to range find
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);

  // We'll use the serial monitor to view the sensor output
  Serial.begin(9600);
  
}

void loop() {

  unsigned long t1;
  unsigned long t2;
  unsigned long pulse_width;
  float cm;
  float inches;

  // Hold the trigger pin high for at least 10 us
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Wait for pulse on echo pin
  while ( digitalRead(ECHO_PIN) == 0 );

  // Measure how long the echo pin was held high (pulse width)
  // Note: the micros() counter will overflow after ~70 min
  t1 = micros();
  while ( digitalRead(ECHO_PIN) == 1);
  t2 = micros();
  pulse_width = t2 - t1;

  // Calculate distance in centimeters and inches. The constants
  // are found in the datasheet, and calculated from the assumed speed 
  //of sound in air at sea level (~340 m/s).
  cm = pulse_width / 58.0;
  inches = pulse_width / 148.0;

  // Print out results
  if ( pulse_width > MAX_DIST ) {
    Serial.println("Out of range");
  } else {
    Serial.print(cm);
    Serial.print(" cm \t");
    Serial.print(inches);
    Serial.println(" in");
  }
  
   //Call the print functionn from the display object to illuminate the value
  d.print("1.234"); // Must be looped!
  
  // Wait at least 60ms before next measurement
  // NEED to revisit this approach, may need to consider a timer (or interrupt) to work with display
  delay(60);
    
}
