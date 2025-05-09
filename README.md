# cs553-c-container

A C-based container system mimicking the functionalities of Linux's native `unshare` command.

# How to Run
Meant to be run on the cs553 course VM. Simply run
```
make clean all
```
which will generate the execuable files `ccontainer` and `bench` for you. Run the container as follows:
```
./ccontainer
```
Running `./bench` will launch a container that exits immediately and prints some internal benchmarks.
# Dependencies
Requires [Docker Engine](https://docs.docker.com/engine/install/ubuntu/) to be installed on the course VM. As per the documentation, Docker can be installed on the course VM using the following series of commands:
```bash
for pkg in docker.io docker-doc docker-compose docker-compose-v2 podman-docker containerd runc; do sudo apt-get remove $pkg; done
# Add Docker's official GPG key:
sudo apt-get update
sudo apt-get install ca-certificates curl
sudo install -m 0755 -d /etc/apt/keyrings
sudo curl -fsSL https://download.docker.com/linux/ubuntu/gpg -o /etc/apt/keyrings/docker.asc
sudo chmod a+r /etc/apt/keyrings/docker.asc

# Add the repository to Apt sources:
echo \
  "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.asc] https://download.docker.com/linux/ubuntu \
  $(. /etc/os-release && echo "${UBUNTU_CODENAME:-$VERSION_CODENAME}") stable" | \
  sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
sudo apt-get update
sudo apt-get install docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin
```
## Benchmarking Dependencies
Generating graphs was done via the jupyter notebook `
<!-- Requires [gperftools](https://github.com/gperftools/gperftools) for profiling. 
After installing (via GitHub or by downloading the latest release), run `./configure` followed by `make && sudo make install` to generate the library files (takes a long time). Additionally, `golang` and `pprof` are required to make sense of the generated data.
```bash
./configure
make && sudo make install
sudo apt-get -yq golang
go install github.com/google/pprof@latest
```

Requires `iperf3` for network profiling, which can be installed using `apt-get`. -->