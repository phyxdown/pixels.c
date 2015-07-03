makefiles = pixels/makefile
edit : 
	cd pixels && make
clean :
	cd pixels && make clean
install :
	cd pixels && make install 
	rm /usr/local/include/pixels.h
	cp pixels.h /usr/local/include/
test : 
	gcc test.c -std=c99 -lpixels -lm -o test
	./test
	rm test
