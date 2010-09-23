# use "gcc" to compile source files.
CC = gcc
# the linker is also "gcc". It might be something else with other compilers.
LD = gcc
# program executable file name.
PROG = mars_base
# define the list of source files.
SRCS = main.c atmosferico.c control.c display.c heightmap.c materiales.c objetos.c overlay.c parser.c shared.c lang.c entities.c
# list of generated object files.
OBJS = main.o atmosferico.o control.o display.o heightmap.o materiales.o objetos.o overlay.o parser.o shared.o lang.o entities.o
# Compiler flags go here.
CFLAGS = -ggdb
#-DOPENGLUT_STATIC -IL_STATIC_LIB
# Linker flags go here. Currently there aren't any, but if we'll switch to
# code optimization, we might add "-s" here to strip debug info and symbols.
LDFLAGS = -lGL -lSDLmain -lSDL -lIL -lILU -lILUT -lSDL_ttf -llua
LDFLAGS2 = -s -lGL -lGLU -lm -lXext -lX11 -lSDLmain -lSDL -mwindows -lIL -lILU -lILUT -lSDL_ttf -llua
# use this command to erase files.
RM = /bin/rm -f


# top-level rule, to compile everything.
all: $(PROG)

# rule to link the program
$(PROG): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(PROG)

# now comes a meta-rule for compiling any "C" source file.
%.o: %.c
	$(CC) $(CFLAGS) -c $<


# rule for cleaning re-compilable files.
clean:
	$(RM) $(PROG) $(OBJS)


# rule for building dependency lists, and writing them to a file
# named ".depend".
depend:
	$(RM) .depend
	makedepend -- $(CFLAGS) -- $(SRCS)
	