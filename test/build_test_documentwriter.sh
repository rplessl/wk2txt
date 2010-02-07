#!/bin/sh

g++ -c `xml2-config --cflags` -DDEBUG -o DocumentWriter.o DocumentWriter.cpp
g++ -c -o test_DocumentWriter.o test_DocumentWriter.cpp
g++ `xml2-config --libs` -o test_DocumentWriter test_DocumentWriter.o DocumentWriter.o
