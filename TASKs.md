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
Thinking like the structure was similar to the SNMP protocol.

The interface should be:
* Read_values(addr)
* Set_values(addr, val)
* Get_info()

#### Read_values(addr)
Function to read values inside the Device like the status of door light, temperature, current consumption, etc. To access one feature of the device, in the function we have to pass the address value of the feature.
* Initialise the communication
* Send the data to the device and wait for the answer
* When the device respond, save the data in a variable
* Return the value

#### Set_values(addr, val)
Function to set value for the features that accept setting values. Again, the function will need the address and the value to set and will respond if the set has success or not. For example, set the Feature temperature of the fridge.
* Initialise the communication
* Send the address and value to the device
* Wait for respond OK from device
* Return True if the response was OK

#### Get_info()
This function returns all the values of the devices and can include the address of the features. This function is the first function to start the communication to get the necessary values of the device.
* Initialise the communication
* Send command to get info from the Device
* Receive the values and wait for the finish signal from Device
* Return all the values

## Task3
Write a C++ Linux application with a DBus interface and a MQTT interface. The application should be able to receive a simple text message on the DBus interface and publish this message on the MQTT interface. The functionality of the program should be demonstrated using publicly available tools for both technologies.