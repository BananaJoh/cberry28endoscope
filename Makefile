all: endoscope

endoscope: main.o tft.o ST7789.o bmp.o
	g++ main.o tft.o ST7789.o bmp.o -lbcm2835 -lrt -lm -lopencv_imgproc -lopencv_videoio -lopencv_core -o endoscope

main.o: main.cpp
	g++ -Os -c main.cpp -lopencv_videoio -I/usr/include/opencv4/

tft.o: tft.c tft.h ST7789.h
	gcc -Os -c tft.c
	
ST7789.o: ST7789.c ST7789.h
	gcc -Os -c ST7789.c

bmp.o: bmp.c bmp.h ST7789.h
	gcc -Os -c bmp.c
	
clean:
	rm -rf *.o endoscope
