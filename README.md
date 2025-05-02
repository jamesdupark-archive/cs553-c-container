# cs553-c-container

A C-based container system mimicking the functionalities of Linux's native `unshare` command.

# How to Run
Meant to be run on the cs553 course VM. Simply run
```
make clean all
```
which will generate the execuable file `ccontainer` for you. Run the container as follows:
```
./ccontainer <flags>
```
The container executable accepts the following flags: