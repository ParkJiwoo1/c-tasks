
TARGET = my_mux

SRCS = timeutil.c thread.c main.c
OBJS = $(SRCS:%.c=%.o)

CC = gcc
CFLAGS = -Wall -g

.SUFFIXES: .c .o

$(TARGET):$(OBJS)
			$(CC) $(CFLAGS) -o $@ $(OBJS)



.c.o: 
	 $(CC) -c $(CFLAGS) $<

dep:
	gccmakedep $(SRCS)

clean:
	rm -f $(OBJS) $(TARGET)

depend:
	@for i in $(SRCS); do \
		$(CC) -MM $(CFLAGS) $$i; \
	done > .tmpdepend; \
	mv -f .tmpdepend .depend

ifeq (.depend,$(wildcard .depend))
	include .depend
endif
