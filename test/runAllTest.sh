#!/bin/bash

ifconfig bri0 down
brctl delbr bri0
echo "0" > /proc/sys/net/ipv4/ip_forward
iptables -D POSTROUTING -t nat -s 192.168.0.0/24 -j MASQUERADE
iptables -D POSTROUTING -t nat PREROUTING  -t nat -p udp --dport 53 -d 192.168.0.254 -j DNAT --to 10.9.128.16
rm /tmp/devicePluggedList.dat
rm /tmp/iptEnvStartup
rm /tmp/rndisList.tmp
rm /etc/udev/rules.d/75-persistent-usbnet.rules

clear
for file in Test*
do
    ./$file
done
