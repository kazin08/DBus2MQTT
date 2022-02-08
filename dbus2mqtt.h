// DBus get message
std::string getDbusMessage ();
// void receive();
void listen();
void query(char* param);
void sendsignal(char* sigvalue);
int async_publish(std::string msg);