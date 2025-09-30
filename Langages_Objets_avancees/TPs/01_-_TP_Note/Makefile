# =============================================================================
# Makefile for TP STL
# =============================================================================
OSTYPE = $(shell uname -s)

# -----------------------------------------------------------------------------
# Tools
# -----------------------------------------------------------------------------
COMPILER = g++
ifeq ($(findstring Darwin,$(OSTYPE)),Darwin)
	# Mac OS
	COMPILER = clang++
endif
LINKER = $(COMPILER)
# Listings with Ascii2Postscript & Postscript2PDF
SOURCE_ENCODING=UTF-8
PRINT_ENCODING=latin1
A2PSTOOL = a2ps-utf8
A2PSCOMMAND = $(A2PSTOOL) -2 --file-align=fill --line-numbers=1 --font-size=10 \
	--chars-per-line=90 --tabsize=4 --pretty-print --highlight-level=heavy \
	--prologue="gray"
ENSCRIPTTOOL = enscript
ENSCRIPTCOMMAND = $(ENSCRIPTTOOL) --nup 2 --tabsize=4 --encoding=$(PRINT_ENCODING) \
	--font=Courier8 \
	--continuous-page-numbers --borders --line-numbers --highlight=cpp \
	--filter="iconv -f $(SOURCE_ENCODING) -t $(PRINT_ENCODING) %s"
PSCOMMAND=$(ENSCRIPTCOMMAND)
PS2PDF = ps2pdf -dPDFX=true -sPAPERSIZE=a4
# Documentation
DOCTOOL = doxygen
# OS specific
ifeq ($(findstring NT,$(OSTYPE)),NT)
	# Windows systems
	SFX=.exe
else
	# Linux, FreeBSD or Darwin or others
	SFX=
endif
ARCH = zip
ARCHOPT =
ARCHEXT = .zip
# current date (used in archive filename)
DATE = $(shell date +%Y-%m-%d)
# Packages manager
PKGCONFIG = pkg-config
# code static analysis
SPLINT = splint
# boost lib location (above include and lib directories)
ifeq ($(findstring Darwin,$(OSTYPE)), Darwin)
	BOOST_LIB = /opt/local
else
	ifeq ($(findstring Linux,$(OSTYPE)), Linux)
		BOOST_LIB = /usr
	else
		BOOST_LIB =
	endif
endif

TOOLS = $(COMPILER) $(A2PSTOOL) $(ENSCRIPTTOOL) $(DOCTOOL) $(PKGCONFIG) $(ARCH) $(SPLINT)

# -----------------------------------------------------------------------------
# Flags and packages
# -----------------------------------------------------------------------------

# Compilation flags
CFLAGS = -std=c++11 -W -Wall -g
# Do not allow copy elision : -fno-elide-constructors
# Optimize flags : -O2 or -O3
# C++ 11 : -std=c++11

# linkage flags
LFLAGS =

# common libraries names (i.e.: m for math, z for zlib, ...)
LIBNAMES =
# common libraries linkage flags (i.e.: -lm -lz -l...)
CLIBS = $(foreach name, $(LIBNAMES),-l$(name))
# boost lib and includes
ifneq ($(BOOST_LIB),)
	LIBS = -L$(BOOST_LIB)/lib
	INCLUDES = -I$(BOOST_LIB)/include
endif
# Used packages list managed by pkg-config
# (check installed packages with pkg-config --list-all)
PACKAGES =
# Compile and Link flags associated with these packages (if any)
#	- INCLUDES are used during compilation step
#	- LIBS are used during linkage step
ifneq ($(PACKAGES),)
	LIBS := $(shell $(PKGCONFIG) --libs $(PACKAGES)) $(LIBS)
	INCLUDES := -I. $(shell $(PKGCONFIG) --cflags $(PACKAGES)) $(INCLUDES)
else
	INCLUDES := -I. $(INCLUDES)
endif

# Special linkage flags for MinGW on Windows
ifeq ($(findstring NT,$(OSTYPE)), NT)
	# Enable auto import is needed by 3rd party Frameworks on windows in order
	# to import symbols from DLLs
	LFLAGS := -Wl,--enable-auto-import $(LFLAGS)
endif

# -----------------------------------------------------------------------------
# Project name and sources (check these variables with "make check")
# -----------------------------------------------------------------------------

# Project name (for listing and archive purpose only)
PROJECT = TP_STL
# Project nature (c or cpp)
EXT=.cpp
# List of classes or modules (couples of .h/.c[pp]) WITHOUT extensions
MODULES = BoundedRandomGenerator StatFunctor TypeException ExternalCheck RCheck
# List of programs (.c[pp] files containing main function) WITHOUT extensions
MAINS = main
# List of c or c++ header files
HEADERS = $(foreach name, $(MODULES), $(name).h)
# List of c or c++ source files
SOURCES = $(foreach name, $(MODULES), $(name)$(EXT)) \
$(foreach name, $(MAINS), $(name)$(EXT))
# List of all sources files (Makefile + .h and .c[pp])
ALLSOURCES = $(foreach name, $(MODULES),$(name).h $(name)$(EXT)) \
$(foreach name, $(MAINS), $(name)$(EXT))
# List of all source files (with .h replaced by .hpp for correct printing with a2ps)
CPPSOURCES = $(ALLSOURCES:.h=.hpp)
# Additional files (docs, readme, etc.)
ADDITIONAL = check_TP_STL.R TP_STL.pdf
# Object files to be linked
# Classes or Modules objects
MOBJECTS = $(foreach name, $(MODULES), $(name).o)
# Main programs objects
POBJECTS = $(foreach name, $(MAINS), $(name).o)
# Programs to be generated
PROGRAMS = $(POBJECTS:.o=$(SFX))
# PS and PDF listings
LISTING = $(PROJECT)
# All files for the archive
ALLFILES = Makefile $(ALLSOURCES) $(ADDITIONAL)
# Repertoire Listings
LISTDIR = listings
# Repertoire Archives
ARCHDIR = archives

# Phony targets (don't need file check)
.PHONY : clean realclean doc ps pdf archive edit check checkenv
# suffixes
.SUFFIXES : $(EXT).o

# -----------------------------------------------------------------------------
# targets
# -----------------------------------------------------------------------------
all : .depend $(PROGRAMS)

other : pdf doc archive

# Automatic dependencies generation .o: .h .c[pp]
.depend :
	@echo building dependencies ...
	$(COMPILER) -MM $(CFLAGS) $(SOURCES) $(INCLUDES) > .depend
	@echo done.

# dependencies include
-include .depend

# source files compilation generic target
$(EXT).o:
	@echo compiling $< file ...
	$(COMPILER) $(CFLAGS) -c $< $(INCLUDES)
	@echo done.

# programs link generic target
%$(SFX) : %.o $(MOBJECTS)
	@echo Linking Executable $@ ...
	$(LINKER) $(LFLAGS) -o $@ $^ $(LIBS) $(CLIBS)
	@echo done

# run the program(s)
run : $(PROGRAMS)
	@$(foreach prgm, $(PROGRAMS), echo executing $(prgm); $(prgm);)

# cleaning object files, listings, documentation and programs
clean : unlinks
	@echo cleaning obj, listing and doc files ...
#	@echo cleaning *.o *~ .depend $(PROGRAMS) $(LISTING).ps $(LISTING).pdf doc
	rm -rf *.o *~ .depend $(PROGRAMS) $(LISTDIR)/* doc
	@echo done.

# cleaning also generated archives
realclean : clean
	@echo cleaning archives files ...
#	@echo cleaning $(PROJECT)-*$(ARCHEXT)
	rm -f $(ARCHDIR)/$(PROJECT)-*$(ARCHEXT)
	@echo done.

# Creating links for C++ header files
links : unlinks
	@echo creating symbolic links for C++ header files
	$(foreach header, $(HEADERS), ln -s $(header) $(header)pp;)
	@echo done.

# Destroying links for C++ header files
unlinks :
	@echo cleaning symbolic links for C++ header files
	rm -f *.hpp
	@echo done.

$(LISTDIR) :
	mkdir $(LISTDIR)

# Postscript listing
ps : $(LISTDIR) links $(CPPSOURCES)
	@echo generating Postscript listing with postcript tool...
	$(PSCOMMAND) --output=$(LISTDIR)/$(LISTING).ps $(CPPSOURCES);
	@echo done.

# PDF listing
pdf : ps
	@echo generating PDF listing ...
	$(PS2PDF) $(LISTDIR)/$(LISTING).ps $(LISTDIR)/$(LISTING).pdf
	@echo done.

# editing files
edit :
	@echo editing files ...
	@echo editor is : $(EDITOR)
	"$(EDITOR)" $(ALLFILES) &
	@echo done.

# generating documentation in doc folder
doc : Doxyfile $(ALLSOURCES)
	@echo generating documentation ...
	$(DOCTOOL) Doxyfile
#	make -C doc/latex
	@echo done.

# backup important files in dated archive
$(ARCHDIR) :
	mkdir $(ARCHDIR)

archive : $(ARCHDIR) $(ALLFILES) pdf
	@echo generating archive ...
	$(ARCH) $(ARCHOPT) $(ARCHDIR)/$(PROJECT)-$(DATE)$(ARCHEXT) $(ALLFILES) \
	$(LISTDIR)/$(LISTING).pdf
	@echo done.

# Check project variables
check :
	@echo Checking project variables ...
	@echo project name: $(PROJECT)
	@echo boost lib: $(BOOST_LIB)
	@echo includes: $(INCLUDES)
	@echo libs: $(LIBS)
	@echo link flags: $(LFLAGS)
	@echo modules: $(MODULES)
	@echo main programs: $(MAINS)
	@echo header files: $(HEADERS)
	@echo source files: $(SOURCES)
	@echo All source files: $(ALLSOURCES)
	@echo All CPP source files: $(CPPSOURCES)
	@echo Classes and Modules objects: $(MOBJECTS)
	@echo Main programs objects: $(POBJECTS)
	@echo Main programs targets: $(PROGRAMS)
	@echo compile flags: $(CFLAGS) \<file\> $(INCLUDES)
	@echo link flags: $(LFLAGS) \<output\> $(LIBS) $(CLIBS)
	@echo done.

# Check tools versions and locations, and also required packages versions
checkenv :
	@echo required tools : $(TOOLS)
	@$(foreach tool, $(TOOLS),\
	echo --------------------------------------------------------------------;\
	echo $(tool) version   : `$(tool) --version`;\
	echo $(tool) location  : `which $(tool)`;)
	@echo --------------------------------------------------------------------;
	@echo required packages : $(PACKAGES)
	@$(foreach package, $(PACKAGES), echo "$(package) version : " \
	`$(PKGCONFIG) --modversion $(package)`;)
