# Makefile for nebula project.

include config.mk

SRCS = src/main.cc src/memory/mbcs/mbc.cc src/memory/mbcs/mbc1.cc
SRCS += src/memory/mmu.cc src/memory/mbcs/romonly.cc src/logging.cc

OBJS = $(SRCS:%.cc=%.o)

.PHONY: all clean distclean $(TARGET)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^

clean:
	rm -f $(OBJS)

distclean: clean
	rm -f $(TARGET) config.mk .depend

depend: .depend

.depend: $(SRCS)
	rm -f .depend
	for file in $(^:%.cc=%); do											\
		$(CXX) $(CFLAGS) -MM $$file.cc -MT $$file.o >> ./.depend;		\
	done

include .depend

# EOF
