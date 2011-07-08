all:build
	make -C build all

build:CMakeLists.txt
	mkdir -p build
	cd build && cmake ..

clean:
	rm -rf build	
