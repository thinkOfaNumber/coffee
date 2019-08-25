# Coffee
E61 / HX boiler coffee machine controller

## The problem
ECM Giotto PP coffee machine controller has “died”. The heating element relay has stopped working.
## The requirements
1. Fix it so I can keep making great coffee at home
2. Learn a bit about microcontrollers, Arduino’s etc.
3. Connect to WiFi or Bluetooth for fun & learning more
## Existing I/O
### Digital Inputs
#### Tank low water level
#### Boiler low water level
#### Manual lever activated switch
#### Pressure activated switch
### Digital Outputs
#### 240V heating element
#### 240V vibration pump
#### 240V solenoid/valve
## Risk Analysis
Risk | Mitigation
---- | ----------
Electrocution with 240V wiring | a)	Disconnect all 240V sources before working on the inside of the machine b)	Ensure 240V wiring is active-switched so components are not “live” when off c)	All wiring must be rated for current and insulated d)	All terminations must be firmly fastened and not expose bare wires
Over-pressurise and blow-up the boiler | e)	Existing thermo-fuse inline to heating element f)	Existing over-pressure relief valve g)	Software timer on pressure-switch to element?
Damage to the pump by running it dry | h)	Software must ensure the pump is never run when the tank water level is low
Damage to the boiler by boiling it dry | i)	Software must ensure that the boiler element is not turned on if boiler water is low
Damage to electronics from internal water leaks | j)	Electronics should be water-resistant from drips/leaks by enclosing in plastic (with drain / ports on bottom side)
## Functions
### Heating the boiler
When pressure switch is low, element should be switched on; when pressure switch is high, element should be switched off.
### Pump protection
When the tank water level is low, the pump must not be operated.
### Boiler water
a)	When the boiler water level is low, the heating element must be turned/kept off
b)	When the boiler water level is low, if the pump can be switched on (subject to 5.2) the pump must be switched on, and the solenoid set to boiler-inlet
c)	When the boiler water level is high, the pump must be switched off, and the solenoid released
### Grouphead water (‘extracting’ the coffee)
a)	When the lever is lifted, if the pump can be switched on (subject to 5.2) and is not being used to fill the boiler (subject to 5.3), the pump must be switched on, and the solenoid set to grouphead outlet
b)	When the lever is lowered, the pump must be switched off and the solenoid released
## Parts
 
## Notes
Growl for iOS notifications
Arduino SSR
diagram

