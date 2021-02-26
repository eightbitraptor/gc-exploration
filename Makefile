CC=clang
IDIR=include
ODIR=obj
CFLAGS=-I$(IDIR)
_OBJ = main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

marksweep: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
