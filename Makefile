export
MAKE := make
MODE := debug
#MODE := release

GXX = g++
LD = ld

CFLAGS = -std=c++11

ifeq ($(MODE), debug)
CLFLAGS += -DDEBUG -g
endif

LKOBJS = $(wildcard ./Build/*.o)

all:
	-@mkdir ./Build
	$(MAKE) -C ./BufferManager all
	$(GXX) $(CFLAGS) -c main.cpp -o ./Build/main.o
	$(GXX) -o MiniSQL ./Build/*.o
	./MiniSQL

clean:
	-@rm -rf ./Build
	-@rm ./MiniSQL
	@echo "Clean Done!"