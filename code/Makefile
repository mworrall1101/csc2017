.phony all:
all: pman foo

pman: pman.c
	gcc pman.c mylibs.c linked_list.c linked_list.h mylibs.h -Wall -o pman

foo: foo.c
	gcc foo.c -o foo
 

.PHONY clean:
clean:
	-rm -rf *.o *.exe
