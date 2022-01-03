.PHONY: all clean

all: bcplay

clean:
	$(RM) bcplay

BCPLAY_SOURCES=$(wildcard src/c/bcplay/*.c)

bcplay: $(BCPLAY_SOURCES) $(wildcard src/c/include/bcplay/*.h)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o bcplay $(BCPLAY_SOURCES)
