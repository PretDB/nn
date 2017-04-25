SRC=nn.c main.c file.c
OBJ=${SRC:.c=.o}
EXE=nn
GRC=gen.c
GOB=${GRC:.c=.o}
GEN=gen
LIB=
INC=


CC=gcc
LD=gcc
CCFLAG= -g
LDFLAG= -lm -g

$(EXE):$(OBJ)
	ctags *
	$(LD) -o $(EXE) $(LDFLAG) $(OBJ) $(LIB)

$(OBJ):$(SRC)
	$(CC) -c $(CCFLAG) $(SRC)
$(GEN):$(GOB)
	$(LD) -o $(GEN) $(GOB)
$(GOB):$(GRC)
	$(CC) -c $(GRC)

clean:
	rm -f $(OBJ)
cleanall:
	rm -f $(OBJ)
	rm -f $(EXE)
cleanback:
	rm -f *~
