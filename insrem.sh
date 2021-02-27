#!/bin/bash

# clear the message queue, then
# insert seconds.ko into the kernal and remove it again, then
# dispaly the messages as a result of doing so

sudo -p "osc"
sudo dmesg -c
clear

printf "beginning script...\n\n"
sudo insmod jiffies.ko
cat /proc/jiffies
sleep 3
cat /proc/jiffies
sleep 2
cat /proc/jiffies
sleep 1
cat /proc/jiffies
sleep 4
sudo rmmod jiffies
sudo dmesg
printf "\n...end of script\n\n"

# Just run this script to run the functions in simple.c