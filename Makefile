IDIR=-I. -I/usr/local/Cellar/boost/1.61.0_1/include/
CC=g++
DEPS = timeserver.cpp 

LIBS=-lm -lboost_system 
LDIR=-L/usr/local/Cellar/boost/1.61.0_1/lib

CFLAGS=$(IDIR) $(LDIR)

#%.o: %.c $(DEPS)
#	$(CC) -c -o $@ $< $(CFLAGS)
TimeServer: timeserver.o  
	$(CC) -o TimeServer timeserver.o $(CFLAGS) $(LIBS)

timeserver.o : timeserver.cpp 
	$(CC) -c timeserver.cpp $(CFLAGS) $(LIBS)

#TimeServer:
#	$(CC) -o TimeServer timeserver.cpp $(CFLAGS) $(LIBS)

#TimeServer: timeserver.o  
#	$(CC) -o TimeServer timeserver.o $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f TimeServer *.o
