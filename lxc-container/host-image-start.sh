#! /bin/bash
mkdir alpine
cd alpine
curl -o alpine.tar.gz \
https://dl-cdn.alpinelinux.org/alpine/v3.17/\
releases/x86_64/alpine-minirootfs-3.17.3-x86_64.tar.gz
tar xvf alpine.tar.gz
rm alpine.tar.gz
# sudo unshare -pfmnriUC --mount-proc=proc --kill-child chroot . sh
sudo unshare -pfmn --mount-proc=proc chroot . sh