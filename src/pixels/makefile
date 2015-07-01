objects = pixel.o math.o status.o
libpixels.a : $(objects)
	ar rcs libpixels.a $(objects)
pixel.o : pixel.h math.h status.h defs.h
	gcc -c pixel.c -I../ -std=c99
math.o : math.h
	gcc -c math.c -I../ -std=c99
status.o : status.h
	gcc -c status.c -I../ -std=c99
clean :
	rm libpixels.a $(objects)
install :
	cp libpixels.a /usr/local/lib 
	rm -rf /usr/local/include/pixels
	mkdir /usr/local/include/pixels 
	cp *.h /usr/local/include/pixels/
