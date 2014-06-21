.PYTHON: clean

all: showimage showvideo imageROI imgproc scan_image edge_detection

showimage: show_image.c
	gcc -o $@ $< -lopencv_highgui -lopencv_core -lopencv_imgproc

showvideo: show_video.c
	gcc -o $@ $< -lopencv_highgui -lopencv_core -lopencv_imgproc

imageROI: imageROI.c
	gcc -o $@ $< -lopencv_highgui -lopencv_core -lopencv_imgproc

imgproc: imgproc.cpp
	g++ -g -o $@ $< util.cpp -lopencv_highgui -lopencv_core -lopencv_imgproc

edge_detection: edge_detection.cpp
	g++ -g -o $@ $< util.cpp -lopencv_highgui -lopencv_core -lopencv_imgproc

scan_image: scan_image.cpp
	g++ -g -o $@ $< util.cpp -lopencv_highgui -lopencv_core -lopencv_imgproc

clean:
	rm showimage showvideo imageROI imgproc scan_image edge_detection
