# Linux Development Environment
This repository contains *one* workflow for building and modifying the Linux kernel.
It consists of two main components.
The first is a docker container that contains all the requirements to build the Linux kernel, as well
    as the requirements to run QEMU.
The second is a QEMU  script that boots a virtual machine running a custom version of the Linux kernel.
Using these together allows you to easily make and test changes to the Linux kernel without needing to
    manage all the packages locally.

***This repository is cloned and modified from rosalab/(unknown)-kernel***

#### Build Docker Container

``` make docker ```

#### Update git submodules
The `linux` directory contains a forked linux kernel source tree as a git submodule. The below commands help you to update it.

```sh
git submodule init

# This will take some time.
git submodule update
```

#### Copy config file to linux folder

``` cp linux-config/.config ./linux ```
Feel free to make changes to the config based on the usecase

#### Build linux

```
make vmlinux
```

#### Run Qemu
```
make qemu-run
```

#### If you want to ssh into the qemu
```
make qemu-ssh
```

#### If you want to enter the docker container where qemu is running
```
make enter-docker
```

#### If you want to debug the kernel using gdb

In an another terminal
```
cd linux
gdb vmlinux
target remote:1234
c
```
then set your breakpoints and debug more


## Adding Ports to QEMU
By default host port 52223 is connected to port 52223 inside the QEMU virtual machine.
If you need to be able to connect to more than one port (or a specific port) on your custom kernel from the host, you will have to add new rules.
The needed rules are in `q-script/yifei-q` and in the Makefile.

### Makefile modifications
You must add a line that maps a host port to a Docker port.
In the Makefile you must add a line 
    ```-p 127.0.0.1:HOST_PORT:DOCKER_PORT```
This will map the host port to the docker port.

### q-script modifications
You must modify the q-script to connect the DOCKER_PORT to a QEMU_PORT.
In the q-script you must append a new rule.
Find the line that starts with `"net += -netdev user..."`.
Then at the end of the line add the text ```"hostfwd=tcp::DOCKER_PORT-:QEMU_PORT"```

