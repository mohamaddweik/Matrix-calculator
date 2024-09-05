#!/bin/bash

gcc threads_calc.c -o run_threads -Wall -lpthread
./run_threads
