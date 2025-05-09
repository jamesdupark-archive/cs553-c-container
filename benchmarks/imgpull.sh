#! /bin/bash

{ time { mkdir alpine; cd alpine; curl -o alpine.tar.gz \
https://dl-cdn.alpinelinux.org/alpine/v3.17/\
releases/x86_64/alpine-minirootfs-3.17.3-x86_64.tar.gz;
tar xvf alpine.tar.gz;
rm alpine.tar.gz; }; } 2> results/native_imgpull.txt
cd ..
rm -rf alpine


cd ..
make clean all
rm -rf alpine
{ time ./bench; } 2>> benchmarks/results/itmp.txt
cd benchmarks/results
grep image < itmp.txt > ccontainer_imgpull_internal.txt
grep -v image < itmp.txt > ccontainer_imgpull.txt
rm itmp.txt

cd ..
docker container prune -f
docker rmi docker-alpine
docker system prune -af
{ time docker build --tag 'docker-alpine' ..; } 2>> results/docker_imgpull.txt