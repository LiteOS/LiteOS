/*******************************************************************************
 * Copyright (c) 2014 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Ian Craggs - initial API and implementation and/or initial documentation
 *******************************************************************************/

#if !defined(MQTTSOCKET_H)
#define MQTTSOCKET_H

#include "MQTT_mbed.h"
#include "TCPSocketConnection.h"

class MQTTSocket
{
public:    
    int connect(char* hostname, int port, int timeout=1000)
    {
        mysock.set_blocking(false, timeout);    // 1 second Timeout 
        return mysock.connect(hostname, port);
    }

    int read(unsigned char* buffer, int len, int timeout)
    {
        mysock.set_blocking(false, timeout);  
        return mysock.receive((char*)buffer, len);
    }
    
    int write(unsigned char* buffer, int len, int timeout)
    {
        mysock.set_blocking(false, timeout);  
        return mysock.send((char*)buffer, len);
    }
    
    int disconnect()
    {
        return mysock.close();
    }
    
private:

    TCPSocketConnection mysock; 
    
};



#endif
