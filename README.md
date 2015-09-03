# About

This Library is intended for use with NCD 8 Relay Particle Core/Photon compatible relay controllers.

The intention of this library is to make use of the NCD 8 channel relay controller with Particle development web IDE as simple as possible for users.
###Developer information
NCD has been designing and manufacturing computer control products since 1995.  We have specialized in hardware design and manufacturing of Relay controllers for 20 years.  We pride ourselves as being the industry leader of computer control relay products.  Our products are proven reliable and we are very excited to support Particle.  For more information on NCD please visit www.controlanything.com 

###Requirements
- NCD 8 Channel Particle Core/Photon Compatible Relay board
- Particle Core/Photon module
- Knowledge base for developing and programming with Particle Core/Photon modules.

### Version
1.0.0

### How to use this library

The libary must be imported into your application.  This can be done through the Particle WEB IDE by selecting Libraries, then select the NCD8Relay.  Click Include in App button.  Select the App you want to include the library in.  Finally click Add to this app.  For more information see [Particles's documentation] [sparkIncludeLibrary] 

### Example use

Once the Library is included in your applicaiton you should see an include statement at the top like this:
```cpp
//This #include statement was automatically added by the Particle IDE.
#include "NCD8Relay/NCD8Relay.h"
```
Now you need to instanciate an object of the library for use in your application like this:
```cpp
NCD8Relay relayController;
```

Here is an example use case for the class
```cpp
// This #include statement was automatically added by the Particle IDE.
#include "NCD8Relay/NCD8Relay.h"
NCD8Relay relayController;

void setup() {
Serial.begin(115200);
relayController.setAddress(0, 0, 0);
}

void loop() {
//Test relays
for(int i = 1; i < 9; i++){
relayController.turnOnRelay(i);
int rOnStatus = relayController.readRelayStatus(i);
Serial.print("Relay status after on: ");
Serial.println(rOnStatus);
delay(500);
}
for(int i = 1; i < 9; i++){
relayController.turnOffRelay(i);
int rOffStatus = relayController.readRelayStatus(i);
Serial.print("Relay status after off: ");
Serial.println(rOffStatus);
delay(500);
}

relayController.turnOnAllRelays();
delay(500);
relayController.turnOffAllRelays();
delay(500);

for(int i = 1; i < 9; i++){
relayController.toggleRelay(i);
delay(500);
relayController.toggleRelay(i);
delay(500);
}
relayController.setBankStatus(85);
int rStatus85 = relayController.readRelayBankStatus();
Serial.print("Status after setting relay bank status to 85: ");
Serial.println(rStatus85);
delay(500);
relayController.setBankStatus(170);
delay(500);
relayController.turnOffAllRelays();
delay(500);
}
```

###Public accessible methods
```cpp
void setAddress(int a0, int a1, int a2);
```
>Must be called first before using the object.  This method should also be called any time communication with
>the controller is lost or broken to recover communication  This method accepts two int arguments.  This
>tells the Library what address to direct commands to.  a0 and a1 ints are representations of the two
>jumpers on the 8 channel relay controller which are labeled on the board A0, A1, and A2.  If the jumper is
>installed then that int in this call should be set to 1.  If it is not installed then the int should be set to 
So if I have A0, A1, and A2 installed I would call ```relayController.setAddress(1, 1, 1).```


```cpp
void turnOnRelay(int Relay);
```
>This method accepts one int argument.  Valid int arguments 1-8.  A call to this method will turn on the
>relay indicated by the passed int argument.


```cpp
void turnOffRelay(int Relay);
```
>This method accepts one int argument.  Valid int arguments 1-8.  A call to this method will turn off the relay
>indicated by the passed int argument.


```cpp
void turnOnAllRelays();
```
>This method does not accept any arguments.  A call to this method will turn on all 8 relays on the
>controller.


```cpp
void turnOffAllRelays();
```
>This method does not accept any arguments.  A call to this method will turn off all 8 relays on the
>controller.


```cpp
void toggleRelay(int relay);
```
>This method accepts one int argument.  Valid int arguments are 1-8.  A call to this method will toggle the
>status of the relay indicated by the passed int argument.  If the relay was previously off before the method
>call the relay will turn on and vice versa.


```cpp
void setBankStatus(int status);
```
>This method accepts two int arguments.  Valid status int arguments 0-255.  A call
>to this method will set the status of all relays on the board to the status byte passed in
the argument(status).  Each relay on the board(total of 8) are represented as bits in the status
>argument.


```cpp
int readRelayStatus(int relay);
```
>This method accepts one int argument and returns one int.  Valid relay int arguments 1-8.  A call to this
>method will read the status of the given relay passed by the relay argument and return the current on/off
>status of the given relay.  1 will be returned if the relay is on and 0 will be returned if the relay is off. 
>256 will be returned if an error has occured(generally due to lack of communication with the controller).


```cpp
int readRelayBankStatus();
```
>This method accepts no arguments and returns one int.  A call to this
>method will read and return the status of all relays on the board. 
>Each relay in the bank is represented as a bit in the returned int.  Valid returns are 0-255.  256 will be
>returned if an error has occured(generally due to lack of communciation with controller).


###Public accessible variables
```cpp
bool initialized;
```
>This boolean indicates the current status of the interface connection to the controller.  This variable should
>be checked often throughout your application.  If communication to the board is lost for any reason this
>boolean variable will return false.  If all is well it will return true.


License
----

GNU
[sparkIncludeLibrary]:https://docs.particle.io/guide/getting-started/build/photon/