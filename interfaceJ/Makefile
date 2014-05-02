mpi : interfaceJ.o childProcess.o imageJRunner.o fifoClass.o
	ar rcs libinterfaceJ.a interfaceJ.o childProcess.o imageJRunner.o fifoClass.o

interfaceJ : main.o interfaceJ.o childProcess.o imageJRunner.o fifoClass.o ImageJLog.o
	g++ -std=c++11 -o interfaceJ main.o interfaceJ.o childProcess.o imageJRunner.o fifoClass.o ImageJLog.o -pthread

main.class.o : main.class.cpp interfaceJ.h
	g++ -std=c++11 -c main.class.cpp
	
ImageJLog.o:	ImageJLog.h ImageJLog.cpp
	g++ -std=c++11 -c ImageJLog.h ImageJLog.cpp


interfaceJ.o: interfaceJ.h imageJRunner.h interfaceJ.cpp fifoClass.h
	g++ -c interfaceJ.cpp

fifoClass.o : interfaceJ.h fifoClass.cpp fifoClass.h
	g++ -c fifoClass.cpp
	
childProcess.o : childProcess.cpp childProcess.h
	g++ -c childProcess.cpp

imageJRunner.o : imageJRunner.cpp imageJRunner.h childProcess.h
	g++ -c imageJRunner.cpp

clean:
	rm -f libinterfaceJ.a
	rm -f *.o
	rm -f interfaceJ
