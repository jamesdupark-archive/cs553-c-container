#! /bin/bash

# This one doesn't work as a script, but typing out this series of commands
# or copy/pasting to the terminal will

# cd .. # run from the project root
make clean all
sudo ./bench
sudo ./ccontainer &
sleep 1
{ ps -C ccontainer -o pid,comm,%cpu,%mem; } > benchmarks/results/ccontainer_res.txt
kill %%
sudo ./bench

sudo docker run -it 'docker-alpine' &
sleep 1
{ ps -C docker -o pid,comm,%cpu,%mem; } > benchmarks/results/docker_res.txt
kill -SIGKILL %%