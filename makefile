IDIR =include
CC=gcc -w

ODIR=obj
SRCDIR=src

MK = @mkdir -p $(@D)

intal_demo:	$(SRCDIR)/intal_demo.c $(ODIR)/intal.o $(ODIR)/utilities.o
	$(CC) -g -o intal_demo $(SRCDIR)/intal_demo.c $(ODIR)/intal.o $(ODIR)/utilities.o

$(ODIR)/intal.o: $(SRCDIR)/intal.c $(IDIR)/intal.h
	$(MK)
	$(CC) -g -c -o $(ODIR)/intal.o -c $(SRCDIR)/intal.c

$(ODIR)/utilities.o: $(SRCDIR)/utilities.c $(IDIR)/utilities.h
	$(MK)
	$(CC) -g -c -o $(ODIR)/utilities.o -c $(SRCDIR)/utilities.c

clean:
	rm -rf $(ODIR)
	rm intal_demo