#! /bin/bash

#### native disk benchmark ####
dd if=/dev/zero of=tmp bs=50M count=100 2> results/native_disk.txt


# these lines are not meant to be run in the terminal but rather in their respective
# containers

#### for ccontainer ####
rm /mnt/test
dd if=/dev/zero of=/mnt/test bs=50M count=100 2> /mnt/ccontainer_disk.txt
rm /mnt/test

#### for docker ####
rm /mnt/test
dd if=/dev/zero of=/mnt/test bs=50M count=100 2> /mnt/docker_disk.txt
rm /mnt/test