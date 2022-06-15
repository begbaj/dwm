SCRIPTPATH="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
while true; do
    if [ ! -d "$SCRIPTPATH/status" ]; then
        mkdir "$SCRIPTPATH/status"
    fi

    masterVol=$(awk -F"[][]" '/Left: / { print $2 $3 $4 }' <(amixer -D pulse sget Master))
    dateTime=$(date "+%d/%m/%y %H:%M:%S")  

    cpu=$($SCRIPTPATH/procper )

    ramt=$(awk '/MemTotal/ {print $2}' <(cat /proc/meminfo))
    ramf=$(awk '/MemAvailable/ {print $2}' <(cat /proc/meminfo))
    ram=$(awk -v f=$ramf -v t=$ramt 'BEGIN {print ((t-f)/t)*100}')
    nordvpn=$(awk -F"Status: " '{ print $2}' <(nordvpn status))
    lip=$(awk -F"inet | brd" '/enp0s/ {print $2}' <(ip addr))

    prompt="LAN: ${lip:1:100} | VPN: ${nordvpn:1:100} | RAM : ${ram:0:5}% | CPU: ${cpu:0:5}% | 墳 : $masterVol | $dateTime | $USER"

    xsetroot -name "$prompt"
    sleep 1
done
