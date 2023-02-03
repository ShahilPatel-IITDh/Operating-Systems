#!/bin/sh
./syscall.sh &
./pipe.sh &
./syscall.sh &
./pipe.sh &
./syscall.sh &
wait