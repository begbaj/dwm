#!/bin/bash
autowall 2 Beach
picom --vsync --experimental-backends -m 1 -f -c --blur-strength 5 --xrender-sync-fence --no-ewmh-fullscreen -b
setsid scripts/statusbar/status.sh >/dev/null 2>&1 < /dev/null &
