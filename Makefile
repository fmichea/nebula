# Makefile for nebula project.

include config.mk

SRCS = src/main.cc

OBJS = $(SRCS:%.cc=%.o)

.PHONY: all clean distclean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^

clean:
	rm -f $(OBJS)

distclean: clean
	rm -f $(TARGET) config.mk

# EOF
