CC=clang
IDIR=include
ODIR=obj

CFLAGS="-I$(IDIR) -Werror -Wall"
debugflags="-g"
optflags="-O0"

_OBJ = main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) $(debugflags) $(cflags) -c -o $@ $< $(CFLAGS)

marksweep: $(OBJ)
	$(CC) $(debugflags) $(cflags) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o marksweep $(INCDIR)/*~
