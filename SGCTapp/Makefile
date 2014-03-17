# Compiler:
CC = g++

# Flags:
# -Wall -pedantic
CFLAGS = -O3

# More flags:
ifeq ($(OS), Windows_NT)
	FRAMEWORKS = -lsgct -lopengl32 -lglu32 -lgdi32 -lws2_32 -static-libgcc -static-libstdc++
else
	FRAMEWORKS = -lsgct -framework Opengl -framework Cocoa -framework IOKit -stdlib=libstdc++
endif

# Even more flags:
ifeq ($(OS), Windows_NT)
	MKDIR = 
	LIBFOLD = -L"C:\sgct\lib\mingw"
	INCFOLD = -I"C:\sgct\include"
else
	MKDIR = mkdir -p bin
	LIBFOLD = -L"/usr/local/lib"
	INCFOLD = -I"/usr/local/include"
endif

# Files:
FILES = $(wildcard src/*.cpp)

# Binary folder:
BINFOLD = bin/

# Binary name:
ifeq ($(OS), Windows_NT)
	BINNAME = main.exe
else
	BINNAME = main
endif

ifeq ($(OS), Windows_NT)
	FOO = this is windows
else
	FOO = this is not windows
endif


all: compile
.PHONY: all

compile: $(FILES)
	$(CC) $(CFLAGS) $(FILES) -o $(BINFOLD)$(BINNAME) $(LIBFOLD) $(INCFOLD) $(FRAMEWORKS)
.PHONY: compile

run:
	./$(BINFOLD)$(BINNAME) -config "configs/single.xml"
.PHONY: run

report:
	cd docs && latex report.tex && bibtex refs && latex report.tex && latex report.tex && cd ..
.PHONY: run

talk:
	$(FOO)
.PHONY: run

clean:
	rm -f $(BINFOLD)*
.PHONY: clean
