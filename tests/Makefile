all:
	@(cd msgs; $(MAKE); find . -executable -type f -exec mv {} ../bin/ \;)
	@(cd helpers; $(MAKE); find . -executable -type f -exec mv {} ../bin/ \;)

clean:
	rm msgs/*.o helpers/*.o
	rm bin/*
