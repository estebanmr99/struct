SOURCEC=res.c
SOURCECPP=gl.cpp

TARGETC=resolver
TARGETCPP=generadorLaberinto

.PHONY: all c cpp

all: c cpp

c:
	$(CC) $(SOURCEC) -o $(TARGETC)  -lGL -lGLU -lglut -lpthread

cpp:
	$(CXX) -o $(TARGETCPP) $(SOURCECPP)

clean:
	-rm -f *.o
	-rm -f *.txt
	-rm -f $(TARGETC)
	-rm -f $(TARGETCPP)