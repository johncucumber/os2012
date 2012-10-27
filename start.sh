#!/bin/bash
rm server
gcc -o client client.c
./server 10018 10
