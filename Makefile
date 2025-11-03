PROG = main
# CC = gcc
DEPS = cardfunctions.h menu.h safeinput.h waitforenter.h
SRC = main.c cardfunctions.c menu.c safeinput.c waitforenter.c
CFLAGS = -Wall -Werror -g
OUTPUTDIR = obj
OBJS = $(addprefix $(OUTPUTDIR)/, $(SRC:.c=.o))

all: $(OUTPUTDIR) $(PROG)

$(PROG): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

$(OUTPUTDIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(OUTPUTDIR):
	@mkdir -p $(OUTPUTDIR)

# på Mac finns inte del
clean:
	@rm -rf $(OUTPUTDIR) $(PROG)

.PHONY: all clean