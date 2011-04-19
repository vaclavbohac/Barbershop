all:
	@(cd msgs; $(MAKE); find . -executable -type f -exec mv {} ../bin/ \;)

clean:
	rm msgs/*.o
	rm bin/*
