// This #include statement was automatically added by the Particle IDE.
#include "NCD8Relay/NCD8Relay.h"

const int centralZone = -6;
const int easternZone = -5;
const int mountainZone = -7;
const int pacificZone = -8;

int eventArray[28][9];
int numberOfEvents = 0;

bool process = false;

void newEventsHandler(const char *event, const char *data);
int setZone(String zone);
int clearMemory(String nothing);
int serialPrintMemory(String nothing);

NCD8Relay controller;

void setup() {
    controller.setAddress(0,0,0);
    if(Particle.subscribe("timeEventsDevice1", newEventsHandler, MY_DEVICES)){
        Serial.println("Subscribed");
    }else{
        Serial.println("Failed to subscribe");
    }
    Particle.function("setTimezone", setZone);
    Particle.function("clearMemory", clearMemory);
    Particle.function("printMemory", serialPrintMemory);
    int storedZone = EEPROM.read(0);
    if(storedZone != 255){
        int zone = (EEPROM.read(0)*-1);
        Time.zone(zone);
        process = true;
        readStoredEvents();
    }
}

void loop() {
    if(process){
        evalTime(Time.year(), Time.month(), Time.day(), Time.weekday(), Time.hour(), Time.minute(), Time.second());
        delay(1000);
    }else{
        Serial.println("No Stored Events");
        delay(2000);
    }
}

void newEventsHandler(const char *event, const char *data){
    Serial.println("Handler Triggered");
    storeEvents((char *)data);    
}

void storeEvents(char *events){
    Serial.printf("events at entry to storeEvents method: %s \n", events);
    //break events up into an array of ints
    int len = 0;
    String testString = String(events);
    int stringLength = testString.length();
    Serial.printf("stringLength: %i \n", stringLength);
    
    char tempEvents[stringLength+1];
    testString.toCharArray(tempEvents, stringLength+1);
    Serial.printf("tempEvents: %s \n", tempEvents);
    char * pch = strtok (tempEvents, ",");
    while (pch != NULL)
    {
        len++;
        pch = strtok (NULL, ",");
    }
    Serial.printf("len: %i \n", len);
    
    int eventArray[len];
    Serial.printf("events before going to split: %s \n", events);
    splitStringToIntArray(events, eventArray, ",");
    
    for(unsigned int i = 1; i < len*4;i+=4){
        Serial.printf("Storing %i in location %i \n", eventArray[(i-1)/4], i);
        EEPROM.put(i, eventArray[(i-1)/4]);
    }
    readStoredEvents();
}

void readStoredEvents(){
    int stopIndex = 0;
    for(int i = 1; i < 2048; i++){
        if(EEPROM.read(i) != 255){
            stopIndex++;
        }
    }
    if(stopIndex == 0){
        Serial.println("No Stored Events");
        return;
    }
    unsigned char bytes[4];
    bytes[0] = (2016 >> 24) &0xFF;
    bytes[1] = (2016 >> 16) &0xFF;
    bytes[2] = (2016 >> 8) &0xFF;
    bytes[3] = 2016&0xFF;
    Serial.printf("%i, %i, %i, %i \n", bytes[0], bytes[1], bytes[2], bytes[3]);
    int locationTest;
    EEPROM.get(1, locationTest);
    Serial.printf("location test %i \n",locationTest);
    int locationTest2;
    EEPROM.get(5, locationTest2);
    Serial.printf("location test 2 %i \n", locationTest2);
    Serial.printf("stopIndex: %i \n" , stopIndex);
    
    numberOfEvents = stopIndex/36;
    Serial.printf("numberOfEvents: %i \n", numberOfEvents);
    
    int startIndex = 1;
    for(int eventIndex = 0; eventIndex < numberOfEvents; eventIndex++){
        Serial.printf("Event at location %i: ",eventIndex);
        for(int i = startIndex; i < startIndex+36; i+=4){
            int arrayIndex = ((i-1)/4) % 9;
            EEPROM.get(i, eventArray[eventIndex][arrayIndex);
            Serial.printf("%i, ", eventArray[eventIndex][arrayIndex);
        }
        Serial.printf("Event at location %i: ",eventIndex);
        Serial.println();
        startIndex+=36;
    }
    Serial.printf("2numberOfEvents: %i \n", numberOfEvents);
}

int setZone(String zone){
    int tZone = zone.toInt();
    EEPROM.write(0, tZone);
    tZone = tZone * -1;
    Time.zone(tZone);
    process = true;
    return 1;
}

void evalTime(int year, int month, int dayOfMonth, int dayOfWeek, int hour, int minute, int second){
    //Cycle through stored events and compare to current date/time.
    for(int i = 0; i < numberOfEvents; i++){
        bool runCommand = true;
        
        //Check Year
        if(eventArray[i][0] != year){
            //Year does not match and is required
            if(eventArray[i][0] != 0){
                runCommand = false;
            }
            
        }
        //Check Month
        if(eventArray[i][1] != month){
            //Month does not match and is required
            if(eventArray[i][1] != 0){
                runCommand = false;
            }
        }
        //Check Day of Month
        if(eventArray[i][2] != dayOfMonth){
            //Day of Month does not match and is required
            if(eventArray[i][2] != 0){
                runCommand = false;
            }
        }
        //Check Day of Week
        if(eventArray[i][3] != dayOfWeek){
            //Day of Week does not match and is required
            if(eventArray[i][3] != 0){
                runCommand = false;
            }
        }
        //Check Hour
        if(eventArray[i][4] != hour){
            //Hour does not match and is required
            if(eventArray[i][4] != 24){
                runCommand = false;
            }
        }
        //Check Minute
        if(eventArray[i][5] != minute){
            //Minute does not match and is required
            if(eventArray[i][5] != 60){
                runCommand = false;
            }
        }
        //Check Second
        if(eventArray[i][6] != second){
            //Second does not match and is required
            if(eventArray[i][6] != 60){
                runCommand = false;
            }
        }
        
        if(runCommand){
            Serial.println("Running command");
            executeCommand(eventArray[i][7], eventArray[i][8]);
        }
    }
}

void splitStringToIntArray(char *s, int *buffer, String del){
    int index = 0;
    
    Serial.printf("s in splitString method: %s \n", s);
    
    char * pch = strtok (s, del);
    while (pch != NULL)
    {
        String pchString = String(pch);
        buffer[index] = pchString.toInt();
        Serial.printf("buffer[%i] = %i \n",index,pchString.toInt());
        index++;
        pch = strtok (NULL, del);
    }
}

int clearMemory(String nothing){
    for(int i = 0; i < 2048; i++){
        EEPROM.write(i, 255);
    }
}

int serialPrintMemory(String nothing){
    for(int i = 0; i < 2048; i++){
        Serial.print(EEPROM.read(i));
        Serial.print(" ");
        if(i != 0 && i%15 == 0){
            Serial.println();
        }
    }
}

void executeCommand(int commandID, int commandData){
    switch(commandID){
        case 0:
            //Turn On All Relays
            controller.turnOnAllRelays();
            break;
        case 1:
            //Turn Off All Relays
            controller.turnOffAllRelays();
            break;
        case 2:
            //Set bank Status
            controller.setBankStatus(commandData);
            break;
        case 3:
            //Turn On Relay #
            controller.turnOnRelay(commandData);
            break;
        case 4:
            //Turn Off Relay #
            controller.turnOffRelay(commandData);
            break;
        case 5:
            //Toggle Relay #
            controller.toggleRelay(commandData);
            break;
    }
}
