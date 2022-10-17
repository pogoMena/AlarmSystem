/*
"StAuth10222: I Michael Mena, 000817498 certify that this material is my original work.
No other person's work has been used without due acknowledgement. I have not made my work available to anyone else."
*/
// COMP-10184 – Mohawk College
// PIR Sensor Test Application
// Turn on the blue LED with motion is detected.
#include <Arduino.h>
// digital input pin definitions
#define PIN_PIR D5
#define PIN_BUTTON D6
// *************************************************************
void setup()
{
    // configure the USB serial monitor
    Serial.begin(115200);
    // configure the LED output
    pinMode(LED_BUILTIN, OUTPUT);
    // PIR sensor is an INPUT
    pinMode(PIN_PIR, INPUT);
    // Button is an INPUT
    pinMode(PIN_BUTTON, INPUT_PULLUP);
    Serial.println("\n\n\n");
}

bool alarmCountdownAlerted = false; // false means the alarm hasnt been alerted
bool bPIR;                          // The alarm variable
bool buttonStatus;
int lightLoop = 0;
int countDown = 10;
int alarmStayOnThreshHold = 40;
bool alarmStayOn = false;
int lightOn = false; // Inverted so this really means true basically

bool alarmDeactivated = false;

void collectInputs()
{
    bPIR = digitalRead(PIN_PIR);
    buttonStatus = digitalRead(PIN_BUTTON);
}
void checkAlarmState()
{

//If the alarm hasnt been deactivated
    if (!alarmDeactivated)
    {
        // If the alarm isnt locked on yet
        if (!alarmStayOn)
        {
            // If statement to handle if the alarm isnt alerted at all yet
            if (alarmCountdownAlerted)
            {
                // when not pressed it is 1 'aka true' so i needed the '!'
                if (!buttonStatus)
                {
                    Serial.println("Alarm has been deactivated");
                    alarmDeactivated = true;
                    alarmCountdownAlerted = false;
                    lightLoop = 0;
                    countDown = 10;
                    digitalWrite(LED_BUILTIN, true);
                    delay(250);
                }
                else
                {
                    // Incrments the lightLoop variable up to 40 before staying on
                    lightLoop++;

                    // Sets the light to off initially
                    digitalWrite(LED_BUILTIN, true);

                    // turns light on and off
                    lightOn ^= true;
                    digitalWrite(LED_BUILTIN, lightOn);

                    if(lightLoop % 4 == 0){
                    Serial.println(countDown--);
                    }
                    
                    // Waits a quarter of a second
                    delay(250);

                    if (lightLoop >= alarmStayOnThreshHold)
                    {
                        Serial.println("Alarm will stay on INTRUDER INTRUDER");
                        alarmStayOn = true;
                    }
                }
            }
            else
            {
                if (bPIR)
                {
                    //Activates the acountdown for the alarm
                    Serial.println("Alarm has been alerted");
                    alarmCountdownAlerted = true;
                }
                digitalWrite(LED_BUILTIN, true);
            }
        }
        else
        {
            // Alarm has been triggered, it will stay on
            digitalWrite(LED_BUILTIN, false);
        }
    }
    //If the alarm HAS been deactivated
    else
    {
        //Reactivates the alarm
        if (!buttonStatus)
        {
            Serial.println("Alarm has been activated");
            alarmDeactivated = false;

            //Delay to avoid double presses
            delay(250);
        }
    }
}

void loop()
{

    collectInputs();
    checkAlarmState();
}
