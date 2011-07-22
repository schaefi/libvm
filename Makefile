all:prep
	make -C build all

prep:CMakeLists.txt
	mkdir -p build
	cd build && cmake ..

build:
	./.doit -p --local

clean:
	rm -rf build
