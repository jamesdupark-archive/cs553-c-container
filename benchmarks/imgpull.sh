#! /bin/bash

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