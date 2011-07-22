all:prep
	#make -C build VERBOSE=1 all
	make -C build

prep:CMakeLists.txt
	mkdir -p build
	cd build && cmake ..

build:
	./.doit -p --local

clean:
	rm -rf build
