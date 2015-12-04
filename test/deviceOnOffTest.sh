#!/bin/bash
CLASS_DIR=`pwd`
SOURCE1=IptFacade
SOURCE2=IPDispatcher
SOURCE3=RNDISController
SOURCE4=IPaddress
SOURCE5=AndroidDevice
SOURCE6=ExternalCommander
SOURCE7=IptLog
SOURCE8=Semaphore
SOURCE9=IptCommand

SOURCE1_PATH=../ap/
SOURCE2_PATH=../ap/
SOURCE3_PATH=../ap/
SOURCE4_PATH=../ap/
SOURCE5_PATH=../ap/
SOURCE6_PATH=../ap/
SOURCE7_PATH=../ap/
SOURCE8_PATH=../ap/
SOURCE9_PATH=../ap/
UNITTEST=DeviceOnOff_unittest
# ---------------------------------------------------------------------------------------
g++ -I/lib/gtest-1.6/include -g -Wall -Wextra -c $CLASS_DIR/$SOURCE1_PATH$SOURCE1.cpp
g++ -I/lib/gtest-1.6/include -g -Wall -Wextra -c $CLASS_DIR/$SOURCE2_PATH$SOURCE2.cpp
g++ -I/lib/gtest-1.6/include -g -Wall -Wextra -c $CLASS_DIR/$SOURCE3_PATH$SOURCE3.cpp
g++ -I/lib/gtest-1.6/include -g -Wall -Wextra -c $CLASS_DIR/$SOURCE4_PATH$SOURCE4.cpp
g++ -I/lib/gtest-1.6/include -g -Wall -Wextra -c $CLASS_DIR/$SOURCE5_PATH$SOURCE5.cpp
g++ -I/lib/gtest-1.6/include -g -Wall -Wextra -c $CLASS_DIR/$SOURCE6_PATH$SOURCE6.cpp
g++ -I/lib/gtest-1.6/include -g -Wall -Wextra -c $CLASS_DIR/$SOURCE7_PATH$SOURCE7.cpp
g++ -I/lib/gtest-1.6/include -g -Wall -Wextra -c $CLASS_DIR/$SOURCE8_PATH$SOURCE8.cpp
g++ -I/lib/gtest-1.6/include -g -Wall -Wextra -c $CLASS_DIR/$SOURCE9_PATH$SOURCE9.cpp
g++ -I/lib/gtest-1.6/include -g -Wall -Wextra -c $CLASS_DIR/src/$UNITTEST.cpp

mkdir unittest
for file in *.o
do
  mv $file unittest
done

g++ -I/lib/gtest-1.6/include -g -Wall -Wextra -pthread\
 unittest/$SOURCE1.o\
 unittest/$SOURCE2.o\
 unittest/$SOURCE3.o\
 unittest/$SOURCE4.o\
 unittest/$SOURCE5.o\
 unittest/$SOURCE6.o\
 unittest/$SOURCE7.o\
 unittest/$SOURCE8.o\
 unittest/$SOURCE9.o\
 unittest/$UNITTEST.o\
 lib/gtest_main.a\
 -o unittest/$UNITTEST
unittest/$UNITTEST
rm $CLASS_DIR/unittest -rf 2>/tmp/null
# ---------------------------------------------------------------------------------------
