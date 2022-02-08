#Download base image ubuntu 20.04
FROM ubuntu:20.04

# Disable Prompt During Packages Installation
ARG DEBIAN_FRONTEND=noninteractive

# Update Ubuntu Software repository
RUN apt update

# Install git and mosquitto broker
RUN apt install -y sudo git mosquitto mosquitto-clients 

# Install cmake
RUN apt install -y build-essential gcc make cmake cmake-gui cmake-curses-gui libssl-dev doxygen graphviz

# Install DBus libreary for ubuntu
RUN apt install -y dbus dbus-x11 libdbus-1-dev libdbus-glib-1-2 libdbus-glib-1-dev

# Create the directory to work in
RUN mkdir /DBus2MQTT

# Change to the directory
WORKDIR /DBus2MQTT

# Download source code from git
# MQTT
RUN git clone https://github.com/eclipse/paho.mqtt.cpp.git

# Change workdir to install paho mqtt c libs
WORKDIR /DBus2MQTT/paho.mqtt.cpp
RUN ./install_paho_mqtt_c.sh
RUN cmake -Bbuild -H. -DPAHO_BUILD_STATIC=ON \
    -DPAHO_BUILD_DOCUMENTATION=TRUE -DPAHO_BUILD_SAMPLES=TRUE
RUN cmake --build build/ --target install
RUN ldconfig

# Change Workdir to the main
WORKDIR /DBus2MQTT

# Copy files from local to docker to build program
COPY . /DBus2MQTT/

# Compile the test using make
RUN make

ENTRYPOINT ["/usr/sbin/init"]
CMD dbus-daemon --system --nofork