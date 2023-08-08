#!/bin/sh
# US - Chicago, IL
{ cpm smart-show purestorage; echo "US - C14, Sunnyvale, CA"; read -r yubikey; echo "$yubikey"; } | sudo openconnect --prot=gp webvpn.purestorage.com

