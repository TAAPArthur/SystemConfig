#!/bin/sh

{ cpm smart-show purestorage; echo "US - SV5, San Jose, CA"; read -r yubikey; echo "$yubikey"; } | sudo openconnect --prot=gp webvpn.purestorage.com
