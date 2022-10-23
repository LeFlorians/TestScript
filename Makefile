# specify destination files
TARGET := lang
TEST_INPUT = test.txt

# general compiler flags
CC := gcc
LIBS := -lm
CFLAGS := -Wall -pg -Og -g -Wno-switch -Wno-return-type

# paper flags
JOBNAME := paper
paper_raw: JOBNAME := paper_raw

# apply optimizations for the release target
release: CFLAGS := -Wall -O3

# activate debug output for test release
test: CFLAGS += -DDEBUG

# define recursive wildcard function
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

# search for .c files in ./src
SRCS := $(call rwildcard,src/,*.c)
OBJS := $(SRCS:%.c=%.o)

.PHONY: all clean

all: $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) $(OBJS) -o $(TARGET)

release: all

test: all
	clear
	$(file >> $(TEST_INPUT))
	./$(TARGET) $(TEST_INPUT)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

paper_raw: paper 

paper: clean paper.tex paper/bibliography.bib
	@(git ls-tree -r main --name-only |\
		grep -E "\.h$$|\.c$$|Makefile$$|\.gperf$$|.gitignore$$|\.md$$" |\
		tr '\n' ',' | sed '$$s/,$$//' > paper/files.txt &&\
		pdflatex -jobname=$(JOBNAME) paper.tex && biber $(JOBNAME) &&\
		pdflatex -jobname=$(JOBNAME) paper.tex && pdflatex -jobname=$(JOBNAME) paper.tex)
	@echo 'Done!'

# target to generate mapop.c
# requires gperf to be installed
mapop:
	gperf --output-file=./src//interpreter/mappings/mapop.c ./mapop.gperf

clean:
	@rm -f -- *.toc *.aux *.run.xml *.log *.bcf \
		*.bbl *.blg *.out *.pdf *.tex.bbl *.tex.blg \
		paper/files.txt *.lof \
		$(TARGET) $(TARGET).exe gmon.out $(call rwildcard,src/,*.o)
