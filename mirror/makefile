CC = g++
ARCHIVE = Smparounis_Papakiriakopoulos_ex4_K22_2011

all: mirror
	@rm *.o

mirror: objects.o rw_functions.o notificationMonitor.o mirror.o
	@$(CC) objects.o rw_functions.o notificationMonitor.o mirror.o -o mirror

objects.o: objects.cpp objects.h generalHeaders.h
	@$(CC) -c objects.cpp

rw_functions.o: rw_functions.cpp rw_functions.h generalHeaders.h
	@$(CC) -c rw_functions.cpp

notificationMonitor.o: notificationMonitor.cpp notificationMonitor.h generalHeaders.h
	@$(CC) -c notificationMonitor.cpp

mirror.o: mirror.cpp mirror.h generalHeaders.h
	@$(CC) -c mirror.cpp

clean:
	@rm -rf *.o mirror $(ARCHIVE).tar
	@touch *

dist: all README makefile
	@mkdir $(ARCHIVE)
	@cp mirror* objects* notificationMonitor* rw_functions* README makefile $(ARCHIVE)/
	@tar cf ./$(ARCHIVE).tar $(ARCHIVE)
	@rm -rf $(ARCHIVE)
