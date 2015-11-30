/* 
 * File:   main.c
 * Author: bart en gerjan
 *
 * Created on 7 april 2015, 13:58
 */

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <jansson.h>
#include <string.h>

#include "json.h"
#include "httpput.h"

int checkPin(int pinID);
void setPin(int pinID, int state);

int state = 0;
json_t *json;
int on = 0, countoff = 0;
int counter;

/*
 * constanten deze worden niet verandert in de code
 */
int _lampId = 2, _bwSensPin = 6, _maxCounter = 40, _false = 0, _true = 1, _error = -1;
char *_command = "curl http://localhost > /dev/null 2>&1"; // command voor het system setup command
char *_fileLightOn = "/var/www/json/TurnLightOn.json"; // lichts on json file
char *_fileLightOff = "/var/www/json/TurnLightOff.json"; // licht off json file
char *_stateUrl = "http://192.168.1.2/api/newdeveloper/lights/3/state"; // url to the filename
char *_MovementEnabled ="MovementEnabled"; // 
char *_state = "state";

/*
 * 
 */
int main(int argc, char** argv) {
    wiringPiSetup();
    for (;;) {
        if (checkPin(_bwSensPin)== _true && on == _false) {
            on = _true;
            countoff = _false;
            //before getting the json, we need to run the php script (for debug remove >/dev/null 2>&1)
            system(_command);
            json = loadJson();
            if (checkEnabled(json, _lampId) == _true) {
                //check if lamp is turned off; if true, turn it on
                if (getState(json, _lampId) == _false) {
                    // TODO: URL must be dynamic
                printf("lamp %i aan",_lampId);
                    httpput(_fileLightOn, _stateUrl);
                }
            }
        } else if ((checkPin(_bwSensPin)== _false) && on == _true) {
            on = _false;
            //before getting the json, we need to run the php script (for debug remove >/dev/null 2>&1)
            system(_command);
            json = loadJson();
            //delay(500);
            if (checkEnabled(json, _lampId) == _true) {

                // check if there is nobody in room; then turn the light off
                if (getState(json, _lampId) == _true) {
                    countoff = _true;
                    counter = _maxCounter;
                }
            }
        }
        if (countoff == _true) {
            counter--;
            if (counter <= 0) {
                countoff = _false;
                printf("lamp %i uit",_lampId);
                httpput(_fileLightOff, _stateUrl);
            }
        }
        delay(100);

    }
    return (EXIT_SUCCESS);
}

int checkEnabled(json_t *json, int lampID) {
    json_t *arrayJson;
    if (json_is_array(json)) {
        arrayJson = json_array_get(json, lampID); // pak lampID hue lamp uit de array
    } else {
        return (_error);
    }
    if (json_is_object(arrayJson)) {
        return ( json_integer_value(json_object_get(arrayJson, _MovementEnabled)));
    } else {
        return (_error);
    }
}

int getState(json_t *json, int lampID) {
    json_t *arrayJson;
    if (json_is_array(json)) {
        arrayJson = json_array_get(json, lampID); // pak lampID hue lamp uit de array
    } else {
        return (_error);
    }
    if (json_is_object(arrayJson)) {
        return ( json_integer_value(json_object_get(arrayJson, _state)));
    } else {
        return (_error);
    }
}

int checkPin(int pinID) {
    pinMode(pinID, INPUT);
    if (digitalRead(pinID)) return _true;
    else return _false;
}

void setPin(int pinID, int state) {
    pinMode(pinID, OUTPUT);
    if (state == _false)
        digitalWrite(pinID, LOW);
    if (state == _true)
        digitalWrite(pinID, HIGH);
}
