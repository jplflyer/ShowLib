#======================================================================
# GNU MAKE REQUIRED!
#
# Notice VPATH. This is a GNU Make thing that lets us put our source
# files where we want. I've broken them based on where they're running.
# I may later break them up even more for one directory for each program.
# Anyway, we search for .cpp files in the list of directories, and it
# just magically works.
#======================================================================

# This needs to be above the include, as he has targets.
.PHONY: all
all: directories bin lib bins

# We include a standard base with lots of boilerplate.
include Makefile-Base

TEST_SRC=tests
TEST_BIN=test-bin${MACAPPEND}

LIB_DIR=showlib

VPATH := ${SRCDIR}:${LIB_DIR}:${TEST_SRC}
INCLUDES += -I./include -I.
CXXFLAGS += ${LOG4CPP_USE_FOUR_ARG_CONSTRUCTOR}
LDFLAGS += -L. -L./lib -lz -llog4cplus -lcppunit -lpthread -lstdc++ -lm -ldl

LIB_NOSORT := $(wildcard ${LIB_DIR}/*.cpp)
LIB_SRC := $(sort ${LIB_NOSORT})
LIB_OBJ := $(patsubst ${LIB_DIR}/%.cpp,${OBJDIR}/%.o,${LIB_SRC})

LIBNAME=show${MACAPPEND}
LIB=lib/libshow${MACAPPEND}.a
LIB_ARGS= -cvrU
ifeq ($(UNAME), Darwin)
	LIB_ARGS= -cvr
endif

INSTALL_BASE=/usr/local

#======================================================================
# Top-level targets.
#======================================================================

Makefile: ;

# Clean the contents of the subdirs.
.PHONY: clean
clean:
	rm -f ${DEPDIR}/* ${OBJDIR}/* ${LIB} ${TEST_BIN}/*

# Clean out the subdirs entirely.
.PHONY: reallyclean
reallyclean:
	rm -rf ${DEPDIR} ${OBJDIR}

.PHONY: echo
echo:
	@echo CXXFLAGS is ${CXXFLAGS}
	@echo LIB_OBJ is ${LIB_OBJ}
	@echo VPATH = ${VPATH}

ifneq (,$(wildcard ${DEPDIR}/*))
include .d/*
endif

#======================================================================
# Making the library.
#======================================================================
lib: ${LIB}

${LIB}: ${LIB_OBJ}
	@mkdir -p lib
	ar ${LIB_ARGS} ${LIB} ${LIB_OBJ}
	ranlib ${LIB}

#======================================================================
# I have a small number of programs.
#======================================================================
bin:
	mkdir -p bin

bins: bin/BCrypt-Password

bin/BCrypt-Password: ${OBJDIR}/BCrypt-Password.o ${LIB}
	$(CXX) ${OBJDIR}/BCrypt-Password.o -L. -l${LIBNAME} ${LDFLAGS} -lbcrypt $(OUTPUT_OPTION)

#======================================================================
# Installation.
#======================================================================
.PHONY: install
install: ${LIB} bins
	mkdir -p ${INSTALL_BASE}/include/showlib
	cp ${LIB_DIR}/*.h ${INSTALL_BASE}/include/showlib
	cp -R include/* ${INSTALL_BASE}/include
	cp ${LIB} ${INSTALL_BASE}/lib
	cp Makefile-Base ${INSTALL_BASE}/etc
	cp bin/BCrypt-Password ${INSTALL_BASE}/bin

#======================================================================
# Tests
#======================================================================

tests: ${TEST_BIN} ${TEST_BIN}/TestJSON ${TEST_BIN}/TestStrings

${TEST_BIN}:
	mkdir -p ${TEST_BIN}

${TEST_BIN}/TestJSON: ${OBJDIR}/TestJSON.o ${OBJDIR}/main-test.o ${LIB}
	$(CXX) ${OBJDIR}/TestJSON.o ${OBJDIR}/main-test.o -L. ${LDFLAGS} -l${LIBNAME} $(OUTPUT_OPTION)

${TEST_BIN}/TestStrings: ${OBJDIR}/TestStrings.o ${OBJDIR}/main-test.o ${LIB}
	$(CXX) ${OBJDIR}/TestStrings.o ${OBJDIR}/main-test.o -L. ${LDFLAGS} -l${LIBNAME} $(OUTPUT_OPTION)
