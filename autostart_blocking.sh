#!/bin/bash
autowall -q "archlinux"
picom --vsync --experimental-backends -m 1 -f -c --blur-strength 5 --xrender-sync-fence --no-ewmh-fullscreen -b
setsid scripts/statusbar/status.sh &
