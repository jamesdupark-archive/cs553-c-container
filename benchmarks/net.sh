#! /bin/bash

# requires that you have each container set up and running on another terminal
# (e.g., using tmux) AND running
# docker container prune
# before launching the docker container

cd results
ping -f -i0.002 -w10 -q 10.0.0.5 > ccontainer_net.txt
ping -f -i0.002 -w10 -q 172.17.0.2 > docker_net.txt