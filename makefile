
.PHONY: clean all

all: tanh.so asymptotic.so hard.so

clean:
	rm -f tanh.so asymptotic.so hard.so

tanh.so: tanh.cc common.cc
	g++ -O3 -ffast-math $(CXXFLAGS) -Wall -o tanh.so -shared  tanh.cc

asymptotic.so: asymptotic.cc common.cc
	g++ -O3 -ffast-math $(CXXFLAGS) -Wall -o asymptotic.so -shared  asymptotic.cc

hard.so: hard.cc common.cc
	g++ -O3 -ffast-math $(CXXFLAGS) -Wall -o hard.so -shared  hard.cc
