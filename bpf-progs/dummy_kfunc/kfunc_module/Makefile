# hello/Makefile — build a kfunc module with BTF
obj-m           += hello.o

# ensure debug info is passed for BTF generation
# (the kernel build system will pick up -g and invoke pahole if CONFIG_DEBUG_INFO_BTF_MODULES=y)
# you can also use KBUILD_CFLAGS_MODULE if you only want to affect modules:
KBUILD_CFLAGS   += -g -O2

all:
	@echo "Building hello.ko with BTF support"
	$(MAKE) -C /lib/modules/$(shell uname -r)/build \
	        M=$(PWD) modules

clean:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build \
	        M=$(PWD) clean
