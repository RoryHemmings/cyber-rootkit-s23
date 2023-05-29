#!/bin/bash

TARGET_ADDR='mw-demo.roryhemmings.com'
TARGET_USERNAME='joebruin'
GATEWAY_PORT=2004

# ssh -l $TARGET_USERNAME -p $GATEWAY_PORT $TARGET_ADDR -o PasswordAuthentication=no
ssh -l $TARGET_USERNAME -p $GATEWAY_PORT $TARGET_ADDR

echo "Backdoor Open"
