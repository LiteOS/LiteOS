#*******************************************************************************
#  Copyright (c) 2017 IBM Corp.
#
#  All rights reserved. This program and the accompanying materials
#  are made available under the terms of the Eclipse Public License v1.0
#  and Eclipse Distribution License v1.0 which accompany this distribution.
#
#  The Eclipse Public License is available at
#     http://www.eclipse.org/legal/epl-v10.html
#  and the Eclipse Distribution License is available at
#    http://www.eclipse.org/org/documents/edl-v10.php.
#
#  Contributors:
#     Ian Craggs - initial version
#*******************************************************************************/

# Samples

add_executable(
  hello
  hello.cpp
)
target_include_directories(hello PRIVATE "../../src" "../../src/linux")
target_link_libraries(hello MQTTPacketClient)

add_executable(
  stdoutsub
  stdoutsub.cpp
)
target_include_directories(stdoutsub PRIVATE "../../src" "../../src/linux")
target_link_libraries(stdoutsub paho-embed-mqtt3c)
