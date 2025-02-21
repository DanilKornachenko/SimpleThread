.PHONY: all build run clean

all: run

build:
	mkdir -p build
	cd build && cmake ..
	cd build && make

run: build
	./build/SimpleThread

clean:
	rm -rf build/*
