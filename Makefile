#-----------------------------------------------------------------
QUELL=main.cpp
PROG=main
CC=g++
CFLAGS=-Wall
#-----------------------------------------------------------------
OBJECTS=$(QUELL:.cpp=.o)
all: $(PROG)

$PROG: $(OBJECTS)
	$(CC) $(CFLAGS) $(QUELL) -o$@

%.o: %-cpp
	$(CC) $(CFLAGS) -c $< -o $@
	./main

Prohaupt.pdf: Prohaupt.tex
	pdflatex Prohaupt.tex
	bibtex Prohaupt
	pdflatex Prohaupt.tex
	pdflatex Prohaupt.tex
	rm -f Prohaupt.toc Prohaupt.log Prohaupt.bbl Prohaupt.blg Prohaupt.aux literatur.bib.bak Prohaupt.out Prohaupt.run.xml Prohaupt-blx.bib 

clean: 
	rm -f $(PROG) *.o
