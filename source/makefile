.PHONY: main.exe
CC=gcc
SDLDIR=C:\Users\yll20\Desktop\work\sdl\sdl2.0\SDL2-2.0.14\x86_64-w64-mingw32
SDLIMAGEDIR=C:\Users\yll20\Desktop\work\sdl\sdl2_image\x86_64-w64-mingw32
SDLTTFDIR=C:\Users\yll20\Desktop\work\sdl\sdl2_ttf\x86_64-w64-mingw32
SDLMIXDIR=C:\Users\yll20\Desktop\work\sdl\sdl2_mixer\x86_64-w64-mingw32
CFLAGS=-I $(SDLDIR)\include\SDL2 -I $(SDLIMAGEDIR)\include\SDL2 -I $(SDLTTFDIR)\include\SDL2 -I $(SDLMIXDIR)\include\SDL2 -L $(SDLDIR)\lib -L $(SDLIMAGEDIR)\lib -L $(SDLTTFDIR)\lib -L $(SDLMIXDIR)\lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -O2 -mwindows
main.exe: main.c
	$(CC) main.c $(CFLAGS) ico.o -o main.exe