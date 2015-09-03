#include "NCD8Relay.h"
#include "spark_wiring_usbserial.h"

//Comment line below out to turn off Serial logging
//#define LOGGING

int address = 0x20;
int address2 = 0x21;
int retrys = 0;
byte outputRegister = 0x0A;

//Constructor
NCD8Relay::NCD8Relay(){
}

//Retry added
void NCD8Relay::setAddress(int a0, int a1, int a2){
    address = 0x20;
    if(a0 == 1){
        address = address | 1;
    }
    if(a1 == 1){
        address = address | 2;
    }
    if(a2 == 1){
        address = address | 4;
    }
    //Start I2C port
    Wire.begin();
    //Open connection to specified address
    retryAddress1:
    Wire.beginTransmission(address);
    //Set all channels to outputs
    Wire.write(0x00);
    Wire.write(0x00);
    //Determine if device is present at that address
    byte status = Wire.endTransmission();
    
    Wire.beginTransmission(address);
    Wire.write(0x06);
    Wire.write(0x00);
    status = Wire.endTransmission();
    if(status != 0){
        if(retrys < 3){
    #ifdef LOGGING
            Serial.println("Retry set address command");
    #endif
            retrys++;
            goto retryAddress1;
        }else{
    #ifdef LOGGING
            Serial.println("Set Address failed");
    #endif
            initialized = false;
            retrys = 0;
        }
        
    }else{
        // Serial.println("Command Successful");
        initialized = true;
        retrys = 0;
        readStatus();
    }
}

void NCD8Relay::turnOnRelay(int relay){
    if(relay > 8 || relay < 0){
        return;
    }
    byte bankValue = bankOneStatus;
    byte registerAddress = 0x0A;
    switch(relay){
        case 1:
            bankValue = bankValue | 1;
            break;
        case 2:
            bankValue = bankValue | 2;
            break;
        case 3:
            bankValue = bankValue | 4;
            break;
        case 4:
            bankValue = bankValue | 8;
            break;
        case 5:
        	bankValue = bankValue | 16;
        	break;
        case 6:
        	bankValue = bankValue | 32;
        	break;
        case 7:
        	bankValue = bankValue | 64;
        	break;
        case 8:
        	bankValue = bankValue | 128;
        	break;
    }
    turnOnRelayRetry:
    Wire.beginTransmission(address);
    Wire.write(registerAddress);
    Wire.write(bankValue);
    byte status = Wire.endTransmission();
    if(status != 0){
        if(retrys < 3){
    #ifdef LOGGING
            Serial.println("Retry Turn On Relay command");
    #endif
            retrys++;
            goto turnOnRelayRetry;
        }else{
    #ifdef LOGGING
            Serial.println("Turn On Relay failed");
    #endif
            initialized = false;
            retrys = 0;
        }
    }else{
        initialized = true;
        retrys = 0;
        readStatus();
    }
}

void NCD8Relay::turnOffRelay(int relay){
    if(relay > 8 || relay < 0){
        return;
    }
    byte bankValue = bankOneStatus;
    byte registerAddress = 0x0A;
    switch(relay){
        case 1:
            bankValue = bankValue & ~1;
            break;
        case 2:
            bankValue = bankValue & ~2;
            break;
        case 3:
            bankValue = bankValue & ~4;
            break;
        case 4:
            bankValue = bankValue & ~8;
            break;
        case 5:
        	bankValue = bankValue & ~16;
        	break;
        case 6:
        	bankValue = bankValue & ~32;
        	break;
        case 7:
        	bankValue = bankValue & ~64;
        	break;
        case 8:
        	bankValue = bankValue & ~128;
        	break;
    }
turnOffRelaysRetry:
    Wire.beginTransmission(address);
    Wire.write(registerAddress);
    Wire.write(bankValue);
    byte status = Wire.endTransmission();
    if(status != 0){
        if(retrys < 3){
#ifdef LOGGING
            Serial.println("Retry Turn Off Relay command");
#endif
            retrys++;
            goto turnOffRelaysRetry;
        }else{
#ifdef LOGGING
            Serial.println("Turn Off Relay command failed");
#endif
            initialized = false;
            retrys = 0;
        }
    }else{
        initialized = true;
        retrys = 0;
        readStatus();
    }
}

void NCD8Relay::turnOnAllRelays(){
turnOnAllRelays1:
    Wire.beginTransmission(address);
    Wire.write(0x0A);
    Wire.write(0xFF);
    byte status = Wire.endTransmission();
    if(status != 0){
        if(retrys < 3){
    #ifdef LOGGING
            Serial.println("Retry turn off all relays banks 1-2 command");
    #endif
            retrys++;
            goto turnOnAllRelays1;
        }else{
    #ifdef LOGGING
            Serial.println("Turn off all relays banks 1-2 command failed");
    #endif
            initialized = false;
            retrys = 0;
        }
    }else{
        initialized = true;
        retrys = 0;
        readStatus();
    }
}

void NCD8Relay::turnOffAllRelays(){
turnOffAllRelaysRetry1:
    Wire.beginTransmission(address);
    Wire.write(0x0A);
    Wire.write(0);
    byte status = Wire.endTransmission();
    if(status != 0){
        if(retrys < 3){
    #ifdef LOGGING
            Serial.println("Retry turn off all relays banks 1-2 command");
    #endif
            retrys++;
            goto turnOffAllRelaysRetry1;
        }else{
    #ifdef LOGGING
            Serial.println("Turn off all relays banks 1-2 command failed");
    #endif
            initialized = false;
            retrys = 0;
        }
    }else{
        initialized = true;
        retrys = 0;
        readStatus();
    }
}

void NCD8Relay::toggleRelay(int relay){
    if(relay > 8 || relay < 0){
        return;
    }
    byte bankValue =  bankOneStatus;
    switch(relay){
        case 1:
            bankValue = bankValue ^ 1;
            break;
        case 2:
            bankValue = bankValue ^ 2;
            break;
        case 3:
            bankValue = bankValue ^ 4;
            break;
        case 4:
            bankValue = bankValue ^ 8;
            break;
        case 5:
        	bankValue = bankValue ^ 16;
        	break;
        case 6:
        	bankValue = bankValue ^ 32;
        	break;
        case 7:
        	bankValue = bankValue ^ 64;
        	break;
        case 8:
        	bankValue = bankValue ^ 128;
        	break;
    }
toggleRelayRetry:
    Wire.beginTransmission(address);
    Wire.write(0x0A);
    Wire.write(bankValue);
    byte status = Wire.endTransmission();
    if(status != 0){
        if(retrys < 3){
#ifdef LOGGING
            Serial.println("Retry toggle relay command");
#endif
            retrys++;
            goto toggleRelayRetry;
        }else{
#ifdef LOGGING
            Serial.println("Toggle relay command failed");
#endif
            initialized = false;
            retrys = 0;
        }
    }else{
        initialized = true;
        retrys = 0;
        readStatus();
    }
}

void NCD8Relay::setBankStatus(int status){
    setBankStatusRetry:
    Wire.beginTransmission(address);
    Wire.write(0x0A);
    Wire.write(status);
    byte s = Wire.endTransmission();
    if(s != 0){
        if(retrys < 3){
    #ifdef LOGGING
            Serial.println("Retry set bank status command");
    #endif
            retrys++;
            goto setBankStatusRetry;
        }else{
    #ifdef LOGGING
            Serial.println("Set bank status command failed");
    #endif
            initialized = false;
            retrys = 0;
        }
    }else{
        initialized = true;
        retrys = 0;
        readStatus();
    }
}

int NCD8Relay::readRelayStatus(int relay){
    if(relay > 8 || relay < 0){
        return 256;
    }
    int value;
    switch(relay){
        case 1:
            value = 1;
            break;
        case 2:
            value = 2;
            break;
        case 3:
        	value = 4;
        	break;
        case 4:
        	value = 8;
        	break;
        case 5:
        	value = 16;
        	break;
        case 6:
        	value = 32;
        	break;
        case 7:
        	value = 64;
        	break;
        case 8:
        	value = 128;
        	break;
    }
    getRelayStatusRetry:
    Wire.beginTransmission(address);
    Wire.write(0x0A);
    byte status = Wire.endTransmission();
    if(status != 0){
        if(retrys < 3){
    #ifdef LOGGING
            Serial.println("Retry read relay status command");
    #endif
            retrys++;
            goto getRelayStatusRetry;
        }else{
    #ifdef LOGGING
            Serial.println("Read relay status command failed");
    #endif
            initialized = false;
            retrys = 0;
            return 256;
        }
    }else{
        retrys = 0;
        initialized = true;
    }
    Wire.requestFrom(address, 1);
    byte bankStatus = Wire.read();
    if(bankStatus & value){
        return 1;
    }else{
        return 0;
    }
}

int NCD8Relay::readRelayBankStatus(){
    readBankStatusRetry:
    Wire.beginTransmission(address);
    Wire.write(0x0A);
    byte status = Wire.endTransmission();
    if(status != 0){
        if(retrys < 3){
    #ifdef LOGGING
            Serial.println("Retry read bank status command");
    #endif
            retrys++;
            goto readBankStatusRetry;
        }else{
    #ifdef LOGGING
            Serial.println("Read bank status command failed");
    #endif
            initialized = false;
            retrys = 0;
            return 256;
        }
    }else{
        retrys = 0;
        initialized = true;
    }
    Wire.requestFrom(address, 1);
    byte bankStatus = Wire.read();
    return bankStatus;
}

void NCD8Relay::readStatus(){
readBankOneRetry:
    //Open Connection to controller
    Wire.beginTransmission(address);
    //Get current status of relays 1-8
    Wire.write(0x0A);
    byte status = Wire.endTransmission();
    if(status != 0){
        if(retrys < 3){
    #ifdef LOGGING
            Serial.println("Retry read status banks 1-2 command");
    #endif
            retrys++;
            goto readBankOneRetry;
        }else{
    #ifdef LOGGING
            Serial.println("Read ststus banks 1-2 command failed");
    #endif
            initialized = false;
            retrys = 0;
        }
    }else{
        retrys = 0;
        initialized = true;
        Wire.requestFrom(address, 1);
        bankOneStatus = Wire.read();
    }
    
}
