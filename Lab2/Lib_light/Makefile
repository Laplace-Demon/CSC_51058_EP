CC = gcc82
## BEGIN_PAS_DONNER
CC = gcc
## END_PAS_DONNER


CFLAGS = -Wall -Wwrite-strings -g

OBJS=buffer.o random.o base64.o

LIB=inf558.a

$(LIB): $(OBJS)
	-rm -f $(LIB)
	ar cr $(LIB) $(OBJS)
	ranlib $(LIB)

buffer.o: buffer.c buffer.h
	$(CC) $(CFLAGS) -c buffer.c

random.o: random.c random.h
	$(CC) $(CFLAGS) -c random.c

base64.o: base64.c base64.h
	$(CC) $(CFLAGS) -c base64.c

##### install the doxygen package, probably along with graphviz
##### it will produce html and latex output which are quite nice
Doxyfile:
	doxygen -g
##	sed 's/OPTIMIZE_OUTPUT_FOR_C  = NO/OPTIMIZE_OUTPUT_FOR_C  = YES/' Doxyfile > /tmp/foo

doc: doxygen
	mv latex/refman.pdf inf558_lib_light.pdf

doxygen: Doxyfile.inf558
	doxygen Doxyfile.inf558
	cd latex; pdflatex refman; makeindex refman; pdflatex refman

clean:
	rm -f *.o inf558.a
