#include <iostream>

#include "dbus2mqtt.h"

int main() {
	std::string messageRcv = "null";

	messageRcv = getDbusMessage();
	std::cout << "getDbusMessage returned " << messageRcv << std::endl;

	if(messageRcv != "null") {
		return -1;
	}

	return 0;
}