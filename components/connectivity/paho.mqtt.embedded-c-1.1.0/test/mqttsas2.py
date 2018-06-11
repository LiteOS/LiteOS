"""
*******************************************************************
  Copyright (c) 2013, 2017 IBM Corp.

  All rights reserved. This program and the accompanying materials
  are made available under the terms of the Eclipse Public License v1.0
  and Eclipse Distribution License v1.0 which accompany this distribution.

  The Eclipse Public License is available at
     http://www.eclipse.org/legal/epl-v10.html
  and the Eclipse Distribution License is available at
    http://www.eclipse.org/org/documents/edl-v10.php.

  Contributors:
     Ian Craggs - initial implementation and/or documentation
*******************************************************************
"""
from __future__ import print_function

# Trace MQTT traffic
import MQTTV3112 as MQTTV3

import socket, sys, select, traceback, datetime, os
import SocketServer as socketserver

logging = True
myWindow = None


def timestamp():
  now = datetime.datetime.now()
  return now.strftime('%Y%m%d %H%M%S')+str(float("."+str(now.microsecond)))[1:]


class MyHandler(socketserver.StreamRequestHandler):

  def handle(self):
    if not hasattr(self, "ids"):
      self.ids = {}
    if not hasattr(self, "versions"):
      self.versions = {}
    inbuf = True
    i = o = e = None
    try:
      clients = self.request
      brokers = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
      brokers.connect((brokerhost, brokerport))
      terminated = False
      while inbuf != None and not terminated:
        (i, o, e) = select.select([clients, brokers], [], [])
        for s in i:
          if s == clients:
            inbuf = MQTTV3.getPacket(clients) # get one packet
            if inbuf == None:
              break
            try:
              packet = MQTTV3.unpackPacket(inbuf)
              if packet.fh.MessageType == MQTTV3.PUBLISH and \
                  packet.topicName == "MQTTSAS topic" and \
                  packet.data == b"TERMINATE":
                print("Terminating client", self.ids[id(clients)])
                brokers.close()
                clients.close()
                terminated = True
                break
              elif packet.fh.MessageType == MQTTV3.CONNECT:
                self.ids[id(clients)] = packet.ClientIdentifier
                self.versions[id(clients)] = 3
              print(timestamp() , "C to S", self.ids[id(clients)], repr(packet))
              #print([hex(b) for b in inbuf])
              #print(inbuf)
            except:
              traceback.print_exc()
            brokers.send(inbuf)       # pass it on
          elif s == brokers:
            inbuf = MQTTV3.getPacket(brokers) # get one packet
            if inbuf == None:
              break
            try:
              print(timestamp(), "S to C", self.ids[id(clients)], repr(MQTTV3.unpackPacket(inbuf)))
            except:
              traceback.print_exc()
            clients.send(inbuf)
      print(timestamp()+" client "+self.ids[id(clients)]+" connection closing")
    except:
      print(repr((i, o, e)), repr(inbuf))
      traceback.print_exc()
    if id(clients) in self.ids.keys():
      del self.ids[id(clients)]
    elif id(clients) in self.versions.keys():
      del self.versions[id(clients)]

class ThreadingTCPServer(socketserver.ThreadingMixIn, socketserver.TCPServer):
  pass

def run():
  global brokerhost, brokerport
  myhost = '127.0.0.1'
  if len(sys.argv) > 1:
    brokerhost = sys.argv[1]
  else:
    brokerhost = '127.0.0.1'

  if len(sys.argv) > 2:
    brokerport = int(sys.argv[2])
  else:
    brokerport = 1883

  if len(sys.argv) > 3:
    myport = int(sys.argv[3])
  else:
    if brokerhost == myhost:
      myport = brokerport + 1
    else:
      myport = 1883

  print("Listening on port", str(myport)+", broker on port", brokerport)
  s = ThreadingTCPServer(("127.0.0.1", myport), MyHandler)
  s.serve_forever()

if __name__ == "__main__":
  run()
