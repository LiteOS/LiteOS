"""
*******************************************************************
  Copyright (c) 2013, 2014 IBM Corp.
 
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

"""

Assertions are used to validate incoming data, but are omitted from outgoing packets.  This is
so that the tests that use this package can send invalid data for error testing.

"""


import logging

logger = logging.getLogger("mqttsas")

# Low-level protocol interface

class MQTTException(Exception):
  pass
   

# Message types
CONNECT, CONNACK, PUBLISH, PUBACK, PUBREC, PUBREL, \
PUBCOMP, SUBSCRIBE, SUBACK, UNSUBSCRIBE, UNSUBACK, \
PINGREQ, PINGRESP, DISCONNECT = range(1, 15)

packetNames = [ "reserved", \
"Connect", "Connack", "Publish", "Puback", "Pubrec", "Pubrel", \
"Pubcomp", "Subscribe", "Suback", "Unsubscribe", "Unsuback", \
"Pingreq", "Pingresp", "Disconnect"]

classNames = [ "reserved", \
"Connects", "Connacks", "Publishes", "Pubacks", "Pubrecs", "Pubrels", \
"Pubcomps", "Subscribes", "Subacks", "Unsubscribes", "Unsubacks", \
"Pingreqs", "Pingresps", "Disconnects"]


def MessageType(byte):
  if byte != None:
    rc = ord(byte[0]) >> 4
  else:
    rc = None
  return rc


def getPacket(aSocket):
  "receive the next packet"
  buf = aSocket.recv(1) # get the first byte fixed header
  if buf == b"":
    return None
  if str(aSocket).find("[closed]") != -1:
    closed = True
  else:
    closed = False
  if closed:
    return None
  # now get the remaining length
  multiplier = 1
  remlength = 0
  while 1:
    next = aSocket.recv(1)
    while len(next) == 0:
      next = aSocket.recv(1)
    buf += next
    digit = ord(buf[-1])
    remlength += (digit & 127) * multiplier
    if digit & 128 == 0:
      break
    multiplier *= 128
  # receive the remaining length if there is any
  rest = ''
  if remlength > 0:
    while len(rest) < remlength:
      rest += aSocket.recv(remlength-len(rest))
  assert len(rest) == remlength
  return buf + rest


class FixedHeaders:

  def __init__(self, aMessageType):
    self.MessageType = aMessageType
    self.DUP = False
    self.QoS = 0
    self.RETAIN = False
    self.remainingLength = 0

  def __eq__(self, fh):
    return self.MessageType == fh.MessageType and \
           self.DUP == fh.DUP and \
           self.QoS == fh.QoS and \
           self.RETAIN == fh.RETAIN # and \
           # self.remainingLength == fh.remainingLength

  def __repr__(self):
    "return printable representation of our data"
    return classNames[self.MessageType]+'(DUP='+repr(self.DUP)+ \
           ", QoS="+repr(self.QoS)+", Retain="+repr(self.RETAIN)

  def pack(self, length):
    "pack data into string buffer ready for transmission down socket"
    buffer = bytes([(self.MessageType << 4) | (self.DUP << 3) |\
                         (self.QoS << 1) | self.RETAIN])
    self.remainingLength = length
    buffer += self.encode(length)
    return buffer

  def encode(self, x):
    assert 0 <= x <= 268435455
    buffer = b''
    while 1:
      digit = x % 128
      x //= 128
      if x > 0:
        digit |= 0x80
      buffer += bytes([digit])
      if x == 0:
        break
    return buffer

  def unpack(self, buffer):
    "unpack data from string buffer into separate fields"
    b0 = ord(buffer[0])
    self.MessageType = b0 >> 4
    self.DUP = ((b0 >> 3) & 0x01) == 1
    self.QoS = (b0 >> 1) & 0x03
    self.RETAIN = (b0 & 0x01) == 1
    (self.remainingLength, bytes) = self.decode(buffer[1:])
    return bytes + 1 # length of fixed header

  def decode(self, buffer):
    multiplier = 1
    value = 0
    bytes = 0
    while 1:
      bytes += 1
      digit = ord(buffer[0])
      buffer = buffer[1:]
      value += (digit & 127) * multiplier
      if digit & 128 == 0:
        break
      multiplier *= 128
    return (value, bytes)


def writeInt16(length):
  return bytes([length // 256, length % 256])

def readInt16(buf):
  return ord(buf[0])*256 + ord(buf[1])

def writeUTF(data):
  # data could be a string, or bytes.  If string, encode into bytes with utf-8
  return writeInt16(len(data)) + (data if type(data) == type(b"") else bytes(data, "utf-8"))

def readUTF(buffer, maxlen):
  if maxlen >= 2:
    length = readInt16(buffer)
  else:
    raise MQTTException("Not enough data to read string length")
  maxlen -= 2
  if length > maxlen:
    raise MQTTException("Length delimited string too long")
  buf = buffer[2:2+length].decode("utf-8")
  logger.info("[MQTT-4.7.3-2] topic names and filters not include null")
  zz = buf.find("\x00") # look for null in the UTF string
  if zz != -1:
    raise MQTTException("[MQTT-1.5.3-2] Null found in UTF data "+buf)
  """for c in range (0xD800, 0xDFFF):
    zz = buf.find(chr(c)) # look for D800-DFFF in the UTF string
    if zz != -1:
      raise MQTTException("[MQTT-1.5.3-1] D800-DFFF found in UTF data "+buf)
  """
  if buf.find("\uFEFF") != -1:
    logger.info("[MQTT-1.5.3-3] U+FEFF in UTF string") 
  return buf

def writeBytes(buffer):
  return writeInt16(len(buffer)) + buffer

def readBytes(buffer):
  length = readInt16(buffer)
  return buffer[2:2+length]


class Packets:

  def pack(self):
    buffer = self.fh.pack(0)
    return buffer

  def __repr__(self):
    return repr(self.fh)

  def __eq__(self, packet):
    return self.fh == packet.fh if packet else False


class Connects(Packets):

  def __init__(self, buffer = None):
    self.fh = FixedHeaders(CONNECT)

    # variable header
    self.ProtocolName = "MQTT"
    self.ProtocolVersion = 4
    self.CleanSession = True
    self.WillFlag = False
    self.WillQoS = 0
    self.WillRETAIN = 0
    self.KeepAliveTimer = 30
    self.usernameFlag = False
    self.passwordFlag = False

    # Payload
    self.ClientIdentifier = ""   # UTF-8
    self.WillTopic = None        # UTF-8
    self.WillMessage = None      # binary
    self.username = None         # UTF-8
    self.password = None         # binary

    if buffer != None:
      self.unpack(buffer)

  def pack(self):    
    connectFlags = bytes([(self.CleanSession << 1) | (self.WillFlag << 2) | \
                       (self.WillQoS << 3) | (self.WillRETAIN << 5) | \
                       (self.usernameFlag << 6) | (self.passwordFlag << 7)])
    buffer = writeUTF(self.ProtocolName) + bytes([self.ProtocolVersion]) + \
              connectFlags + writeInt16(self.KeepAliveTimer)
    buffer += writeUTF(self.ClientIdentifier) 
    if self.WillFlag:
      buffer += writeUTF(self.WillTopic) 
      buffer += writeBytes(self.WillMessage) 
    if self.usernameFlag:
      buffer += writeUTF(self.username) 
    if self.passwordFlag:
      buffer += writeBytes(self.password) 
    buffer = self.fh.pack(len(buffer)) + buffer
    return buffer

  def unpack(self, buffer):
    assert len(buffer) >= 2
    assert MessageType(buffer) == CONNECT

    try:
      fhlen = self.fh.unpack(buffer)
      packlen = fhlen + self.fh.remainingLength
      assert len(buffer) >= packlen, "buffer length %d packet length %d" % (len(buffer), packlen)
      curlen = fhlen # points to after header + remaining length
      assert self.fh.DUP == False, "[MQTT-2.1.2-1]"
      assert self.fh.QoS == 0, "[MQTT-2.1.2-1] QoS was not 0, was %d" % self.fh.QoS
      assert self.fh.RETAIN == False, "[MQTT-2.1.2-1]"

      self.ProtocolName = readUTF(buffer[curlen:], packlen - curlen)
      curlen += len(self.ProtocolName) + 2
      assert self.ProtocolName == "MQTT", "Wrong protocol name %s" % self.ProtocolName

      self.ProtocolVersion = ord(buffer[curlen])
      curlen += 1

      connectFlags = ord(buffer[curlen])
      assert (connectFlags & 0x01) == 0, "[MQTT-3.1.2-3] reserved connect flag must be 0"
      self.CleanSession = ((connectFlags >> 1) & 0x01) == 1
      self.WillFlag = ((connectFlags >> 2) & 0x01) == 1
      self.WillQoS = (connectFlags >> 3) & 0x03
      self.WillRETAIN = (connectFlags >> 5) & 0x01
      self.passwordFlag = ((connectFlags >> 6) & 0x01) == 1
      self.usernameFlag = ((connectFlags >> 7) & 0x01) == 1
      curlen +=1

      if self.WillFlag:
        assert self.WillQoS in [0, 1, 2], "[MQTT-3.1.2-14] will qos must not be 3"
      else:
        assert self.WillQoS == 0, "[MQTT-3.1.2-13] will qos must be 0, if will flag is false"
        assert self.WillRETAIN == False, "[MQTT-3.1.2-14] will retain must be false, if will flag is false"

      self.KeepAliveTimer = readInt16(buffer[curlen:])
      curlen += 2
      logger.info("[MQTT-3.1.3-3] Clientid must be present, and first field")
      logger.info("[MQTT-3.1.3-4] Clientid must be Unicode, and between 0 and 65535 bytes long")
      self.ClientIdentifier = readUTF(buffer[curlen:], packlen - curlen)
      curlen += len(self.ClientIdentifier) + 2

      if self.WillFlag:
        self.WillTopic = readUTF(buffer[curlen:], packlen - curlen)
        curlen += len(self.WillTopic) + 2
        self.WillMessage = readBytes(buffer[curlen:])
        curlen += len(self.WillMessage) + 2
        logger.info("[[MQTT-3.1.2-9] will topic and will message fields must be present")
      else:
        self.WillTopic = self.WillMessage = None

      if self.usernameFlag:
        assert len(buffer) > curlen+2, "Buffer too short to read username length"
        self.username = readUTF(buffer[curlen:], packlen - curlen)
        curlen += len(self.username) + 2
        logger.info("[MQTT-3.1.2-19] username must be in payload if user name flag is 1")
      else:
        logger.info("[MQTT-3.1.2-18] username must not be in payload if user name flag is 0")
        assert self.passwordFlag == False, "[MQTT-3.1.2-22] password flag must be 0 if username flag is 0"

      if self.passwordFlag:
        assert len(buffer) > curlen+2, "Buffer too short to read password length"
        self.password = readBytes(buffer[curlen:])
        curlen += len(self.password) + 2
        logger.info("[MQTT-3.1.2-21] password must be in payload if password flag is 0")
      else:
        logger.info("[MQTT-3.1.2-20] password must not be in payload if password flag is 0")

      if self.WillFlag and self.usernameFlag and self.passwordFlag:
        logger.info("[MQTT-3.1.3-1] clientid, will topic, will message, username and password all present")

      assert curlen == packlen, "Packet is wrong length curlen %d != packlen %d"
    except:
      logger.exception("[MQTT-3.1.4-1] server must validate connect packet and close connection without connack if it does not conform")
      raise



  def __repr__(self):
    buf = repr(self.fh)+", ProtocolName="+str(self.ProtocolName)+", ProtocolVersion=" +\
          repr(self.ProtocolVersion)+", CleanSession="+repr(self.CleanSession) +\
          ", WillFlag="+repr(self.WillFlag)+", KeepAliveTimer=" +\
          repr(self.KeepAliveTimer)+", ClientId="+str(self.ClientIdentifier) +\
          ", usernameFlag="+repr(self.usernameFlag)+", passwordFlag="+repr(self.passwordFlag)
    if self.WillFlag:
      buf += ", WillQoS=" + repr(self.WillQoS) +\
             ", WillRETAIN=" + repr(self.WillRETAIN) +\
             ", WillTopic='"+ self.WillTopic +\
             "', WillMessage='"+str(self.WillMessage)+"'"
    if self.username:
      buf += ", username="+self.username
    if self.password:
      buf += ", password="+str(self.password)
    return buf+")"

  def __eq__(self, packet):
    rc = Packets.__eq__(self, packet) and \
           self.ProtocolName == packet.ProtocolName and \
           self.ProtocolVersion == packet.ProtocolVersion and \
           self.CleanSession == packet.CleanSession and \
           self.WillFlag == packet.WillFlag and \
           self.KeepAliveTimer == packet.KeepAliveTimer and \
           self.ClientIdentifier == packet.ClientIdentifier and \
           self.WillFlag == packet.WillFlag
    if rc and self.WillFlag:
      rc = self.WillQoS == packet.WillQoS and \
           self.WillRETAIN == packet.WillRETAIN and \
           self.WillTopic == packet.WillTopic and \
           self.WillMessage == packet.WillMessage
    return rc


class Connacks(Packets):

  def __init__(self, buffer=None, DUP=False, QoS=0, Retain=False, ReturnCode=0):
    self.fh = FixedHeaders(CONNACK)
    self.fh.DUP = DUP
    self.fh.QoS = QoS
    self.fh.Retain = Retain
    self.flags = 0
    self.returnCode = ReturnCode
    if buffer != None:
      self.unpack(buffer)

  def pack(self):
    buffer = bytes([self.flags, self.returnCode])
    buffer = self.fh.pack(len(buffer)) + buffer
    return buffer

  def unpack(self, buffer):
    assert len(buffer) >= 4
    assert MessageType(buffer) == CONNACK
    self.fh.unpack(buffer)
    assert self.fh.remainingLength == 2, "Connack packet is wrong length %d" % self.fh.remainingLength
    assert ord(buffer[2]) in  [0, 1], "Connect Acknowledge Flags"
    self.returnCode = ord(buffer[3])
    assert self.fh.DUP == False, "[MQTT-2.1.2-1]"
    assert self.fh.QoS == 0, "[MQTT-2.1.2-1]"
    assert self.fh.RETAIN == False, "[MQTT-2.1.2-1]"

  def __repr__(self):
    return repr(self.fh)+", Session present="+str((self.flags & 0x01) == 1)+", ReturnCode="+repr(self.returnCode)+")"

  def __eq__(self, packet):
    return Packets.__eq__(self, packet) and \
           self.returnCode == packet.returnCode


class Disconnects(Packets):

  def __init__(self, buffer=None, DUP=False, QoS=0, Retain=False):
    self.fh = FixedHeaders(DISCONNECT)
    self.fh.DUP = DUP
    self.fh.QoS = QoS
    self.fh.Retain = Retain
    if buffer != None:
      self.unpack(buffer)

  def unpack(self, buffer):
    assert len(buffer) >= 2
    assert MessageType(buffer) == DISCONNECT
    self.fh.unpack(buffer)
    assert self.fh.remainingLength == 0, "Disconnect packet is wrong length %d" % self.fh.remainingLength
    logger.info("[MQTT-3.14.1-1] disconnect reserved bits must be 0")
    assert self.fh.DUP == False, "[MQTT-2.1.2-1]"
    assert self.fh.QoS == 0, "[MQTT-2.1.2-1]"
    assert self.fh.RETAIN == False, "[MQTT-2.1.2-1]"

  def __repr__(self):
    return repr(self.fh)+")"


class Publishes(Packets):

  def __init__(self, buffer=None, DUP=False, QoS=0, Retain=False, MsgId=0, TopicName="", Payload=b""):
    self.fh = FixedHeaders(PUBLISH)
    self.fh.DUP = DUP
    self.fh.QoS = QoS
    self.fh.Retain = Retain
    # variable header
    self.topicName = TopicName
    self.messageIdentifier = MsgId
    # payload
    self.data = Payload
    if buffer != None:
      self.unpack(buffer)

  def pack(self):
    buffer = writeUTF(self.topicName)
    if self.fh.QoS != 0:
      buffer +=  writeInt16(self.messageIdentifier)
    buffer += self.data
    buffer = self.fh.pack(len(buffer)) + buffer
    return buffer

  def unpack(self, buffer):
    assert len(buffer) >= 2
    assert MessageType(buffer) == PUBLISH
    fhlen = self.fh.unpack(buffer)
    assert self.fh.QoS in [0, 1, 2], "QoS in Publish must be 0, 1, or 2"
    packlen = fhlen + self.fh.remainingLength
    assert len(buffer) >= packlen
    curlen = fhlen
    try:
      self.topicName = readUTF(buffer[fhlen:], packlen - curlen)
    except UnicodeDecodeError:
      logger.info("[MQTT-3.3.2-1] topic name in publish must be utf-8")
      raise
    curlen += len(self.topicName) + 2
    if self.fh.QoS != 0:
      self.messageIdentifier = readInt16(buffer[curlen:])
      logger.info("[MQTT-2.3.1-1] packet indentifier must be in publish if QoS is 1 or 2")
      curlen += 2
      assert self.messageIdentifier > 0, "[MQTT-2.3.1-1] packet indentifier must be > 0"
    else:
      logger.info("[MQTT-2.3.1-5] no packet indentifier in publish if QoS is 0")
      self.messageIdentifier = 0
    self.data = buffer[curlen:fhlen + self.fh.remainingLength]
    if self.fh.QoS == 0:
      assert self.fh.DUP == False, "[MQTT-2.1.2-4]"
    return fhlen + self.fh.remainingLength

  def __repr__(self):
    rc = repr(self.fh)
    if self.fh.QoS != 0:
      rc += ", MsgId="+repr(self.messageIdentifier)
    rc += ", TopicName="+repr(self.topicName)+", Payload="+repr(self.data)+")"
    return rc

  def __eq__(self, packet):
    rc = Packets.__eq__(self, packet) and \
         self.topicName == packet.topicName and \
         self.data == packet.data
    if rc and self.fh.QoS != 0:
      rc = self.messageIdentifier == packet.messageIdentifier
    return rc


class Pubacks(Packets):

  def __init__(self, buffer=None, DUP=False, QoS=0, Retain=False, MsgId=0):
    self.fh = FixedHeaders(PUBACK)
    self.fh.DUP = DUP
    self.fh.QoS = QoS
    self.fh.Retain = Retain
    # variable header
    self.messageIdentifier = MsgId
    if buffer != None:
      self.unpack(buffer)

  def pack(self):
    buffer = writeInt16(self.messageIdentifier)
    buffer = self.fh.pack(len(buffer)) + buffer
    return buffer

  def unpack(self, buffer):
    assert len(buffer) >= 2
    assert MessageType(buffer) == PUBACK
    fhlen = self.fh.unpack(buffer)
    assert self.fh.remainingLength == 2, "Puback packet is wrong length %d" % self.fh.remainingLength
    assert len(buffer) >= fhlen + self.fh.remainingLength
    self.messageIdentifier = readInt16(buffer[fhlen:])
    assert self.fh.DUP == False, "[MQTT-2.1.2-1] Puback reserved bits must be 0"
    assert self.fh.QoS == 0, "[MQTT-2.1.2-1] Puback reserved bits must be 0"
    assert self.fh.RETAIN == False, "[MQTT-2.1.2-1] Puback reserved bits must be 0"
    return fhlen + 2

  def __repr__(self):
    return repr(self.fh)+", MsgId "+repr(self.messageIdentifier)

  def __eq__(self, packet):
    return Packets.__eq__(self, packet) and \
           self.messageIdentifier == packet.messageIdentifier


class Pubrecs(Packets):

  def __init__(self, buffer=None, DUP=False, QoS=0, Retain=False, MsgId=0):
    self.fh = FixedHeaders(PUBREC)
    self.fh.DUP = DUP
    self.fh.QoS = QoS
    self.fh.Retain = Retain
    # variable header
    self.messageIdentifier = MsgId
    if buffer != None:
      self.unpack(buffer)

  def pack(self):
    buffer = writeInt16(self.messageIdentifier)
    buffer = self.fh.pack(len(buffer)) + buffer
    return buffer

  def unpack(self, buffer):
    assert len(buffer) >= 2
    assert MessageType(buffer) == PUBREC
    fhlen = self.fh.unpack(buffer)
    assert self.fh.remainingLength == 2, "Pubrec packet is wrong length %d" % self.fh.remainingLength
    assert len(buffer) >= fhlen + self.fh.remainingLength
    self.messageIdentifier = readInt16(buffer[fhlen:])
    assert self.fh.DUP == False, "[MQTT-2.1.2-1] Pubrec reserved bits must be 0"
    assert self.fh.QoS == 0, "[MQTT-2.1.2-1] Pubrec reserved bits must be 0"
    assert self.fh.RETAIN == False, "[MQTT-2.1.2-1] Pubrec reserved bits must be 0"
    return fhlen + 2

  def __repr__(self):
    return repr(self.fh)+", MsgId="+repr(self.messageIdentifier)+")"

  def __eq__(self, packet):
    return Packets.__eq__(self, packet) and \
           self.messageIdentifier == packet.messageIdentifier


class Pubrels(Packets):

  def __init__(self, buffer=None, DUP=False, QoS=1, Retain=False, MsgId=0):
    self.fh = FixedHeaders(PUBREL)
    self.fh.DUP = DUP
    self.fh.QoS = QoS
    self.fh.Retain = Retain
    # variable header
    self.messageIdentifier = MsgId
    if buffer != None:
      self.unpack(buffer)

  def pack(self):
    buffer = writeInt16(self.messageIdentifier)
    buffer = self.fh.pack(len(buffer)) + buffer
    return buffer

  def unpack(self, buffer):
    assert len(buffer) >= 2
    assert MessageType(buffer) == PUBREL
    fhlen = self.fh.unpack(buffer)
    assert self.fh.remainingLength == 2, "Pubrel packet is wrong length %d" % self.fh.remainingLength
    assert len(buffer) >= fhlen + self.fh.remainingLength
    self.messageIdentifier = readInt16(buffer[fhlen:])
    assert self.fh.DUP == False, "[MQTT-2.1.2-1] DUP should be False in PUBREL"
    assert self.fh.QoS == 1, "[MQTT-2.1.2-1] QoS should be 1 in PUBREL"
    assert self.fh.RETAIN == False, "[MQTT-2.1.2-1] RETAIN should be False in PUBREL"
    logger.info("[MQTT-3.6.1-1] bits in fixed header for pubrel are ok")
    return fhlen + 2

  def __repr__(self):
    return repr(self.fh)+", MsgId="+repr(self.messageIdentifier)+")"

  def __eq__(self, packet):
    return Packets.__eq__(self, packet) and \
           self.messageIdentifier == packet.messageIdentifier


class Pubcomps(Packets):

  def __init__(self, buffer=None, DUP=False, QoS=0, Retain=False, MsgId=0):
    self.fh = FixedHeaders(PUBCOMP)
    self.fh.DUP = DUP
    self.fh.QoS = QoS
    self.fh.Retain = Retain
    # variable header
    self.messageIdentifier = MsgId
    if buffer != None:
      self.unpack(buffer)

  def pack(self):
    buffer = writeInt16(self.messageIdentifier)
    buffer = self.fh.pack(len(buffer)) + buffer
    return buffer

  def unpack(self, buffer):
    assert len(buffer) >= 2
    assert MessageType(buffer) == PUBCOMP
    fhlen = self.fh.unpack(buffer)
    assert len(buffer) >= fhlen + self.fh.remainingLength
    assert self.fh.remainingLength == 2, "Pubcomp packet is wrong length %d" % self.fh.remainingLength
    self.messageIdentifier = readInt16(buffer[fhlen:])
    assert self.fh.DUP == False, "[MQTT-2.1.2-1] DUP should be False in Pubcomp"
    assert self.fh.QoS == 0, "[MQTT-2.1.2-1] QoS should be 0 in Pubcomp"
    assert self.fh.RETAIN == False, "[MQTT-2.1.2-1] Retain should be false in Pubcomp"
    return fhlen + 2

  def __repr__(self):
    return repr(self.fh)+", MsgId="+repr(self.messageIdentifier)+")"

  def __eq__(self, packet):
    return Packets.__eq__(self, packet) and \
           self.messageIdentifier == packet.messageIdentifier


class Subscribes(Packets):

  def __init__(self, buffer=None, DUP=False, QoS=1, Retain=False, MsgId=0, Data=[]):
    self.fh = FixedHeaders(SUBSCRIBE)
    self.fh.DUP = DUP
    self.fh.QoS = QoS
    self.fh.Retain = Retain
    # variable header
    self.messageIdentifier = MsgId
    # payload - list of topic, qos pairs
    self.data = Data[:]
    if buffer != None:
      self.unpack(buffer)

  def pack(self):
    buffer = writeInt16(self.messageIdentifier)
    for d in self.data:
      buffer += writeUTF(d[0]) + bytes([d[1]])
    buffer = self.fh.pack(len(buffer)) + buffer
    return buffer

  def unpack(self, buffer):
    assert len(buffer) >= 2
    assert MessageType(buffer) == SUBSCRIBE
    fhlen = self.fh.unpack(buffer)
    assert len(buffer) >= fhlen + self.fh.remainingLength
    logger.info("[MQTT-2.3.1-1] packet indentifier must be in subscribe")
    self.messageIdentifier = readInt16(buffer[fhlen:])
    assert self.messageIdentifier > 0, "[MQTT-2.3.1-1] packet indentifier must be > 0"
    leftlen = self.fh.remainingLength - 2
    self.data = []
    while leftlen > 0:
      topic = readUTF(buffer[-leftlen:], leftlen)
      leftlen -= len(topic) + 2
      qos = ord(buffer[-leftlen])
      assert qos in [0, 1, 2], "[MQTT-3-8.3-2] reserved bits must be zero"
      leftlen -= 1
      self.data.append((topic, qos))
    assert len(self.data) > 0, "[MQTT-3.8.3-1] at least one topic, qos pair must be in subscribe"
    assert leftlen == 0
    assert self.fh.DUP == False, "[MQTT-2.1.2-1] DUP must be false in subscribe"
    assert self.fh.QoS == 1, "[MQTT-2.1.2-1] QoS must be 1 in subscribe"
    assert self.fh.RETAIN == False, "[MQTT-2.1.2-1] RETAIN must be false in subscribe"
    return fhlen + self.fh.remainingLength

  def __repr__(self):
    return repr(self.fh)+", MsgId="+repr(self.messageIdentifier)+\
           ", Data="+repr(self.data)+")"

  def __eq__(self, packet):
    return Packets.__eq__(self, packet) and \
           self.messageIdentifier == packet.messageIdentifier and \
           self.data == packet.data


class Subacks(Packets):

  def __init__(self, buffer=None, DUP=False, QoS=0, Retain=False, MsgId=0, Data=[]):
    self.fh = FixedHeaders(SUBACK)
    self.fh.DUP = DUP
    self.fh.QoS = QoS
    self.fh.Retain = Retain
    # variable header
    self.messageIdentifier = MsgId
    # payload - list of qos
    self.data = Data[:]
    if buffer != None:
      self.unpack(buffer)

  def pack(self):
    buffer = writeInt16(self.messageIdentifier)
    for d in self.data:
      buffer += bytes([d])
    buffer = self.fh.pack(len(buffer)) + buffer
    return buffer

  def unpack(self, buffer):
    assert len(buffer) >= 2
    assert MessageType(buffer) == SUBACK
    fhlen = self.fh.unpack(buffer)
    assert len(buffer) >= fhlen + self.fh.remainingLength
    self.messageIdentifier = readInt16(buffer[fhlen:])
    leftlen = self.fh.remainingLength - 2
    self.data = []
    while leftlen > 0:
      qos = buffer[-leftlen]
      assert ord(qos) in [0, 1, 2, 0x80], "[MQTT-3.9.3-2] return code in QoS must be 0, 1, 2 or 0x80, was "+ord(qos)
      leftlen -= 1
      self.data.append(qos)
    assert leftlen == 0
    assert self.fh.DUP == False, "[MQTT-2.1.2-1] DUP should be false in suback"
    assert self.fh.QoS == 0, "[MQTT-2.1.2-1] QoS should be 0 in suback"
    assert self.fh.RETAIN == False, "[MQTT-2.1.2-1] Retain should be false in suback"
    return fhlen + self.fh.remainingLength

  def __repr__(self):
    return repr(self.fh)+", MsgId="+repr(self.messageIdentifier)+\
           ", Data="+repr(self.data)+")"

  def __eq__(self, packet):
    return Packets.__eq__(self, packet) and \
           self.messageIdentifier == packet.messageIdentifier and \
           self.data == packet.data


class Unsubscribes(Packets):

  def __init__(self, buffer=None, DUP=False, QoS=1, Retain=False, MsgId=0, Data=[]):
    self.fh = FixedHeaders(UNSUBSCRIBE)
    self.fh.DUP = DUP
    self.fh.QoS = QoS
    self.fh.Retain = Retain
    # variable header
    self.messageIdentifier = MsgId
    # payload - list of topics
    self.data = Data[:]
    if buffer != None:
      self.unpack(buffer)

  def pack(self):
    buffer = writeInt16(self.messageIdentifier)
    for d in self.data:
      buffer += writeUTF(d)
    buffer = self.fh.pack(len(buffer)) + buffer
    return buffer

  def unpack(self, buffer):
    assert len(buffer) >= 2
    assert MessageType(buffer) == UNSUBSCRIBE
    fhlen = self.fh.unpack(buffer)
    assert len(buffer) >= fhlen + self.fh.remainingLength
    logger.info("[MQTT-2.3.1-1] packet indentifier must be in unsubscribe")
    self.messageIdentifier = readInt16(buffer[fhlen:])
    assert self.messageIdentifier > 0, "[MQTT-2.3.1-1] packet indentifier must be > 0"
    leftlen = self.fh.remainingLength - 2
    self.data = []
    while leftlen > 0:
      topic = readUTF(buffer[-leftlen:], leftlen)
      leftlen -= len(topic) + 2
      self.data.append(topic)
    assert leftlen == 0
    assert self.fh.DUP == False, "[MQTT-2.1.2-1]"
    assert self.fh.QoS == 1, "[MQTT-2.1.2-1]"
    assert self.fh.RETAIN == False, "[MQTT-2.1.2-1]"
    logger.info("[MQTT-3-10.1-1] fixed header bits are 0,0,1,0")
    return fhlen + self.fh.remainingLength

  def __repr__(self):
    return repr(self.fh)+", MsgId="+repr(self.messageIdentifier)+\
           ", Data="+repr(self.data)+")"

  def __eq__(self, packet):
    return Packets.__eq__(self, packet) and \
           self.messageIdentifier == packet.messageIdentifier and \
           self.data == packet.data


class Unsubacks(Packets):

  def __init__(self, buffer=None, DUP=False, QoS=0, Retain=False, MsgId=0):
    self.fh = FixedHeaders(UNSUBACK)
    self.fh.DUP = DUP
    self.fh.QoS = QoS
    self.fh.Retain = Retain
    # variable header
    self.messageIdentifier = MsgId
    if buffer != None:
      self.unpack(buffer)

  def pack(self):
    buffer = writeInt16(self.messageIdentifier)
    buffer = self.fh.pack(len(buffer)) + buffer
    return buffer

  def unpack(self, buffer):
    assert len(buffer) >= 2
    assert MessageType(buffer) == UNSUBACK
    fhlen = self.fh.unpack(buffer)
    assert len(buffer) >= fhlen + self.fh.remainingLength
    self.messageIdentifier = readInt16(buffer[fhlen:])
    assert self.messageIdentifier > 0, "[MQTT-2.3.1-1] packet indentifier must be > 0"
    self.messageIdentifier = readInt16(buffer[fhlen:])
    assert self.fh.DUP == False, "[MQTT-2.1.2-1]"
    assert self.fh.QoS == 0, "[MQTT-2.1.2-1]"
    assert self.fh.RETAIN == False, "[MQTT-2.1.2-1]"
    return fhlen + self.fh.remainingLength

  def __repr__(self):
    return repr(self.fh)+", MsgId="+repr(self.messageIdentifier)+")"

  def __eq__(self, packet):
    return Packets.__eq__(self, packet) and \
           self.messageIdentifier == packet.messageIdentifier


class Pingreqs(Packets):

  def __init__(self, buffer=None, DUP=False, QoS=0, Retain=False):
    self.fh = FixedHeaders(PINGREQ)
    self.fh.DUP = DUP
    self.fh.QoS = QoS
    self.fh.Retain = Retain
    if buffer != None:
      self.unpack(buffer)

  def unpack(self, buffer):
    assert len(buffer) >= 2
    assert MessageType(buffer) == PINGREQ
    fhlen = self.fh.unpack(buffer)
    assert self.fh.remainingLength == 0
    assert self.fh.DUP == False, "[MQTT-2.1.2-1]"
    assert self.fh.QoS == 0, "[MQTT-2.1.2-1]"
    assert self.fh.RETAIN == False, "[MQTT-2.1.2-1]"
    return fhlen

  def __repr__(self):
    return repr(self.fh)+")"


class Pingresps(Packets):

  def __init__(self, buffer=None, DUP=False, QoS=0, Retain=False):
    self.fh = FixedHeaders(PINGRESP)
    self.fh.DUP = DUP
    self.fh.QoS = QoS
    self.fh.Retain = Retain
    if buffer != None:
      self.unpack(buffer)

  def unpack(self, buffer):
    assert len(buffer) >= 2
    assert MessageType(buffer) == PINGRESP
    fhlen = self.fh.unpack(buffer)
    assert self.fh.remainingLength == 0
    assert self.fh.DUP == False, "[MQTT-2.1.2-1]"
    assert self.fh.QoS == 0, "[MQTT-2.1.2-1]"
    assert self.fh.RETAIN == False, "[MQTT-2.1.2-1]"
    return fhlen

  def __repr__(self):
    return repr(self.fh)+")"

classes = [None, Connects, Connacks, Publishes, Pubacks, Pubrecs,
           Pubrels, Pubcomps, Subscribes, Subacks, Unsubscribes,
           Unsubacks, Pingreqs, Pingresps, Disconnects]

def unpackPacket(buffer):
  if MessageType(buffer) != None:
    packet = classes[MessageType(buffer)]()
    packet.unpack(buffer)
  else:
    packet = None
  return packet

if __name__ == "__main__":
  fh = FixedHeaders(CONNECT)
  tests = [0, 56, 127, 128, 8888, 16383, 16384, 65535, 2097151, 2097152,
           20555666, 268435454, 268435455]
  for x in tests:
    try:
      assert x == fh.decode(fh.encode(x))[0]
    except AssertionError:
      print("Test failed for x =", x, fh.decode(fh.encode(x)))
  try:
    fh.decode(fh.encode(268435456))
    print("Error")
  except AssertionError:
    pass

  for packet in classes[1:]:
    before = str(packet())   
    after = str(unpackPacket(packet().pack()))
    try:
      assert before == after
    except:
      print("before:", before, "\nafter:", after)
  print("End")

