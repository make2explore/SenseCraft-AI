// ---------------------------------- make2explore.com -------------------------------------------------------//
// Project           - Gesture Controlled Home Appliances + SenseCraft AI Platform Application
// Created By        - info@make2explore.com
// Last Modified     - 04/04/2025 17:36:00 @admin
// Software          - C/C++, Arduino IDE, Libraries
// Hardware          - Xiao ESP32S3, Grove Vision AI V2, 2 Channel Relay Board, 5V DC Fan, AC Bulb, Grove base     
// Sensors Used      - Grove Vision AI V2 with Raspberry Pi Camera V1.3
// Source Repo       - github.com/make2explore
// ===========================================================================================================//

// Include necessary libraries
#include <Arduino.h>
#include <Wire.h>                                 // Include Wire/I2C library
#include <Seeed_Arduino_SSCMA.h>                  // Include SSCMA library

// Declare an instance of the SSCMA class, named AI.
SSCMA AI;

#define SSCMA_CONNECT_TO_XIAO_S3         1        // Connect SSCMA to Xiao ESP32S3
#define SSCMA_CONNECT_TO_GORVE_VISION_AI 0

// Pin Definitions - D0 and D1 pins of Xiao ESP32S3 board are connected to the Relays
const int R1 = D0;
const int R2 = D1;

// Setup Function
void setup()
{
// Serial port initialization
Serial.begin(115200);
while (!Serial) delay(1000);
// I2C init for Xiao
#if SSCMA_CONNECT_TO_XIAO_S3
    Wire.begin();
    AI.begin(&Wire);                              // initializes the AI model.
#endif
// I2C init for Grove Vision
#if SSCMA_CONNECT_TO_GORVE_VISION_AI
    Wire.begin();
    AI.begin(&Wire, D3);
#endif

// Pins connected to Relay module set as OUTPUT
pinMode(R1, OUTPUT);
pinMode(R2, OUTPUT);

// Relay module support Active low signal, So First send HIGH signal to Turn off the Relays
digitalWrite(R1, HIGH);
digitalWrite(R2, HIGH);

}


// Loop Function
void loop()
{
  if (!AI.invoke(1, false, true))                 // Call AI.invoke() to run the AI model.
  {
    for (int i = 0; i < AI.boxes().size(); i++)   // Traverse through each and every bounding box frame
    {
      if(AI.boxes()[i].target == 1){              // Check for Gesture - Rock
        if(AI.boxes()[i].score >= 70){            // Check for level of confidence - >70
          digitalWrite(R1, LOW);                  // Turn On the Bulb
        }
      }
      if(AI.boxes()[i].target == 2){              // Check for Gesture - Scissors
        if(AI.boxes()[i].score >= 70){            // Check for level of confidence - >70
          digitalWrite(R2, LOW);                  // Turn On the Fan
        }
      }
      if(AI.boxes()[i].target == 0){              // Check for Gesture - Paper
        if(AI.boxes()[i].score >= 70){            // Check for level of confidence - >70
          digitalWrite(R1, HIGH);                 // Turn OFF the Bulb
          digitalWrite(R2, HIGH);                 // Turn OFF the Fan
        }
      }
    }
  }
}

// ---------------------------------- make2explore.com----------------------------------------------------//