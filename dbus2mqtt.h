// DBus get message
std::string getDbusMessage ();
void listen();
void query(char* param);
void sendsignal(char* sigvalue);
int async_publish(std::string msg);