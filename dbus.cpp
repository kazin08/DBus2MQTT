#include <dbus/dbus.h>

#include <cstddef>
#include <cstdio>
#include <iostream>
#include <string>

#include "dbus2mqtt.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

std::string getDbusMessage () {
    DBusError dbus_error;
    DBusConnection * dbus_conn = nullptr;
    DBusMessage * dbus_msg = nullptr;
    DBusMessage * dbus_reply = nullptr;
    char * dbus_result = nullptr;
    std::string aux = "";

    // Initialize D-Bus error
    ::dbus_error_init(&dbus_error);

    // Connect to D-Bus
    if ( nullptr == (dbus_conn = ::dbus_bus_get(DBUS_BUS_SYSTEM, &dbus_error)) ) {
        ::perror(dbus_error.name);
        ::perror(dbus_error.message);

    // Compose remote procedure call
    } else if ( nullptr == (dbus_msg = ::dbus_message_new_method_call("org.freedesktop.DBus", "/", "org.freedesktop.DBus.Introspectable", "Introspect")) ) {
        ::dbus_connection_unref(dbus_conn);
        ::perror("ERROR: ::dbus_message_new_method_call - Unable to allocate memory for the message!");

    // Invoke remote procedure call, block for response
    } else if ( nullptr == (dbus_reply = ::dbus_connection_send_with_reply_and_block(dbus_conn, dbus_msg, DBUS_TIMEOUT_USE_DEFAULT, &dbus_error)) ) {
        ::dbus_message_unref(dbus_msg);
        ::dbus_connection_unref(dbus_conn);
        ::perror(dbus_error.name);
        ::perror(dbus_error.message);

    // Parse response
    } else if ( !::dbus_message_get_args(dbus_reply, &dbus_error, DBUS_TYPE_STRING, &dbus_result, DBUS_TYPE_INVALID) ) {
        ::dbus_message_unref(dbus_msg);
        ::dbus_message_unref(dbus_reply);
        ::dbus_connection_unref(dbus_conn);
        ::perror(dbus_error.name);
        ::perror(dbus_error.message);

    // Work with the results of the remote procedure call
    } else {
        
        aux = dbus_result;

        /*
         * Applications must not close shared connections -
         * see dbus_connection_close() docs. This is a bug in the application.
         */
        //::dbus_connection_close(dbus_conn);

        // When using the System Bus, unreference
        // the connection instead of closing it
        ::dbus_connection_unref(dbus_conn);
    }

    return aux;
}

/**
 * Connect to the DBUS bus and send a broadcast signal
 */
void sendsignal(char* sigvalue)
{
    DBusMessage* msg;
    DBusMessageIter args;
    DBusConnection* conn;
    DBusError err;
    int ret;
    dbus_uint32_t serial = 0;

    printf("Sending signal with value %s\n", sigvalue);

    // initialise the error value
    dbus_error_init(&err);

    // connect to the DBUS system bus, and check for errors
    conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
    if (dbus_error_is_set(&err)) { 
        fprintf(stderr, "Connection Error (%s)\n", err.message); 
        dbus_error_free(&err); 
    }
    if (NULL == conn) { 
        exit(1); 
    }

    // register our name on the bus, and check for errors
    ret = dbus_bus_request_name(conn, "test.signal.source", DBUS_NAME_FLAG_REPLACE_EXISTING , &err);
    if (dbus_error_is_set(&err)) { 
        fprintf(stderr, "Name Error (%s)\n", err.message); 
        dbus_error_free(&err); 
    }
    if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret) { 
        exit(1);
    }

    // create a signal & check for errors 
    msg = dbus_message_new_signal("/test/signal/Object", // object name of the signal
                                    "test.signal.Type", // interface name of the signal
                                    "Test"); // name of the signal
    if (NULL == msg) 
    { 
        fprintf(stderr, "Message Null\n"); 
        exit(1); 
    }

    // append arguments onto signal
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &sigvalue)) {
        fprintf(stderr, "Out Of Memory!\n"); 
        exit(1);
    }

    // send the message and flush the connection
    if (!dbus_connection_send(conn, msg, &serial)) {
        fprintf(stderr, "Out Of Memory!\n"); 
        exit(1);
    }
    dbus_connection_flush(conn);
    
    printf("Signal Sent\n");
    
    // free the message 
    dbus_message_unref(msg);
}

/**
 * Call a method on a remote object
 */
void query(char* param) 
{
    DBusMessage* msg;
    DBusMessageIter args;
    DBusConnection* conn;
    DBusError err;
    DBusPendingCall* pending;
    int ret;
    bool stat;
    dbus_uint32_t level;

    printf("Calling remote method with %s\n", param);

    // initialiset the errors
    dbus_error_init(&err);

    // connect to the system bus and check for errors
    conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
    if (dbus_error_is_set(&err)) { 
        fprintf(stderr, "Connection Error (%s)\n", err.message); 
        dbus_error_free(&err);
    }
    if (NULL == conn) { 
        exit(1); 
    }

    // request our name on the bus
    ret = dbus_bus_request_name(conn, "test.method.caller", DBUS_NAME_FLAG_REPLACE_EXISTING , &err);
    if (dbus_error_is_set(&err)) { 
        fprintf(stderr, "Name Error (%s)\n", err.message); 
        dbus_error_free(&err);
    }
    if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret) { 
        exit(1);
    }

    // create a new method call and check for errors
    msg = dbus_message_new_method_call("test.method.server", // target for the method call
                                        "/test/method/Object", // object to call on
                                        "test.method.Type", // interface to call on
                                        "Method"); // method name
    if (NULL == msg) { 
        fprintf(stderr, "Message Null\n");
        exit(1);
    }

    // append arguments
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &param)) {
        fprintf(stderr, "Out Of Memory!\n"); 
        exit(1);
    }
    
    // send message and get a handle for a reply
    if (!dbus_connection_send_with_reply (conn, msg, &pending, -1)) { // -1 is default timeout
        fprintf(stderr, "Out Of Memory!\n"); 
        exit(1);
    }
    if (NULL == pending) { 
        fprintf(stderr, "Pending Call Null\n"); 
        exit(1); 
    }
    dbus_connection_flush(conn);
    
    printf("Request Sent\n");
    
    // free message
    dbus_message_unref(msg);
    
    // block until we recieve a reply
    dbus_pending_call_block(pending);

    // get the reply message
    msg = dbus_pending_call_steal_reply(pending);
    if (NULL == msg) {
        fprintf(stderr, "Reply Null\n"); 
        exit(1); 
    }
    // free the pending message handle
    dbus_pending_call_unref(pending);

    // read the parameters
    if (!dbus_message_iter_init(msg, &args))
        fprintf(stderr, "Message has no arguments!\n"); 
    else if (DBUS_TYPE_BOOLEAN != dbus_message_iter_get_arg_type(&args)) 
        fprintf(stderr, "Argument is not boolean!\n"); 
    else
        dbus_message_iter_get_basic(&args, &stat);

    if (!dbus_message_iter_next(&args))
        fprintf(stderr, "Message has too few arguments!\n"); 
    else if (DBUS_TYPE_UINT32 != dbus_message_iter_get_arg_type(&args)) 
        fprintf(stderr, "Argument is not int!\n"); 
    else
        dbus_message_iter_get_basic(&args, &level);

    printf("Got Reply: %d, %d\n", stat, level);
    
    // free reply 
    dbus_message_unref(msg);   
}

void reply_to_method_call(DBusMessage* msg, DBusConnection* conn)
{
    DBusMessage* reply;
    DBusMessageIter args;
    bool stat = true;
    dbus_uint32_t level = 21614;
    dbus_uint32_t serial = 0;
    char* param = "";

    // read the arguments
    if (!dbus_message_iter_init(msg, &args))
        fprintf(stderr, "Message has no arguments!\n"); 
    else if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args)) 
        fprintf(stderr, "Argument is not string!\n"); 
    else 
        dbus_message_iter_get_basic(&args, &param);

    printf("Method called with %s\n", param);

    // create a reply from the message
    reply = dbus_message_new_method_return(msg);

    // add the arguments to the reply
    dbus_message_iter_init_append(reply, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_BOOLEAN, &stat)) { 
        fprintf(stderr, "Out Of Memory!\n"); 
        exit(1);
    }
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_UINT32, &level)) { 
        fprintf(stderr, "Out Of Memory!\n"); 
        exit(1);
    }

    // send the reply && flush the connection
    if (!dbus_connection_send(conn, reply, &serial)) {
        fprintf(stderr, "Out Of Memory!\n"); 
        exit(1);
    }
    dbus_connection_flush(conn);

    // free the reply
    dbus_message_unref(reply);
}

/**
 * Server that exposes a method call and waits for it to be called
 */
void listen() 
{
    DBusMessage* msg;
    DBusConnection* conn;
    DBusError err;
    int ret;

    printf("Listening for method calls\n");

    // initialise the error
    dbus_error_init(&err);
    
    // connect to the bus and check for errors
    conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
    if (dbus_error_is_set(&err)) { 
        fprintf(stderr, "Connection Error (%s)\n", err.message); 
        dbus_error_free(&err); 
    }
    if (NULL == conn) {
        fprintf(stderr, "Connection Null\n"); 
        exit(1); 
    }
    
    // request our name on the bus and check for errors
    ret = dbus_bus_request_name(conn, "test.method.server", DBUS_NAME_FLAG_REPLACE_EXISTING , &err);
    if (dbus_error_is_set(&err)) { 
        fprintf(stderr, "Name Error (%s)\n", err.message); 
        dbus_error_free(&err);
    }
    if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret) { 
        fprintf(stderr, "Not Primary Owner (%d)\n", ret);
        exit(1); 
    }

    // loop, testing for new messages
    while (true) {
        // non blocking read of the next available message
        dbus_connection_read_write(conn, 0);
        msg = dbus_connection_pop_message(conn);

        // loop again if we haven't got a message
        if (NULL == msg) { 
            continue; 
        }
        
        // check this is a method call for the right interface & method
        if (dbus_message_is_method_call(msg, "test.method.Type", "Method")) 
            reply_to_method_call(msg, conn);

        // free the message
        dbus_message_unref(msg);
    }

}