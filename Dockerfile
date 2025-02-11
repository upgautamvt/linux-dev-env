FROM ubuntu:24.04 AS linux-builder

ENV PATH=/linux/tools/lib/bpf/:$PATH \
    LINUX=/linux

RUN DEBIAN_FRONTEND=noninteractive apt-get update
RUN DEBIAN_FRONTEND=noninteractive apt-get upgrade -y

RUN DEBIAN_FRONTEND=noninteractive apt-get update
RUN DEBIAN_FRONTEND=noninteractive apt-get install --fix-missing -y \
    git \
    build-essential \
    gcc \
    g++ \
    cpio \
    fakeroot \
    libncurses5-dev \
    libssl-dev \
    ccache \
    dwarves \
    libelf-dev \
    cmake \
    automake \
    mold \
    libdw-dev \
    libdwarf-dev \
    bpfcc-tools \
    libbpfcc-dev \
    libbpfcc \
    zstd \
    linux-headers-generic \
    libtinfo-dev \
    terminator \
    libstdc++-12-dev \
    libstdc++-13-dev \
    libstdc++-14-dev \
    bc \
    fping \
    xterm \
    trace-cmd \
    tcpdump \
    flex \
    bison \
    rsync \
    python3-venv \
    ltrace \
    sysdig \
    kmod \
    xdp-tools \
    net-tools \
    iproute2 \
    htop \
    libcap-dev \
    libdisasm-dev \
    binutils-dev \
    unzip \
    pkg-config \
    lsb-release \
    wget \
    curl \
    software-properties-common \
    gnupg \
    zlib1g \
    openssh-client \
    openssh-server \
    strace \
    bpftrace \
    tmux \
    gdb \
    attr \
    busybox \
    vim \
    openssl \
    genisoimage \
    pciutils \
    llvm \
    libvirt-daemon-system \
    libvirt-clients \
    qemu-kvm gcc-multilib \
    libbpf-dev \
    linux-tools-common


RUN wget https://apt.llvm.org/llvm.sh
RUN chmod +x llvm.sh
RUN ./llvm.sh 20
RUN ln -sf /usr/bin/clang-20 /usr/bin/clang
RUN ln -sf /usr/bin/clang++-20 /usr/bin/clang++
RUN ln -sf /usr/bin/ld.lld-20 /usr/bin/ld.lld
RUN ln -sf /usr/bin/llvm-ar-20 /usr/bin/llvm-ar
RUN ln -sf /usr/bin/llvm-strip-20 /usr/bin/llvm-strip
RUN ln -sf /usr/bin/llvm-objdump-20 /usr/bin/llvm-objdump
RUN ln -sf /usr/bin/llc-20 /usr/bin/llc
RUN ln -sf /usr/bin/llvm-readelf-20 /usr/bin/llvm-readelf
RUN ln -sf /usr/bin/llvm-dis-20 /usr/bin/llvm-dis
RUN ln -sf /usr/bin/opt-20 /usr/bin/opt

# Qemu
RUN DEBIAN_FRONTEND=noninteractive apt-get update
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y qemu-kvm libvirt-clients libvirt-daemon-system bridge-utils  \
    virtinst libvirt-daemon xterm attr busybox

RUN DEBIAN_FRONTEND=noninteractive apt-get update
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y openssh-client openssh-server

# Debugging
RUN DEBIAN_FRONTEND=noninteractive apt-get update
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y tmux gdb

RUN DEBIAN_FRONTEND=noninteractive apt-get update
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y strace

# Moo kernel bcc and bpftrace
RUN DEBIAN_FRONTEND=noninteractive apt-get update
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y bpftrace


