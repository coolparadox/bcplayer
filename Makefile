all: build/bcplay
.PHONY: all

clean:
	$(RM) -r build
.PHONY: clean

tags:
	$(RM) tags
	ctags -R --c-types=+p /usr/include
	ctags -aR src/c
.PHONY: tags

start: /home/bcplayer1/bcplay /home/bcplayer1/.Xauthority /home/bcplayer1/xstartup
	sudo -iu bcplayer1 vncserver :1 -geometry 960x600 -autokill -xstartup ~bcplayer1/xstartup -SecurityTypes None -AcceptSetDesktopSize=0
.PHONY: start
.SILENT: start

watch:
	vncviewer -Shared=0 -ViewOnly :1 &
.PHONY: watch
.SILENT: watch

stop:
	sudo -iu bcplayer1 DISPLAY=:1 xdotool getactivewindow | sudo -iu bcplayer1 DISPLAY=:1 xargs -rn1 xdotool windowkill
.PHONY: stop
.SILENT: stop

/home/bcplayer1/bcplay: build/bcplay
	sudo cp -f $< $@
.PHONY: /home/bcplayer1/bcplay
.SILENT: /home/bcplayer1/bcplay

/home/bcplayer1/.Xauthority: /home/lorandi/.Xauthority
	sudo cp -f $< $@
.PHONY: /home/bcplayer1/.Xauthority
.SILENT: /home/bcplayer1/.Xauthority

/home/bcplayer1/xstartup: src/sh/xstartup
	sudo cp -f $< $@
	sudo chown bcplayer1 $@
	sudo chmod 0755 $@
.PHONY: /home/bcplayer1/xstartup
.SILENT: /home/bcplayer1/xstartup

BCPLAY_SOURCES=$(wildcard src/c/bcplay/*.c)

build/bcplay: $(BCPLAY_SOURCES) $(wildcard src/c/include/bcplay/*.h)
	mkdir -p build
	$(CC) $(CPPFLAGS) $(CFLAGS) -o build/bcplay $(BCPLAY_SOURCES)
