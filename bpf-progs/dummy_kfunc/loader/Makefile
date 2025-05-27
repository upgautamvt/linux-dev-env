USER_SRC := $(wildcard *.user.c)
USER := $(USER_SRC:.c=)

BPF-CLANG := clang
BPF_CLANG_CFLAGS := -target bpf -g -Wall -O2 -c

#Your custom compiled linux source code directory
LINUX_DIR := ../../../linux
USER-CFLAGS := -I$(LINUX_DIR)/usr/include -I$(LINUX_DIR)/tools/lib/ -L$(LINUX_DIR)/tools/lib/bpf/

all: $(USER)

$(USER) : % : %.c
	gcc $(USER-CFLAGS) $< -lbpf -o $@

.PHONY : clean

clean :
	rm $(FILES) $(USER)