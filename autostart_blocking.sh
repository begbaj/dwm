#!/bin/bash
autowall 2 Beach
picom -b
setsid scripts/statusbar/status.sh >/dev/null 2>&1 < /dev/null &
