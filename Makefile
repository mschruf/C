CC = gcc
CDEBUG = -g
CWARNINGS = -Werror -Wall -Wextra -pedantic-errors
CDEFS = #-NDEBUG
COPTS = -std=c99 #-O1
CINCLUDES = #-Ilib
CFLAGS = $(CDEBUG) $(CWARNINGS) $(CDEFS) $(COPTS)

LIB_MATH = -lm
LDFLAGS = #-g

######

SRCS_KSET_EXAMPLE = k-set-example.c lib/k-set.c lib/n-choose-k.c
SRCS_ALL = $(SRCS_KSET_EXAMPLE)
OBJS_KSET_EXAMPLE = $(SRCS_KSET_EXAMPLE:.c=.o)
OBJS_ALL = $(OBJS_KSET_EXAMPLE)
BINS = k-set-example

%.o: %.c
	$(CC) -MD -MP $(CINCLUDES) $(CFLAGS) -c -o $@ $<

all: $(BINS)

k-set-example: $(OBJS_KSET_EXAMPLE)
	$(CC) $(LDFLAGS) -o $@ $(OBJS_KSET_EXAMPLE)

-include $(SRCS:.c=.d)

clean:
	rm -f $(BINS) $(OBJS_ALL) $(SRCS_ALL:.c=.d) core

.PHONY:	all clean
