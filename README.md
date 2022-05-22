Collection of personal dot files for ease of install across devices.


# Config
Collection of dot files. All application specific configuration is intended to be under here (except in cases where applications are patched)

# bin

Collection of special use executables. Basically they are many small wrappers of convince software that aren't big enough to be their own projects. They are generally written in c or POSIX shell and have very few dependencies if any

Some highlights:
* [attach](bin/attach) -- wrapper around abacdo
* [brownnoise](bin/brownnoise)/[pinknoise](bin/pinknoise) -- noise generator
* [delay](bin/delay) -- sleep some specified time then run specified command
* [highlight](bin/highlight) -- highlight selected text
* [kiss-setup.sh](bin/kiss-setup.sh) -- sets up system wide kiss repros
* [led](bin/led) -- activate leds in a priority queue
* [link_dot_files.sh](bin/link_dot_files.sh) -- links the files in Config/ to ~/.config/
* [nmenu](bin/nmenu) -- an interactive menu selection based on config files; intended as a primary UI for mobile devices with few buttons
* [screenshot](bin/screenshot) -- take a screenshot/video
* [status-bar](bin/status-bar) -- wrapper around dzen for the 2 status bars used
* [tz_select](bin/tz_select) -- manually select timezone
* [update-timezone](bin/update-timezone) -- update timezone based on region
* [vol](bin/vol) -- update volume and notify user
* [weather.sh](bin/weather.sh) -- get weather info and cache it
* [ytmenu](bin/ytmenu) -- search for youtube videos and plays the selected one
