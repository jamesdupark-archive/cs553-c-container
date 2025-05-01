#! /bin/bash
CONTAINER_PID=$(ps -C sh -o pid= | tail -1) # assuming no additional shell processes spawn
# echo $CONTAINER_PID
# echo $(ps -C sh -o pid=)
sudo mkdir /sys/fs/cgroup/553cgroup
cd /sys/fs/cgroup/553cgroup
# echo "100000" > memory.max
echo "100000" | sudo tee memory.max > /dev/null
# echo $CONTAINER_PID > cgroup.procs
echo $CONTAINER_PID | sudo tee cgroup.procs > /dev/null
# echo 2295 > cgroup.procs


# cd ~/ilab/cs553/cs553-2-webserver
# echo "" > cgroup.procs
# sudo rmdir /sys/fs/cgroup/553cgroup