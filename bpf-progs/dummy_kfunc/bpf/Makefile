SOURCES := $(wildcard *.kern.c)
FILES := $(SOURCES:.c=.o)

CLANG ?= clang
LLVM_STRIP ?= llvm-strip
BPF_TARGET := bpf

CFLAGS := -O2 -g -target $(BPF_TARGET) -Wall -Werror -I/usr/include

all: $(FILES)

$(FILES) : %.o : %.c
	$(CLANG) $(CFLAGS) -c $< -o $@

.PHONY : clean

clean:
	rm $(FILES)