#!/bin/bash
CLASS_DIR=`pwd`
SOURCE3=RNDISController
SOURCE7=IptLog
SOURCE3_PATH=../ap/
SOURCE7_PATH=../ap/
UNITTEST=RNDISController_unittest
# ----------------------------------------------------------------------------------

g++ -I/lib/gtest-1.6/include -g -Wall -Wextra -c $CLASS_DIR/$SOURCE3_PATH$SOURCE3.cpp
g++ -I/lib/gtest-1.6/include -g -Wall -Wextra -c $CLASS_DIR/$SOURCE7_PATH$SOURCE7.cpp
g++ -I/lib/gtest-1.6/include -g -Wall -Wextra -c $CLASS_DIR/src/$UNITTEST.cpp

mkdir unittest
for file in *.o
do
  mv $file unittest
done

g++ -I/lib/gtest-1.6/include -g -Wall -Wextra -pthread\
 unittest/$SOURCE3.o\
 unittest/$SOURCE7.o\
 unittest/$UNITTEST.o\
 lib/gtest_main.a\
 -o unittest/$UNITTEST
unittest/$UNITTEST
rm $CLASS_DIR/unittest -rf 2>/tmp/null
# ---------------------------------------------------------------------------------------
