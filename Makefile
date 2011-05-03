FLAGS = -Wall -DDEBUG

all:
	@(cd server-processes; $(MAKE) FLAGS="$(FLAGS)")
	@(cd client; $(MAKE) FLAGS="$(FLAGS)")

clean:
	@(cd server-processes; make clean)
	@(cd client; make clean)
	@(cd shared; make clean)
