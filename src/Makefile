ODIR=obj
IDIR=include
EDIR=bin
LDIR=logs
OUTDIR=../output

CC=g++
CFLAGS=-I$(IDIR)

LIBS=

_DEPS = find_political_donors.h heap_utils.h misc_utils.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = find_political_donors.o heap_utils.o misc_utils.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_EXE = find_political_donors
EXE = $(patsubst %,$(EDIR)/%,$(_EXE))

$(ODIR)/%.o: %.cc $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXE): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(OUTDIR)/* $(LDIR)/* $(EDIR)/* *~ core 
