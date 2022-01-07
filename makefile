
.PHONY: clean all

all: tanh.so asymptotic.so hard.so enveloped.so

clean:
	rm -f tanh.so asymptotic.so hard.so enveloped.so

enveloped.so: enveloped.cc common.cc
	g++ -O3 -ffast-math $(CXXFLAGS) -Wall -o enveloped.so -shared  enveloped.cc

tanh.so: tanh.cc common.cc
	g++ -O3 -ffast-math $(CXXFLAGS) -Wall -o tanh.so -shared  tanh.cc

asymptotic.so: asymptotic.cc common.cc
	g++ -O3 -ffast-math $(CXXFLAGS) -Wall -o asymptotic.so -shared  asymptotic.cc

hard.so: hard.cc common.cc
	g++ -O3 -ffast-math $(CXXFLAGS) -Wall -o hard.so -shared  hard.cc
