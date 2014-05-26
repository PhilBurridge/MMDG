# Compiler:
CC = g++

# Flags:
# -Wall -pedantic
 CFLAGS = -std=c++11

# More flags:
ifeq ($(OS), Windows_NT)
	FRAMEWORKS = -lsgct -lopengl32 -lglu32 -lgdi32 -lws2_32 -static-libgcc -static-libstdc++
else ifeq ($(shell uname), Linux)
	FRAMEWORKS = -lsgct -lGL -lGLU -lX11 -lXrandr -lXi -lXxf86vm -pthread
else
	FRAMEWORKS = -lsgct -framework Opengl -framework Cocoa -framework IOKit -stdlib=libstdc++
endif

# Even more flags:
ifeq ($(OS), Windows_NT)
	MKDIR = 
	LIBFOLD = -L"C:\sgct\lib\mingw"
	INCFOLD = -I"C:\sgct\include"
else ifeq ($(shell uname), Linux)
	MKDIR = mkdir -p bin
	LIBFOLD = -L"/usr/lib"
	INCFOLD = -I"/usr/include"
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

#TWO NODES 
run-double:
	./$(BINFOLD)$(BINNAME) -config "configs/two_nodes.xml"
.PHONY: run

run-double-second:
	./$(BINFOLD)$(BINNAME) -config "configs/two_nodes.xml" --slave
.PHONY: run

# CLUSTER TEST
run-fish-cluster:
	./$(BINFOLD)$(BINNAME) -config "configs/fisheye_cluster_test.xml"
.PHONY: run

run-fish-cluster-second:
	./$(BINFOLD)$(BINNAME) -config "configs/fisheye_cluster_test.xml" --slave
.PHONY: run


run-dome:
	./$(BINFOLD)$(BINNAME) -config "configs/dome_sim.xml"
.PHONY: run

run-fisheye:
	./$(BINFOLD)$(BINNAME) -config "configs/single_fisheye_fxaa.xml"
.PHONY: run

talk:
	$(FOO)
.PHONY: run

clean:
	rm -f $(BINFOLD)*
.PHONY: clean
