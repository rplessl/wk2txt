#!/bin/sh

g++ -c -DDEBUG -o OptionParser.o OptionParser.cpp
g++ -c -o test_OptionParser.o test_OptionParser.cpp
g++ -o test_OptionParser test_OptionParser.o OptionParser.o
