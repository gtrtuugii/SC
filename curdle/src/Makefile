
##########################
## DO NOT SUBMIT THIS FILE
##########################


.PHONY: all clean test docs
.DELETE_ON_ERROR:

######
# variables open to being overridden
#
# override by running e.g.
#
#		CC=clang make all

# enable sanitizers
safety_features = \
	-fno-omit-frame-pointer \
	-fsanitize=address \
	-fsanitize=undefined \
	-fsanitize=leak \
	-fstack-protector-strong

SHELL 	= bash
CC 			= gcc
CFLAGS	= -g -std=c11 -pedantic -Wall -Wextra $(safety_features)
LDFLAGS = $(sanitizers)

adjust_score.o:

lib = libcurdle.a

$(lib): adjust_score.o
	ar rcs $@ $^

all: $(lib)

# Requires that `doxygen` and `dot` are on your PATH.
# Intall with `sudo apt-get doxygen graphviz` if not.
docs: Doxyfile *.c *.h
	doxygen

clean:
	-rm -rf *.o $(lib) adjust_score.h docs/html


