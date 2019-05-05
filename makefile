CC = gcc
OBJS = eurovision.o citizenVote.o state.o judge.o eurovisionTests.o\
eurovisionTestsMain.o map.o libmtm.a
EXEC = eurovision
DEBUG_FLAG = -DNDEBUG valgrind --leak-check=full ./eurovision
COMP_FLAG = -std=c99 -g -Wall -Werror -pedantic-errors -L. -lmtm

$(EXEC) : $(OBJS)
	$(CC) $(DEBUG_FLAG) $(OBJS)  -o $@
eurovision.o : eurovision.c eurovision.h list.h map.o state.o judge.o citizenVote.o
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
eurovisionTests.o : eurovisionTests.c list.h eurovision.h eurovisionTests.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
eurovisionTestsMain.o : eurovisionTestsMain.c eurovisionTests.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
map.o : map.c map.h eurovision.h list.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
state.o : state.c state.h map.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
judge.o : judge.c judge.h map.h list.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
citizenVote.o : citizenVote.c citizenVote.h map.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
clean:
	rm -f $(OBJS) $(EXEC)
