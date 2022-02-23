# pl32lib

pl32lib is a library containing boilerplate routines that I use for every
project I make. Most of it is strictly C99-compliant and thus fully portable.
The `pl32-term` module is the only component that isn't fully portable (it uses
POSIX calls, meaning that while portable to any POSIX system, it won't compile
on everything).

# Build instructions

**NOTICE: This project doesn't have a build system in place, you will need to
build it manually. This will only cover Unix-like/Cygwin systems with the GCC
compiler.**

Open a terminal (if on a graphical environment), and run the following from the
project directory:

```
cd src
# For shared
gcc -c -fPIC -I../include pl32-memory.c pl32-shell.c pl32-file.c -o libpl32.o
gcc -shared libpl32.o -o libpl32.so
# For static
gcc -c -I../include pl32-memory.c pl32-shell.c pl32-file.c -o libpl32.o
ar rc libpl32.a libpl32.o
```
