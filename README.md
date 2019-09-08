# Coffee
E61 / HX boiler coffee machine controller

## 1 The problem
ECM Giotto PP coffee machine controller has “died”. The heating element relay has stopped working.
## 2 The requirements
1. Fix it so I can keep making great coffee at home
2. Learn a bit about microcontrollers, Arduino’s etc.
3. Connect to WiFi or Bluetooth for fun & learning more
## 3 Existing I/O
### 3.1 Digital Inputs
#### 3.1.1 Tank low water level
#### 3.1.2 Boiler low water level
#### 3.1.3 Manual lever activated switch
#### 3.1.4 Pressure activated switch
### 3.2 Digital Outputs
#### 3.2.1 240V heating element
#### 3.2.2 240V vibration pump
#### 3.2.3 240V solenoid/valve
## 4 Risk Analysis
Risk | Mitigation
---- | ----------
Electrocution with 240V wiring | a)	Disconnect all 240V sources before working on the inside of the machine b)	Ensure 240V wiring is active-switched so components are not “live” when off c)	All wiring must be rated for current and insulated d)	All terminations must be firmly fastened and not expose bare wires
Over-pressurise and blow-up the boiler | e)	Existing thermo-fuse inline to heating element f)	Existing over-pressure relief valve g)	Software timer on pressure-switch to element?
Damage to the pump by running it dry | h)	Software must ensure the pump is never run when the tank water level is low
Damage to the boiler by boiling it dry | i)	Software must ensure that the boiler element is not turned on if boiler water is low
Damage to electronics from internal water leaks | j)	Electronics should be water-resistant from drips/leaks by enclosing in plastic (with drain / ports on bottom side)
## 5 Functions
### 5.1 Heating the boiler
When pressure switch is low, element should be switched on; when pressure switch is high, element should be switched off.
### 2 Pump protection
When the tank water level is low, the pump must not be operated.
### 5.3 Boiler water
1. When the boiler water level is low, the heating element must be turned/kept off
2. When the boiler water level is low, if the pump can be switched on (subject to 5.2) the pump must be switched on, and the solenoid set to boiler-inlet
3. When the boiler water level is high, the pump must be switched off, and the solenoid released
### 5.4 Grouphead water (‘extracting’ the coffee)
1. When the lever is lifted, if the pump can be switched on (subject to 5.2) and is not being used to fill the boiler (subject to 5.3), the pump must be switched on, and the solenoid set to grouphead outlet
2. When the lever is lowered, the pump must be switched off and the solenoid released
## 6 Parts
 
## 7 Notes
Growl for iOS notifications

Arduino SSR

diagram


ESP8266 pullup/down resistors vs WeMos on-board ones:
https://www.reddit.com/r/esp8266/comments/49k38s/esp8266_pulluppulldown_enabledisable_functions/

LED overview:
https://learn.adafruit.com/all-about-leds
