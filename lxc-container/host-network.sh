#! /bin/bash
CONTAINER_PID=$(ps -C sh -o pid= | tail -1) # assuming no additional shell processes spawn
# echo "$CONTAINER_PID"
# echo $(ps -C sh -o pid=)
sudo ip link add host netns 1 type veth peer container netns "$CONTAINER_PID"
sudo ip addr add 10.0.0.9/24 dev host
sudo ip link set host up