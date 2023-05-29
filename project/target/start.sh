#!/bin/sh

# These commands need to be run at runtime (don't work during docker build)

# disable rsyslogd security percautions
sed -i "s/FileOwner syslog/FileOwner root/g" /etc/rsyslog.conf
sed -i "s/PrivDropToGroup syslog/PrivDropToGroup root/g" /etc/rsyslog.conf
sed -i "s/PrivDropToUser syslog/PrivDropToUser root/g" /etc/rsyslog.conf

sudo rsyslogd
service ssh start
service apache2 start

./target
