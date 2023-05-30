#!/bin/bash

# Set the target number to your assigned target number
TARGET_NUM=1

TARGET_ADDR='mw-demo.roryhemmings.com'
TARGET_USERNAME='joebruin'
GATEWAY_PORT=$((($TARGET_NUM * 1000) + 1004))

ssh -l $TARGET_USERNAME -p $GATEWAY_PORT $TARGET_ADDR