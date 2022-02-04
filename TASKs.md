# Concept and design

Assume there’s a device model consisting of three layers 
* Device
* Entity (with sub-entities)
* Feature
where the following applies:
* 1 Device has 1..K Entities
* 1 Entity has 0..L sub-Entities
* 1 Entity has 1 parent (Device or Entity)
* 1 Entity has 0..M Features
* 1 Feature has 1 parent (Entity)

## Task1
How can a commercially available fridge/freezer combination be modelled with this device model? Hint: As an example, it can be expected that the freezer part has a switch to control the cooling temperature setpoint. This switch would be modelled as a feature. 

### Answer
The commercial fridge/freezer combination can be modelled like this:
Device = Brand name (ej: Samsung, LG, etc)
Entity = Model of the brand (ej: RT43K6235BS/BG, GSXV90BSAE, etc)
Feature = door light, temperature, humidity, current consumption, etc

## Task2
Assume that a library should be written that supports working with such a tree data structure. How would you design the interface of such a library (pseudo code)?
Note: This is only about the interface, do not implement the library.

### Answer
Use the brand as a code (ej: Samsung = 1, LG = 2, etc) then use the exact code of the fridge/freezer again with a new numerical code (ej: RT43K6235BS/BG = 1, RT43K6236BS/BG = 2) and for the feature we can use a normally numerical order for all the features for all brands. (ej= door light will be always 1, etc)
This logic is similar to MIB in SNMP protocol.

## Task3
Write a C++ Linux application with a DBus interface and a MQTT interface. The application should be able to receive a simple text message on the DBus interface and publish this message on the MQTT interface. The functionality of the program should be demonstrated using publicly available tools for both technologies.