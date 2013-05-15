Barbershop
==========

Implementation of well known IPC (inter-process communication) problem called [sleeping barber](http://en.wikipedia.org/wiki/Sleeping_barber_problem).

All you have to do to compile project is run 'make' in the project root directory.
   
    $ make

To run server (or client) go to the bin directory and run one of the programs:
   
    $ cd bin
    $ ./server # or ./client

In case of some error you can clear shared memory and remove semaphores by:
    
    $ make clrshared

Project includes some basic unit tests. To compile them you need to install libgtest-dev package. Compilation of tests is posible only from tests directory. All tests can be executed at once with prepared shell script 'run_tests.sh'.
    
    $ sudo apt-get install libgtest-dev
    $ cd barbershop/tests
    $ make
    $ ./run_tests.sh

Václav Boháč &copy; 2011 - 2013.
