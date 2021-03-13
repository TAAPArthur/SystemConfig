#!/bin/sh

[ -z "$NOTIFICATION_ID" ] || export HERBE_ID=/$NOTIFICATION_ID
herbe "$*"
