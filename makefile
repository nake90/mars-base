# use "gcc" to compile source files.
CC = gcc
# the linker is also "gcc". It might be something else with other compilers.
LD = gcc
# program executable file name.
PROG = mars_base
# define the list of source files.
SRCS = main.c atmosferico.c control.c display.c heightmap.c materiales.c objetos.c overlay.c parser.c shared.c lang.c
# list of generated object files.
OBJS = main.o atmosferico.o control.o display.o heightmap.o materiales.o objetos.o overlay.o parser.o shared.o lang.o
# Compiler flags go here.
CFLAGS = -ggdb -DOPENGLUT_STATIC -IL_STATIC_LIB
# Linker flags go here. Currently there aren't any, but if we'll switch to
# code optimization, we might add "-s" here to strip debug info and symbols.
LDFLAGS = -s -lGL -lGLU -lm -lXext -lX11 -lSDLmain -lSDL -mwindows -lIL -lILU -lILUT -lSDL_ttf
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

# DO NOT DELETE

main.o: mars_base_private.h /usr/include/time.h /usr/include/features.h
main.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
main.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
main.o: /usr/include/bits/time.h /usr/include/bits/types.h
main.o: /usr/include/bits/typesizes.h /usr/include/xlocale.h
main.o: /usr/include/GL/gl.h /usr/include/GL/glext.h /usr/include/inttypes.h
main.o: /usr/include/stdint.h /usr/include/bits/wchar.h
main.o: /usr/include/SDL/SDL.h /usr/include/SDL/SDL_main.h
main.o: /usr/include/SDL/SDL_stdinc.h /usr/include/SDL/SDL_config.h
main.o: /usr/include/SDL/SDL_platform.h /usr/include/sys/types.h
main.o: /usr/include/endian.h /usr/include/bits/endian.h
main.o: /usr/include/bits/byteswap.h /usr/include/sys/select.h
main.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
main.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
main.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
main.o: /usr/include/wchar.h /usr/include/bits/stdio_lim.h
main.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
main.o: /usr/include/alloca.h /usr/include/string.h /usr/include/strings.h
main.o: /usr/include/ctype.h /usr/include/iconv.h
main.o: /usr/include/SDL/begin_code.h /usr/include/SDL/close_code.h
main.o: /usr/include/SDL/SDL_audio.h /usr/include/SDL/SDL_error.h
main.o: /usr/include/SDL/SDL_endian.h /usr/include/SDL/SDL_mutex.h
main.o: /usr/include/SDL/SDL_thread.h /usr/include/SDL/SDL_rwops.h
main.o: /usr/include/SDL/SDL_cdrom.h /usr/include/SDL/SDL_cpuinfo.h
main.o: /usr/include/SDL/SDL_events.h /usr/include/SDL/SDL_active.h
main.o: /usr/include/SDL/SDL_keyboard.h /usr/include/SDL/SDL_keysym.h
main.o: /usr/include/SDL/SDL_mouse.h /usr/include/SDL/SDL_video.h
main.o: /usr/include/SDL/SDL_joystick.h /usr/include/SDL/SDL_quit.h
main.o: /usr/include/SDL/SDL_loadso.h /usr/include/SDL/SDL_timer.h
main.o: /usr/include/SDL/SDL_version.h /usr/include/SDL/SDL_ttf.h
main.o: /usr/include/SDL/SDL.h /usr/include/IL/ilut.h /usr/include/IL/il.h
main.o: /usr/include/limits.h /usr/include/bits/posix1_lim.h
main.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
main.o: /usr/include/bits/posix2_lim.h /usr/include/IL/ilu.h shared.h lang.h
main.o: objetos.h overlay.h heightmap.h materiales.h atmosferico.h control.h
main.o: display.h parser.h
atmosferico.o: atmosferico.h shared.h /usr/include/GL/gl.h
atmosferico.o: /usr/include/GL/glext.h /usr/include/inttypes.h
atmosferico.o: /usr/include/features.h /usr/include/sys/cdefs.h
atmosferico.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
atmosferico.o: /usr/include/gnu/stubs-64.h /usr/include/stdint.h
atmosferico.o: /usr/include/bits/wchar.h /usr/include/SDL/SDL.h
atmosferico.o: /usr/include/SDL/SDL_main.h /usr/include/SDL/SDL_stdinc.h
atmosferico.o: /usr/include/SDL/SDL_config.h /usr/include/SDL/SDL_platform.h
atmosferico.o: /usr/include/sys/types.h /usr/include/bits/types.h
atmosferico.o: /usr/include/bits/typesizes.h /usr/include/time.h
atmosferico.o: /usr/include/bits/time.h /usr/include/xlocale.h
atmosferico.o: /usr/include/endian.h /usr/include/bits/endian.h
atmosferico.o: /usr/include/bits/byteswap.h /usr/include/sys/select.h
atmosferico.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
atmosferico.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
atmosferico.o: /usr/include/stdio.h /usr/include/libio.h
atmosferico.o: /usr/include/_G_config.h /usr/include/wchar.h
atmosferico.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
atmosferico.o: /usr/include/stdlib.h /usr/include/alloca.h
atmosferico.o: /usr/include/string.h /usr/include/strings.h
atmosferico.o: /usr/include/ctype.h /usr/include/iconv.h
atmosferico.o: /usr/include/SDL/begin_code.h /usr/include/SDL/close_code.h
atmosferico.o: /usr/include/SDL/SDL_audio.h /usr/include/SDL/SDL_error.h
atmosferico.o: /usr/include/SDL/SDL_endian.h /usr/include/SDL/SDL_mutex.h
atmosferico.o: /usr/include/SDL/SDL_thread.h /usr/include/SDL/SDL_rwops.h
atmosferico.o: /usr/include/SDL/SDL_cdrom.h /usr/include/SDL/SDL_cpuinfo.h
atmosferico.o: /usr/include/SDL/SDL_events.h /usr/include/SDL/SDL_active.h
atmosferico.o: /usr/include/SDL/SDL_keyboard.h /usr/include/SDL/SDL_keysym.h
atmosferico.o: /usr/include/SDL/SDL_mouse.h /usr/include/SDL/SDL_video.h
atmosferico.o: /usr/include/SDL/SDL_joystick.h /usr/include/SDL/SDL_quit.h
atmosferico.o: /usr/include/SDL/SDL_loadso.h /usr/include/SDL/SDL_timer.h
atmosferico.o: /usr/include/SDL/SDL_version.h /usr/include/SDL/SDL_ttf.h
atmosferico.o: /usr/include/SDL/SDL.h mars_base_private.h lang.h
control.o: control.h /usr/include/SDL/SDL.h /usr/include/SDL/SDL_main.h
control.o: /usr/include/SDL/SDL_stdinc.h /usr/include/SDL/SDL_config.h
control.o: /usr/include/SDL/SDL_platform.h /usr/include/sys/types.h
control.o: /usr/include/features.h /usr/include/sys/cdefs.h
control.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
control.o: /usr/include/gnu/stubs-64.h /usr/include/bits/types.h
control.o: /usr/include/bits/typesizes.h /usr/include/time.h
control.o: /usr/include/bits/time.h /usr/include/xlocale.h
control.o: /usr/include/endian.h /usr/include/bits/endian.h
control.o: /usr/include/bits/byteswap.h /usr/include/sys/select.h
control.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
control.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
control.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
control.o: /usr/include/wchar.h /usr/include/bits/stdio_lim.h
control.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
control.o: /usr/include/alloca.h /usr/include/string.h /usr/include/strings.h
control.o: /usr/include/inttypes.h /usr/include/stdint.h
control.o: /usr/include/bits/wchar.h /usr/include/ctype.h
control.o: /usr/include/iconv.h /usr/include/SDL/begin_code.h
control.o: /usr/include/SDL/close_code.h /usr/include/SDL/SDL_audio.h
control.o: /usr/include/SDL/SDL_error.h /usr/include/SDL/SDL_endian.h
control.o: /usr/include/SDL/SDL_mutex.h /usr/include/SDL/SDL_thread.h
control.o: /usr/include/SDL/SDL_rwops.h /usr/include/SDL/SDL_cdrom.h
control.o: /usr/include/SDL/SDL_cpuinfo.h /usr/include/SDL/SDL_events.h
control.o: /usr/include/SDL/SDL_active.h /usr/include/SDL/SDL_keyboard.h
control.o: /usr/include/SDL/SDL_keysym.h /usr/include/SDL/SDL_mouse.h
control.o: /usr/include/SDL/SDL_video.h /usr/include/SDL/SDL_joystick.h
control.o: /usr/include/SDL/SDL_quit.h /usr/include/SDL/SDL_loadso.h
control.o: /usr/include/SDL/SDL_timer.h /usr/include/SDL/SDL_version.h
control.o: shared.h /usr/include/GL/gl.h /usr/include/GL/glext.h
control.o: /usr/include/SDL/SDL_ttf.h /usr/include/SDL/SDL.h
control.o: mars_base_private.h lang.h /usr/include/IL/ilut.h
control.o: /usr/include/IL/il.h /usr/include/limits.h
control.o: /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h
control.o: /usr/include/linux/limits.h /usr/include/bits/posix2_lim.h
control.o: /usr/include/IL/ilu.h objetos.h overlay.h heightmap.h parser.h
display.o: /usr/include/GL/gl.h /usr/include/GL/glext.h
display.o: /usr/include/inttypes.h /usr/include/features.h
display.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
display.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
display.o: /usr/include/stdint.h /usr/include/bits/wchar.h
display.o: /usr/include/GL/glu.h /usr/include/SDL/SDL_ttf.h
display.o: /usr/include/SDL/SDL.h /usr/include/SDL/begin_code.h
display.o: /usr/include/SDL/close_code.h /usr/include/IL/ilut.h
display.o: /usr/include/IL/il.h /usr/include/stdio.h
display.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
display.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
display.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
display.o: /usr/include/limits.h /usr/include/bits/posix1_lim.h
display.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
display.o: /usr/include/bits/posix2_lim.h /usr/include/IL/ilu.h atmosferico.h
display.o: shared.h /usr/include/SDL/SDL.h /usr/include/SDL/SDL_main.h
display.o: /usr/include/SDL/SDL_stdinc.h /usr/include/SDL/SDL_config.h
display.o: /usr/include/SDL/SDL_platform.h /usr/include/sys/types.h
display.o: /usr/include/time.h /usr/include/bits/time.h
display.o: /usr/include/xlocale.h /usr/include/endian.h
display.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
display.o: /usr/include/sys/select.h /usr/include/bits/select.h
display.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h
display.o: /usr/include/bits/pthreadtypes.h /usr/include/stdlib.h
display.o: /usr/include/alloca.h /usr/include/string.h /usr/include/strings.h
display.o: /usr/include/ctype.h /usr/include/iconv.h
display.o: /usr/include/SDL/SDL_audio.h /usr/include/SDL/SDL_error.h
display.o: /usr/include/SDL/SDL_endian.h /usr/include/SDL/SDL_mutex.h
display.o: /usr/include/SDL/SDL_thread.h /usr/include/SDL/SDL_rwops.h
display.o: /usr/include/SDL/SDL_cdrom.h /usr/include/SDL/SDL_cpuinfo.h
display.o: /usr/include/SDL/SDL_events.h /usr/include/SDL/SDL_active.h
display.o: /usr/include/SDL/SDL_keyboard.h /usr/include/SDL/SDL_keysym.h
display.o: /usr/include/SDL/SDL_mouse.h /usr/include/SDL/SDL_video.h
display.o: /usr/include/SDL/SDL_joystick.h /usr/include/SDL/SDL_quit.h
display.o: /usr/include/SDL/SDL_loadso.h /usr/include/SDL/SDL_timer.h
display.o: /usr/include/SDL/SDL_version.h mars_base_private.h lang.h
display.o: objetos.h display.h heightmap.h overlay.h
heightmap.o: heightmap.h shared.h /usr/include/GL/gl.h
heightmap.o: /usr/include/GL/glext.h /usr/include/inttypes.h
heightmap.o: /usr/include/features.h /usr/include/sys/cdefs.h
heightmap.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
heightmap.o: /usr/include/gnu/stubs-64.h /usr/include/stdint.h
heightmap.o: /usr/include/bits/wchar.h /usr/include/SDL/SDL.h
heightmap.o: /usr/include/SDL/SDL_main.h /usr/include/SDL/SDL_stdinc.h
heightmap.o: /usr/include/SDL/SDL_config.h /usr/include/SDL/SDL_platform.h
heightmap.o: /usr/include/sys/types.h /usr/include/bits/types.h
heightmap.o: /usr/include/bits/typesizes.h /usr/include/time.h
heightmap.o: /usr/include/bits/time.h /usr/include/xlocale.h
heightmap.o: /usr/include/endian.h /usr/include/bits/endian.h
heightmap.o: /usr/include/bits/byteswap.h /usr/include/sys/select.h
heightmap.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
heightmap.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
heightmap.o: /usr/include/stdio.h /usr/include/libio.h
heightmap.o: /usr/include/_G_config.h /usr/include/wchar.h
heightmap.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
heightmap.o: /usr/include/stdlib.h /usr/include/alloca.h
heightmap.o: /usr/include/string.h /usr/include/strings.h
heightmap.o: /usr/include/ctype.h /usr/include/iconv.h
heightmap.o: /usr/include/SDL/begin_code.h /usr/include/SDL/close_code.h
heightmap.o: /usr/include/SDL/SDL_audio.h /usr/include/SDL/SDL_error.h
heightmap.o: /usr/include/SDL/SDL_endian.h /usr/include/SDL/SDL_mutex.h
heightmap.o: /usr/include/SDL/SDL_thread.h /usr/include/SDL/SDL_rwops.h
heightmap.o: /usr/include/SDL/SDL_cdrom.h /usr/include/SDL/SDL_cpuinfo.h
heightmap.o: /usr/include/SDL/SDL_events.h /usr/include/SDL/SDL_active.h
heightmap.o: /usr/include/SDL/SDL_keyboard.h /usr/include/SDL/SDL_keysym.h
heightmap.o: /usr/include/SDL/SDL_mouse.h /usr/include/SDL/SDL_video.h
heightmap.o: /usr/include/SDL/SDL_joystick.h /usr/include/SDL/SDL_quit.h
heightmap.o: /usr/include/SDL/SDL_loadso.h /usr/include/SDL/SDL_timer.h
heightmap.o: /usr/include/SDL/SDL_version.h /usr/include/SDL/SDL_ttf.h
heightmap.o: /usr/include/SDL/SDL.h mars_base_private.h lang.h
heightmap.o: /usr/include/IL/ilut.h /usr/include/IL/il.h
heightmap.o: /usr/include/limits.h /usr/include/bits/posix1_lim.h
heightmap.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
heightmap.o: /usr/include/bits/posix2_lim.h /usr/include/IL/ilu.h
materiales.o: materiales.h shared.h /usr/include/GL/gl.h
materiales.o: /usr/include/GL/glext.h /usr/include/inttypes.h
materiales.o: /usr/include/features.h /usr/include/sys/cdefs.h
materiales.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
materiales.o: /usr/include/gnu/stubs-64.h /usr/include/stdint.h
materiales.o: /usr/include/bits/wchar.h /usr/include/SDL/SDL.h
materiales.o: /usr/include/SDL/SDL_main.h /usr/include/SDL/SDL_stdinc.h
materiales.o: /usr/include/SDL/SDL_config.h /usr/include/SDL/SDL_platform.h
materiales.o: /usr/include/sys/types.h /usr/include/bits/types.h
materiales.o: /usr/include/bits/typesizes.h /usr/include/time.h
materiales.o: /usr/include/bits/time.h /usr/include/xlocale.h
materiales.o: /usr/include/endian.h /usr/include/bits/endian.h
materiales.o: /usr/include/bits/byteswap.h /usr/include/sys/select.h
materiales.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
materiales.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
materiales.o: /usr/include/stdio.h /usr/include/libio.h
materiales.o: /usr/include/_G_config.h /usr/include/wchar.h
materiales.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
materiales.o: /usr/include/stdlib.h /usr/include/alloca.h
materiales.o: /usr/include/string.h /usr/include/strings.h
materiales.o: /usr/include/ctype.h /usr/include/iconv.h
materiales.o: /usr/include/SDL/begin_code.h /usr/include/SDL/close_code.h
materiales.o: /usr/include/SDL/SDL_audio.h /usr/include/SDL/SDL_error.h
materiales.o: /usr/include/SDL/SDL_endian.h /usr/include/SDL/SDL_mutex.h
materiales.o: /usr/include/SDL/SDL_thread.h /usr/include/SDL/SDL_rwops.h
materiales.o: /usr/include/SDL/SDL_cdrom.h /usr/include/SDL/SDL_cpuinfo.h
materiales.o: /usr/include/SDL/SDL_events.h /usr/include/SDL/SDL_active.h
materiales.o: /usr/include/SDL/SDL_keyboard.h /usr/include/SDL/SDL_keysym.h
materiales.o: /usr/include/SDL/SDL_mouse.h /usr/include/SDL/SDL_video.h
materiales.o: /usr/include/SDL/SDL_joystick.h /usr/include/SDL/SDL_quit.h
materiales.o: /usr/include/SDL/SDL_loadso.h /usr/include/SDL/SDL_timer.h
materiales.o: /usr/include/SDL/SDL_version.h /usr/include/SDL/SDL_ttf.h
materiales.o: /usr/include/SDL/SDL.h mars_base_private.h lang.h
materiales.o: /usr/include/IL/ilut.h /usr/include/IL/il.h
materiales.o: /usr/include/limits.h /usr/include/bits/posix1_lim.h
materiales.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
materiales.o: /usr/include/bits/posix2_lim.h /usr/include/IL/ilu.h
objetos.o: objetos.h shared.h /usr/include/GL/gl.h /usr/include/GL/glext.h
objetos.o: /usr/include/inttypes.h /usr/include/features.h
objetos.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
objetos.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
objetos.o: /usr/include/stdint.h /usr/include/bits/wchar.h
objetos.o: /usr/include/SDL/SDL.h /usr/include/SDL/SDL_main.h
objetos.o: /usr/include/SDL/SDL_stdinc.h /usr/include/SDL/SDL_config.h
objetos.o: /usr/include/SDL/SDL_platform.h /usr/include/sys/types.h
objetos.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
objetos.o: /usr/include/time.h /usr/include/bits/time.h
objetos.o: /usr/include/xlocale.h /usr/include/endian.h
objetos.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
objetos.o: /usr/include/sys/select.h /usr/include/bits/select.h
objetos.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h
objetos.o: /usr/include/bits/pthreadtypes.h /usr/include/stdio.h
objetos.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
objetos.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
objetos.o: /usr/include/stdlib.h /usr/include/alloca.h /usr/include/string.h
objetos.o: /usr/include/strings.h /usr/include/ctype.h /usr/include/iconv.h
objetos.o: /usr/include/SDL/begin_code.h /usr/include/SDL/close_code.h
objetos.o: /usr/include/SDL/SDL_audio.h /usr/include/SDL/SDL_error.h
objetos.o: /usr/include/SDL/SDL_endian.h /usr/include/SDL/SDL_mutex.h
objetos.o: /usr/include/SDL/SDL_thread.h /usr/include/SDL/SDL_rwops.h
objetos.o: /usr/include/SDL/SDL_cdrom.h /usr/include/SDL/SDL_cpuinfo.h
objetos.o: /usr/include/SDL/SDL_events.h /usr/include/SDL/SDL_active.h
objetos.o: /usr/include/SDL/SDL_keyboard.h /usr/include/SDL/SDL_keysym.h
objetos.o: /usr/include/SDL/SDL_mouse.h /usr/include/SDL/SDL_video.h
objetos.o: /usr/include/SDL/SDL_joystick.h /usr/include/SDL/SDL_quit.h
objetos.o: /usr/include/SDL/SDL_loadso.h /usr/include/SDL/SDL_timer.h
objetos.o: /usr/include/SDL/SDL_version.h /usr/include/SDL/SDL_ttf.h
objetos.o: /usr/include/SDL/SDL.h mars_base_private.h lang.h
objetos.o: /usr/include/GL/glu.h /usr/include/IL/ilut.h /usr/include/IL/il.h
objetos.o: /usr/include/limits.h /usr/include/bits/posix1_lim.h
objetos.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
objetos.o: /usr/include/bits/posix2_lim.h /usr/include/IL/ilu.h
objetos.o: /usr/include/dirent.h /usr/include/bits/dirent.h
objetos.o: /usr/include/sys/stat.h /usr/include/bits/stat.h
overlay.o: control.h /usr/include/SDL/SDL.h /usr/include/SDL/SDL_main.h
overlay.o: /usr/include/SDL/SDL_stdinc.h /usr/include/SDL/SDL_config.h
overlay.o: /usr/include/SDL/SDL_platform.h /usr/include/sys/types.h
overlay.o: /usr/include/features.h /usr/include/sys/cdefs.h
overlay.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
overlay.o: /usr/include/gnu/stubs-64.h /usr/include/bits/types.h
overlay.o: /usr/include/bits/typesizes.h /usr/include/time.h
overlay.o: /usr/include/bits/time.h /usr/include/xlocale.h
overlay.o: /usr/include/endian.h /usr/include/bits/endian.h
overlay.o: /usr/include/bits/byteswap.h /usr/include/sys/select.h
overlay.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
overlay.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
overlay.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
overlay.o: /usr/include/wchar.h /usr/include/bits/stdio_lim.h
overlay.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
overlay.o: /usr/include/alloca.h /usr/include/string.h /usr/include/strings.h
overlay.o: /usr/include/inttypes.h /usr/include/stdint.h
overlay.o: /usr/include/bits/wchar.h /usr/include/ctype.h
overlay.o: /usr/include/iconv.h /usr/include/SDL/begin_code.h
overlay.o: /usr/include/SDL/close_code.h /usr/include/SDL/SDL_audio.h
overlay.o: /usr/include/SDL/SDL_error.h /usr/include/SDL/SDL_endian.h
overlay.o: /usr/include/SDL/SDL_mutex.h /usr/include/SDL/SDL_thread.h
overlay.o: /usr/include/SDL/SDL_rwops.h /usr/include/SDL/SDL_cdrom.h
overlay.o: /usr/include/SDL/SDL_cpuinfo.h /usr/include/SDL/SDL_events.h
overlay.o: /usr/include/SDL/SDL_active.h /usr/include/SDL/SDL_keyboard.h
overlay.o: /usr/include/SDL/SDL_keysym.h /usr/include/SDL/SDL_mouse.h
overlay.o: /usr/include/SDL/SDL_video.h /usr/include/SDL/SDL_joystick.h
overlay.o: /usr/include/SDL/SDL_quit.h /usr/include/SDL/SDL_loadso.h
overlay.o: /usr/include/SDL/SDL_timer.h /usr/include/SDL/SDL_version.h
overlay.o: shared.h /usr/include/GL/gl.h /usr/include/GL/glext.h
overlay.o: /usr/include/SDL/SDL_ttf.h /usr/include/SDL/SDL.h
overlay.o: mars_base_private.h lang.h overlay.h heightmap.h
overlay.o: /usr/include/GL/glu.h /usr/include/IL/ilut.h /usr/include/IL/il.h
overlay.o: /usr/include/limits.h /usr/include/bits/posix1_lim.h
overlay.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
overlay.o: /usr/include/bits/posix2_lim.h /usr/include/IL/ilu.h
parser.o: parser.h /usr/include/stdio.h /usr/include/features.h
parser.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
parser.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
parser.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
parser.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
parser.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
parser.o: /usr/include/stdlib.h /usr/include/sys/types.h /usr/include/time.h
parser.o: /usr/include/bits/time.h /usr/include/xlocale.h
parser.o: /usr/include/endian.h /usr/include/bits/endian.h
parser.o: /usr/include/bits/byteswap.h /usr/include/sys/select.h
parser.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
parser.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
parser.o: /usr/include/alloca.h shared.h /usr/include/GL/gl.h
parser.o: /usr/include/GL/glext.h /usr/include/inttypes.h
parser.o: /usr/include/stdint.h /usr/include/bits/wchar.h
parser.o: /usr/include/SDL/SDL.h /usr/include/SDL/SDL_main.h
parser.o: /usr/include/SDL/SDL_stdinc.h /usr/include/SDL/SDL_config.h
parser.o: /usr/include/SDL/SDL_platform.h /usr/include/string.h
parser.o: /usr/include/strings.h /usr/include/ctype.h /usr/include/iconv.h
parser.o: /usr/include/SDL/begin_code.h /usr/include/SDL/close_code.h
parser.o: /usr/include/SDL/SDL_audio.h /usr/include/SDL/SDL_error.h
parser.o: /usr/include/SDL/SDL_endian.h /usr/include/SDL/SDL_mutex.h
parser.o: /usr/include/SDL/SDL_thread.h /usr/include/SDL/SDL_rwops.h
parser.o: /usr/include/SDL/SDL_cdrom.h /usr/include/SDL/SDL_cpuinfo.h
parser.o: /usr/include/SDL/SDL_events.h /usr/include/SDL/SDL_active.h
parser.o: /usr/include/SDL/SDL_keyboard.h /usr/include/SDL/SDL_keysym.h
parser.o: /usr/include/SDL/SDL_mouse.h /usr/include/SDL/SDL_video.h
parser.o: /usr/include/SDL/SDL_joystick.h /usr/include/SDL/SDL_quit.h
parser.o: /usr/include/SDL/SDL_loadso.h /usr/include/SDL/SDL_timer.h
parser.o: /usr/include/SDL/SDL_version.h /usr/include/SDL/SDL_ttf.h
parser.o: /usr/include/SDL/SDL.h mars_base_private.h lang.h
shared.o: /usr/include/GL/glu.h /usr/include/GL/gl.h /usr/include/GL/glext.h
shared.o: /usr/include/inttypes.h /usr/include/features.h
shared.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
shared.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
shared.o: /usr/include/stdint.h /usr/include/bits/wchar.h
shared.o: /usr/include/IL/ilut.h /usr/include/IL/il.h /usr/include/stdio.h
shared.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
shared.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
shared.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
shared.o: /usr/include/limits.h /usr/include/bits/posix1_lim.h
shared.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
shared.o: /usr/include/bits/posix2_lim.h /usr/include/IL/ilu.h
shared.o: /usr/include/math.h /usr/include/bits/huge_val.h
shared.o: /usr/include/bits/huge_valf.h /usr/include/bits/huge_vall.h
shared.o: /usr/include/bits/inf.h /usr/include/bits/nan.h
shared.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
shared.o: /usr/include/time.h /usr/include/bits/time.h /usr/include/xlocale.h
shared.o: shared.h /usr/include/SDL/SDL.h /usr/include/SDL/SDL_main.h
shared.o: /usr/include/SDL/SDL_stdinc.h /usr/include/SDL/SDL_config.h
shared.o: /usr/include/SDL/SDL_platform.h /usr/include/sys/types.h
shared.o: /usr/include/endian.h /usr/include/bits/endian.h
shared.o: /usr/include/bits/byteswap.h /usr/include/sys/select.h
shared.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
shared.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
shared.o: /usr/include/stdlib.h /usr/include/alloca.h /usr/include/string.h
shared.o: /usr/include/strings.h /usr/include/ctype.h /usr/include/iconv.h
shared.o: /usr/include/SDL/begin_code.h /usr/include/SDL/close_code.h
shared.o: /usr/include/SDL/SDL_audio.h /usr/include/SDL/SDL_error.h
shared.o: /usr/include/SDL/SDL_endian.h /usr/include/SDL/SDL_mutex.h
shared.o: /usr/include/SDL/SDL_thread.h /usr/include/SDL/SDL_rwops.h
shared.o: /usr/include/SDL/SDL_cdrom.h /usr/include/SDL/SDL_cpuinfo.h
shared.o: /usr/include/SDL/SDL_events.h /usr/include/SDL/SDL_active.h
shared.o: /usr/include/SDL/SDL_keyboard.h /usr/include/SDL/SDL_keysym.h
shared.o: /usr/include/SDL/SDL_mouse.h /usr/include/SDL/SDL_video.h
shared.o: /usr/include/SDL/SDL_joystick.h /usr/include/SDL/SDL_quit.h
shared.o: /usr/include/SDL/SDL_loadso.h /usr/include/SDL/SDL_timer.h
shared.o: /usr/include/SDL/SDL_version.h /usr/include/SDL/SDL_ttf.h
shared.o: /usr/include/SDL/SDL.h mars_base_private.h lang.h
lang.o: lang.h shared.h /usr/include/GL/gl.h /usr/include/GL/glext.h
lang.o: /usr/include/inttypes.h /usr/include/features.h
lang.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
lang.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
lang.o: /usr/include/stdint.h /usr/include/bits/wchar.h
lang.o: /usr/include/SDL/SDL.h /usr/include/SDL/SDL_main.h
lang.o: /usr/include/SDL/SDL_stdinc.h /usr/include/SDL/SDL_config.h
lang.o: /usr/include/SDL/SDL_platform.h /usr/include/sys/types.h
lang.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
lang.o: /usr/include/time.h /usr/include/bits/time.h /usr/include/xlocale.h
lang.o: /usr/include/endian.h /usr/include/bits/endian.h
lang.o: /usr/include/bits/byteswap.h /usr/include/sys/select.h
lang.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
lang.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
lang.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
lang.o: /usr/include/wchar.h /usr/include/bits/stdio_lim.h
lang.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
lang.o: /usr/include/alloca.h /usr/include/string.h /usr/include/strings.h
lang.o: /usr/include/ctype.h /usr/include/iconv.h
lang.o: /usr/include/SDL/begin_code.h /usr/include/SDL/close_code.h
lang.o: /usr/include/SDL/SDL_audio.h /usr/include/SDL/SDL_error.h
lang.o: /usr/include/SDL/SDL_endian.h /usr/include/SDL/SDL_mutex.h
lang.o: /usr/include/SDL/SDL_thread.h /usr/include/SDL/SDL_rwops.h
lang.o: /usr/include/SDL/SDL_cdrom.h /usr/include/SDL/SDL_cpuinfo.h
lang.o: /usr/include/SDL/SDL_events.h /usr/include/SDL/SDL_active.h
lang.o: /usr/include/SDL/SDL_keyboard.h /usr/include/SDL/SDL_keysym.h
lang.o: /usr/include/SDL/SDL_mouse.h /usr/include/SDL/SDL_video.h
lang.o: /usr/include/SDL/SDL_joystick.h /usr/include/SDL/SDL_quit.h
lang.o: /usr/include/SDL/SDL_loadso.h /usr/include/SDL/SDL_timer.h
lang.o: /usr/include/SDL/SDL_version.h /usr/include/SDL/SDL_ttf.h
lang.o: /usr/include/SDL/SDL.h mars_base_private.h parser.h
