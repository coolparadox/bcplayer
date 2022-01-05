.PHONY: all clean tags setuid

all: build/bcplay

clean:
	$(RM) -r build

tags:
	$(RM) tags
	ctags -R --c-types=+p /usr/include
	ctags -aR src/c

play: build/bcplay
	cp -ft /tmp $<
	sudo -iu bcplayer1 /tmp/bcplay

BCPLAY_SOURCES=$(wildcard src/c/bcplay/*.c)

build/bcplay: $(BCPLAY_SOURCES) $(wildcard src/c/include/bcplay/*.h)
	mkdir -p build
	$(CC) $(CPPFLAGS) $(CFLAGS) -o build/bcplay $(BCPLAY_SOURCES)
