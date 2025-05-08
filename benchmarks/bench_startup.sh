#! /bin/bash
cd ..
make clean all
for i in {1..10}; do
    { time ./bench; } 2>> benchmarks/results/tmp.txt
done
cd benchmarks/results
grep image < tmp.txt > ccontainer_startup_internal.txt
grep -v image < tmp.txt > ccontainer_startup.txt
rm tmp.txt

for i in {1..10}; do
    { time docker run 'docker-alpine'; } 2>> docker_startup.txt
done
cd ..