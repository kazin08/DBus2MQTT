dbus: main.cpp dbus.cpp
	g++ -o dbus `pkg-config --cflags --libs dbus-1` main.cpp dbus.cpp mqtt_async_pub.cpp -I/usr/include/dbus-1.0 -I/usr/lib/x86_64-linux-gnu/dbus-1.0/include \
	-I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include/ -ldbus-1 -ldbus-glib-1 -Wall -Wextra -o dbus -I/home/ema-pc/Documents/DBus2MQTT -pthread \
	-lpaho-mqttpp3 -lpaho-mqtt3as -lssl -lcrypto -lpthread

mqtt: mqtt_async_pub.cpp
	g++ -o mqtt mqtt_async_pub.cpp -I./src/mqtt -lpaho-mqttpp3 -lpaho-mqtt3as -lssl -lcrypto -lpthread

clean:
	rm dbus2mqtt