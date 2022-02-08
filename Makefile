all: dbus2mqtt dbus

dbus2mqtt: main.cpp
	g++ -o dbus2mqtt `pkg-config --cflags --libs dbus-1` main.cpp mqtt_async_pub.cpp dbus.cpp -I/usr/include/dbus-1.0 -I/usr/lib/x86_64-linux-gnu/dbus-1.0/include \
	-I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include/ -ldbus-1 -ldbus-glib-1 -Wall -Wextra -pthread \
	-lpaho-mqttpp3 -lpaho-mqtt3as -lssl -lcrypto -lpthread

dbus: sendDbusMsg.cpp
	g++ -o dbus `pkg-config --cflags --libs dbus-1` sendDbusMsg.cpp dbus.cpp -I/usr/include/dbus-1.0 -I/usr/lib/x86_64-linux-gnu/dbus-1.0/include \
	-I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include/ -ldbus-1 -ldbus-glib-1 -Wall -Wextra -lpaho-mqttpp3 -lpaho-mqtt3as

clean:
	rm dbus2mqtt dbus