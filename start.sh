#!/bin/bash
rm server
g++ -o server server.cpp -lpthread
gcc -o client client.c
./server 10003 10
