
//   0               1               2               3					bytes
//   0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7	bits
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  | / / / / / / / | / / / / / / / | / / / / / / / | / / / / / / / |
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  |  VER  |  CC   |             SIZE              | / / / / / / / |
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  |  0xF  |  0x1  |         CONFIG SIZE           | / / / / / / / |
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  |  0xF  |  0x2  |         CONFIG SIZE           | / / / / / / / |
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  |           MTU SIZE            |Link T |Band T | / / / / / / / |
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  |           APN Length          |       APN                    ~|
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  |           Username Length     |       Username               ~|
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  |           Password Length     |       Password               ~|
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  |           Host Length         |       Host Value Field       ~|
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  |           Userdata Length     |       Userdata               ~|
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  |  0xF  |  0x3  |         CONFIG SIZE           | / / / / / / / |
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  |D|E| T |   OL  |         LOG Buffer Size       | / / / / / / / |
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  |          Userdata Length      |           Userdata           ~|
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+


<?xml version="1.0"  encoding="utf-8"?>
<config>
  <item>
    <version>1.0</version>
    <cfgcnt>3</cfgcnt>
  </item>

  <item id="1">
  </item>

  <item id="2">
      <data name="Mtu">1024</data>
      <data name="Linktype">1</data>
	  <data name="Bandtype">1</data>
      <data name="APN"></data>
	  <data name="Username"></data>
      <data name="Password"></data>
	  <data name="Host">192.168.1.1</data>
	  <data name="Userdata"></data>
  </item>

  <item id="3">
      <data name="LogEnabled">1</data>
      <data name="LogExtOutput">1</data>
      <data name="LogOutputType">2</data>
	  <data name="LogBufferSize">200</data>
	  <data name="Userdata"></data>
  </item>


</config>