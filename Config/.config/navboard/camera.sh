#!/bin/sh


v4l2-ctl -l -d /dev/video2 | {
    while read -r name _ type _ vars; do
        echo "$name $type $vars"
    done
       #                      brightness 0x00980900 (int)    : min=0 max=255 step=1 default=128 value=128

}
