##########################
## DO NOT SUBMIT THIS FILE
##########################

.PHONY: all clean test docs

.DELETE_ON_ERROR:

check_c_flags 	:= $(shell pkg-config --cflags check)
check_ld_flags 	:= $(shell pkg-config --libs check)

######
# variables open to being overridden
#
# override by running e.g.
#
#		CC=clang make test

safety_features = \
	-fno-omit-frame-pointer \
	-fsanitize=address \
	-fsanitize=undefined \
	-fsanitize=leak \
	-fstack-protector-strong

SHELL 	= bash
CC 			= gcc
CFLAGS 	= -g -I. -std=c11 -pedantic -Wall -Wextra $(check_c_flags) $(safety_features)
LDFLAGS = -L. -lcurdle $(check_ld_flags) $(safety_features)

test_modules = \
	check_adjust_score

# one of
# "silent", "minimal", "normal", "verbose"
test_verbosity=verbose

# uncomment to disable libcheck from
# forking (makes using gdb easier)
#
#ck_fork=no

# uncomment, and set this to the name of some test case in order
# to run *only* that test case
#
#ck_run_case=arithmetic_works

####
# generate a header file from ../src *.c and *.h
# files
# (normally a bad practice)

makeheaders:
	curl -L -o $@.c 'https://fossil-scm.org/home/raw/ca90c1e2460d79e48f97c2aac60a39bcb8024e394de7cff5164c9dcbc96ce529?at=makeheaders.c'
	gcc -o $@ $@.c

adjust_score.h: ../src/*.[ch] makeheaders
	set -e; \
	srcdir="$$PWD"; \
	tmp=`mktemp -d --tmpdir curdle-tmp.XXXXXXXXXX`; \
	cp $^ $$tmp; \
	cd $$tmp; \
	sed -i '/CURDLE/ s|^|//|' *.h; \
	./makeheaders -h *.[ch] | \
		sed 's|^typedef.*|struct score_record;|' > "$$srcdir/"$@

####
# files copied from src directory
# (also not great practice)

libcurdle.a: ../src/*.[ch]
	cd ../src && make $@
	cp -a ../src/$@ $@

####
# local files

# how to make a .c file from a .ts file
%.c: %.ts
	checkmk $< > $@

# for each test module, the .c file is a target
$(foreach test_module,$(test_modules),$(eval $(test_module).c:))

# for each test module, the .o file is a target
$(foreach test_module,$(test_modules),$(eval $(test_module).o:))

# for each test module, the program is a target
# and test_utils.o is a dependency
$(foreach test_module,$(test_modules),$(eval $(test_module): test_utils.o))

# alter bad builtin rule for linking
# (puts libs in bad order)
%: %.o
	$(CC) -o $@ $^ $(LDFLAGS)

test_c_files = $(test_modules:=.c)

test_o_files = $(test_modules:=.o)

test_progs = $(test_modules)

all: $(test_progs)

test_utils.o: test_utils.h

check_adjust_score.o: adjust_score.h libcurdle.a

test: all
	@set -e; \
	for test_prog in $(test_progs); do \
		: "set env vars then run test prog" ; \
		$(if $(ck_run_case),CK_RUN_CASE=$(ck_run_case),) \
			CK_FORK=$(ck_fork) \
			CK_VERBOSITY=$(test_verbosity) \
				./$$test_prog; \
	done

clean:
	-rm -rf *.o $(test_c_files) $(test_progs) adjust_score.h

extra-clean: clean
	-rm -rf libcurdle.a
	-(cd ../src; make clean)




