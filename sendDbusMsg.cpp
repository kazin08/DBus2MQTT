#include <iostream>
#include <string.h>
#include "dbus2mqtt.h"

int main(int argc, char* argv[]) {
    // Messagee variable
    char* msg = "no msg";

    if (argc < 2 || strcmp(argv[1], "send") != 0) {
      std::cout << "Syntax: dbus send [<message>]" << std::endl;
      return 1;
    } else {
        msg = argv[2];
        sendsignal(msg);
    }

    return 0;
}