XX=g++

CFLAGS=-Wall -c -O3

LDFLAGS=-Wall -O3

TARGETS = Beagle_GPIO.o	\
	  glcd.o \
	  stlcd 


all: $(TARGETS)

Beagle_GPIO.o: Beagle_GPIO.cc Beagle_GPIO.hh
	@echo Compiling $< to $@
	@$(CXX) $(CFLAGS) Beagle_GPIO.cc

glcd.o: glcd.cc glcd.hh
	@echo Compiling $< to $@
	@$(CXX) $(CFLAGS) glcd.cc -o $@


stlcd: stlcd.cc stlcd.hh Beagle_GPIO.o glcd.o
	@echo Compiling $< to $@
	@$(CXX) $(LDLAGS) stlcd.cc Beagle_GPIO.o glcd.o -o $@


clean:
	@rm -rf *~ *.o $(TARGETS) 


######################################
######################################

