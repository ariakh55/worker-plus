# Worker+ build file
# Written and created by Aria Khoshnood
# This build file is optimized for Linux, Windows and MacOS
# Uncomment the commands and variables needed to build this project on Windows.
# Notice: Cygwin is required to build for Windows envrionment

CC = g++
# Uncommnet this line to compile this program without terminal in Windows
# CFLAGS = -mwindows

# Change the file extension to the OS specific executable extension.
# OBJ = build/main.exe
OBJ = build/main

# Add extra flag processing and calculation flags
CPP_LIB = lib/bootstraper.cpp lib/flags.cpp

build: prebuild
	$(CC) -g main.cpp $(CPP_LIB) -o $(OBJ) #$(CFLAGS)

#Windows prebuild action
# prebuild:
#	if exist ".\build\"  echo "Dir exists" else mkdir ".\build\"

prebuild:
	if [ -d "./build" ]; then echo "Dir exists"; else mkdir build; fi
