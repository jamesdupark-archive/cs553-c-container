#! /bin/bash

# This one doesn't work as a script, but typing out this series of commands
# or copy/pasting to the terminal will

#### run from the project root ####
cd .. 
make clean all
# sudo ./bench
sudo ./ccontainer
# sleep 1

#### switch to new terminal ####
{ ps -C ccontainer -o pid,comm,%cpu,%mem; } > benchmarks/results/ccontainer_res.txt

#### now switch back to ccontainer terminal and close container ####


# kill %%
# sudo ./bench

sudo docker run -it 'docker-alpine'

#### switch to new terminal ####
{ ps -C docker -o pid,comm,%cpu,%mem; } > benchmarks/results/docker_res.txt

#### now switch back to docker terminal and close container ####

# kill -SIGKILL %%