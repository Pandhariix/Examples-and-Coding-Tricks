CC = g++ -std=c++11 -O2 -Wall
SRC = redtracking.cpp
EXE = RedTracking
INC = "C:/opencv/build/install/include"
LIB = "C:/opencv/build/bin"
LINK = -lopencv_highgui300 -lopencv_imgproc300 -lopencv_core300 -lopencv_videoio300 -lopencv_imgcodecs300

all:
	echo 'Compiling for $(OS)...'

ifeq ($(OS),Windows_NT)
	$(CC) $(SRC) -o $(EXE) -I$(INC) -L$(LIB) $(LINK)

else
	$(CC) $(SRC) -o $(EXE) `pkg-config --cflags --libs opencv`

endif

clean:

ifeq ($(OS),Windows_NT)
	del RedTracking.exe

else
	rm -rf RedTracking

endif