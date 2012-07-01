# Makefile for nebula project.

include config.mk

SRCS = src/main.cc src/memory/mbcs/mbc.cc src/memory/mbcs/mbc1.cc
SRCS += src/memory/mmu.cc src/memory/mbcs/romonly.cc src/logging.cc
SRCS += src/z80/z80opcodes.cc src/z80/z80.cc src/graphics/gpu.cc
SRCS += src/graphics/bgwtile.cc src/graphics/timer.cc src/graphics/sprites.cc
SRCS += src/z80/z80cbopcodes.cc src/z80/interrupts.cc
SRCS += src/z80/z80disassembler.cc

OBJS = $(SRCS:%.cc=%.o)

.PHONY: all clean distclean

all: defs $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(OBJS)

distclean: clean
	rm -f $(TARGET) config.mk .depend src/z80/genOpcodes.hh

defs: ./src/z80/gen_opcodes.py
	./src/z80/gen_opcodes.py

depend: .depend

.depend: $(SRCS)
	rm -f .depend
	for file in $(^:%.cc=%); do												\
		$(CXX) $(CFLAGS) -MM $$file.cc -MT $$file.o >> .depend;				\
	done

include .depend

# EOF
