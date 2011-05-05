FLAGS = -Wall

# Uncomment line bellow for debug mode.
# FLAGS += -DDEBUG

all:
	@(cd server-processes; $(MAKE) FLAGS="$(FLAGS)")
	@(cd server-threads;   $(MAKE) FLAGS="$(FLAGS)")
	@(cd client; $(MAKE) FLAGS="$(FLAGS)")
	@(cd tools;  $(MAKE) FLAGS="$(FLAGS)")

clean:
	@(cd server-processes; $(MAKE) clean)
	@(cd server-threads;   $(MAKE) clean)
	@(cd client; $(MAKE) clean)
	@(cd shared; $(MAKE) clean)
	@(cd tools;  $(MAKE) clean)

clrshared: 
	@(cd tools; ./clrsems; ./clrshm)
