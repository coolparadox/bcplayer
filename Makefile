.PHONY: all clean tags

all: bcplay

clean:
	$(RM) bcplay

tags:
	$(RM) tags
	ctags -R --c-types=+p /usr/include
	ctags -aR src/c

BCPLAY_SOURCES=$(wildcard src/c/bcplay/*.c)

bcplay: $(BCPLAY_SOURCES) $(wildcard src/c/include/bcplay/*.h)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o bcplay $(BCPLAY_SOURCES)
