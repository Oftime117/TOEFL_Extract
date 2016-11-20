IDIR=include
SDIR=src
ODIR=obj
LDIR=lib

CC=g++ -std=gnu++11
DEBUG=-g
CFLAGS=-Wall -c $(DEBUG) -I$(IDIR) 
LFLAGS=-Wall $(DEBUG)



LIBS=-lpthread -pthread # additionnal libraries

_DEPS = corpus.h essay.h model.h tools.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o corpus.o essay.o model.o tools.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	$(CC) $(LFLAGS) -c -o $@ $< $(CFLAGS)

run: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INDIR)/*~
