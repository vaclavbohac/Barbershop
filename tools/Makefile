all: clrsems clrshm

INC = ../shared

clrsems: clrsems.c shared
	cc -I$(INC) -o $@ $< ../shared/sems.o

clrshm: clrshm.c shared
	cc -I$(INC) -o $@ $< ../shared/shared.o

shared:
	@(cd $(INC); $(MAKE))

clean:
	-rm clrshm clrsems
