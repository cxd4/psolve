FLAGS_C="-Wall -pedantic -ansi -std=c89"
FLAGS_x86="-masm=intel -msse2"

cc -S -O3 $FLAGS_C $FLAGS_x86 -o ./psolve.s ./psolve.c
as -o ./psolve.o ./psolve.s
cc -s -o ./psolve ./psolve.o

cc -S -O3 $FLAGS_C $FLAGS_x86 -o ./qsolve.s ./qsolve.c
as -o ./qsolve.o ./qsolve.s
cc -s -o ./qsolve ./qsolve.o -lm

./psolve 1 0 -1

# Show solutions for x, when f(x) = x^4 + x^3 + x^2 + x + 1 = 0.
./qsolve 1 1 1 1 1
