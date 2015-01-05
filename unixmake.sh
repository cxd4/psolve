FLAGS_C="-Wall -pedantic -ansi -std=c89"
FLAGS_x86="-masm=intel -msse2"

cc -S -O3 $FLAGS_C $FLAGS_x86 -o ./psolve.s ./psolve.c
as -o ./psolve.o ./psolve.s
cc -s -o ./psolve ./psolve.o

./psolve 1 0 -1
