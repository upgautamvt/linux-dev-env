BASE_PROJ ?= $(shell pwd)
LINUX ?= ${BASE_PROJ}/linux
DOCKER_IMAGE ?= "runtime-dev"
SSH_PORT ?= "52222"
NET_PORT ?= "52223"
GDB_PORT ?= "1234"
.ALWAYS:

all: vmlinux 

docker: .ALWAYS
	docker buildx build --network=host --progress=plain -t ${DOCKER_IMAGE} .

qemu-run: 
	docker run --privileged --rm \
	--device=/dev/kvm:/dev/kvm \
	-v ${BASE_PROJ}:/linux-dev-env -v ${LINUX}:/linux \
	-w /linux \
	-p 127.0.0.1:${SSH_PORT}:52222 \
	-p 127.0.0.1:${NET_PORT}:52223 \
	-p 127.0.0.1:52224:52224/udp \
	-p 127.0.0.1:${GDB_PORT}:1234 \
	-it ${DOCKER_IMAGE}:latest \
	/linux-dev-env/q-script/yifei-q -s

# connect running qemu by ssh
qemu-ssh:
	ssh -o "UserKnownHostsFile=/dev/null" -o "StrictHostKeyChecking=no" -t root@127.0.0.1 -p ${SSH_PORT}

vmlinux: 
	docker run --rm -v ${LINUX}:/linux -w /linux ${DOCKER_IMAGE}  make -j`nproc` bzImage 

headers-install: 
	docker run --rm -v ${LINUX}:/linux -w /linux ${DOCKER_IMAGE}  make -j`nproc` headers_install 

modules-install: 
	docker run --rm -v ${LINUX}:/linux -w /linux ${DOCKER_IMAGE}  make -j`nproc` modules
	docker run --rm -v ${LINUX}:/linux -w /linux ${DOCKER_IMAGE}  make -j`nproc` modules_install

kernel:
	docker run --rm -v ${LINUX}:/linux -w /linux ${DOCKER_IMAGE}  make -j`nproc` 

linux-clean:
	docker run --rm -v ${LINUX}:/linux -w /linux ${DOCKER_IMAGE} make distclean

enter-docker:
	docker run --rm -v ${BASE_PROJ}:/linux-dev-env -w /linux-dev-env -it ${DOCKER_IMAGE} /bin/bash

libbpf:
	docker run --rm -v ${LINUX}:/linux -w /linux/tools/lib/bpf ${DOCKER_IMAGE} make -j`nproc`

libbpf-clean:
	docker run --rm -v ${LINUX}:/linux -w /linux/tools/lib/bpf ${DOCKER_IMAGE} make clean -j`nproc`

bpftool:
	docker run --rm -v ${LINUX}:/linux -w /linux/tools/bpf/bpftool ${DOCKER_IMAGE} make -j`nproc`

bpftool-clean:
	docker run --rm -v ${LINUX}:/linux -w /linux/tools/bpf/bpftool ${DOCKER_IMAGE} make clean -j`nproc`

perf:
	docker run --rm -v ${LINUX}:/linux -w /linux/tools/perf ${DOCKER_IMAGE} make -j`nproc`

perf-clean:
	docker run --rm -v ${LINUX}:/linux -w /linux/tools/per ${DOCKER_IMAGE} make clean -j`nproc`