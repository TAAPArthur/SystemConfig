#!/bin/sh

{ cpm smart-show purestorage; echo "US - Chicago, IL"; read -r yubikey; echo "$yubikey"; } | sudo openconnect --prot=gp webvpn.purestorage.com

