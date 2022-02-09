# DBus2MQTT
Write a C++ Linux application with a DBus interface and a MQTT interface. The application should be able to receive a simple text message on the DBus interface and publish this message on the MQTT interface. The functionality of the program should be demonstrated using publicly available tools for both technologies.

## Build docker
To generate the docker image, use the following command:

```bash
docker build -t test .
```

After the generation of the docker image, first we need to run the image and then execute it:

### Run the image
```bash
docker run --privileged -it -d --rm --name dbus_image test
```

### Execute the image
```bash
docker exec -it dbus_image /bin/bash
```

## Execute the test
Inside the container, we have to execute the follow command to get access to the debus in that shell:

```bash
export $(dbus-launch)
```

In the folder inside the container, we have 2 executable files: dbus and dbus2mqtt.
dbus2mqtt has the dbus server and executing without any args, it will wait for a message from dbus that can be sent using dbus, but for this we need to open another console and enter in the container again using the command from Execute the image. Also with another container we can see the MQTT message using mosquitto.

To test dbus2mqtt with a message sended from the same executable use:

```bash
./dbus2mqtt "message"
```

This will initialize the Dbus server and then send the "message" from Dbus and send it to the MQTT broker.
To subscribe all the MQTT topics, execute:

```bash
mosquitto_sub -h localhost -t '#'
```

To test dbus2mqtt sending individuals message from dbus, execute:

```bash
./dbus2mqtt
```

Then in another console:

```bash
./dbus send "message"
```

Sending the message exit from dbus, will stop the dbus server.