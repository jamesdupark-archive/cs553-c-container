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
Generating graphs was done via the jupyter notebook `graphing.ipynb`. Since I was SSH'ing into ilab/the course VM, I decided to run `jupyter notebook` locally for conveience' sake. The instructions in the notebook can be followed to generate new graphs from new data if necessary. Here are the packages needed for the notebook to run:
```
matplotlib
numpy
pandas
re
```
<!-- Requires [gperftools](https://github.com/gperftools/gperftools) for profiling. 
After installing (via GitHub or by downloading the latest release), run `./configure` followed by `make && sudo make install` to generate the library files (takes a long time). Additionally, `golang` and `pprof` are required to make sense of the generated data.
```bash
./configure
make && sudo make install
sudo apt-get -yq golang
go install github.com/google/pprof@latest
```

Requires `iperf3` for network profiling, which can be installed using `apt-get`. -->

# Benchmarking
It is **IMPERATIVE** that benchmarks be run in a specific order, as some of them may interfere with the performance of the following (particularly, `imgpull.sh` must be run before running anything else).
> There is a bug in the course VM(?) where deleting the `alpine` directory will cause it
to continue to persist in some ghost state until the VM is restarted, simply restart the VM
if there is trouble or if `ccontainer` or `bench` have been run before `imgpull.sh` has been run.

The recommended order is:
1. `imgpull.sh`
2. `bench_startup.sh`
3. `net.sh`
4. `disk.sh`
5. `resource.sh`

Note that although the benchmarks are provided as shell scripts for conveinence of recording the necessary instructions,
not every benchmark can be run simply by executing the script through - some require some level of setup beforehand,
while others require you to run specific commands within the container (which doesn't play well with shell scripting).
These instances are noted in their respective files.

**All benchmarks should be run from the `benchmarks` folder with `sudo`**
## `imgpull.sh`
This one **MUST** be run first, and can be run as-is.
## `bench_startup.sh`
This one can also be run as-is and **MUST** be run after `imgpull.sh`. An additional file `get_times.sh` has been provided
for convenience of extracting the recorded runtimes, as `time` reports multiple different types of time (real/user/kernel)

Times can be separated from their respective files by running the `get_times.sh` script, then copy-pasting the results to their respective variables as multiline strings. `get_times.sh` has the following syntax:
```
./get_times.sh <path_to_file> <key1> ... <keyn>
```
where `path_to_file` is the path from the current directory to the target file (e.g. `results/docker_startup.txt`) and `key1` through `keyn` are the keys on which to separate out lines (e.g. `real`, `user`, `sys`)
## `net.sh`
This one requires starting a `docker` container (using `sudo run-docker-container.sh`) and an instance of `ccontainer` in separate terminals from the one being used to benchmark. After both have been set up, the script can be run as-is.
## `disk.sh`
This one requires starting a `docker` container (using `sudo run-docker-container.sh`) and an instance of `ccontainer` in separate terminals. The script features three groups of commands: one to run natively, one to run in `ccontainer`, and one to run in the Docker container. The files can be extracted from their containers by using `cp` from the `alpine` directory (for `ccontainer`) or `sudo docker cp` from the Docker container.
## `resource.sh`
This one requires starting a `docker` container (using `sudo run-docker-container.sh`) and an instance of `ccontainer` in separate terminals from the one being used to benchmark. After setting each one up, the appropriate `ps` command can be run from the host terminal (see the comments in the script).