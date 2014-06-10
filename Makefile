.PYTHON: clean

all: showimage showvideo imageROI imgproc

showimage: show_image.c
	gcc -o $@ $< -lopencv_highgui -lopencv_core -lopencv_imgproc

showvideo: show_video.c
	gcc -o $@ $< -lopencv_highgui -lopencv_core -lopencv_imgproc

imageROI: imageROI.c
	gcc -o $@ $< -lopencv_highgui -lopencv_core -lopencv_imgproc

imgproc: imgproc.cpp
	g++ -g -o $@ $< -lopencv_highgui -lopencv_core -lopencv_imgproc
clean:
	rm showimage showvideo imageROI imgproc