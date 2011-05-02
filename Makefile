all:
	@(cd server-processes; $(MAKE); mv server ../bin/)
	@(cd client; $(MAKE); mv client ../bin/)

clean:
	@(cd bin; rm *)
	@(cd server-processes; make clean)
	@(cd client; make clean)
