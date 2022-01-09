CFLAGS=-lX11

all: bcplay
.PHONY: all

clean:
	$(RM) -r bcplay
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
	sudo tail -f /var/log/messages | sed -r -e '/ bcplay\[/!d' -e 's/.* bcplay\[[^ ]* //'
.PHONY: watch
.SILENT: watch

stop:
	sudo -iu bcplayer1 DISPLAY=:1 xdotool getactivewindow | sudo -iu bcplayer1 DISPLAY=:1 xargs -rn1 xdotool windowkill
.PHONY: stop
.SILENT: stop

/home/bcplayer1/bcplay: bcplay
	sudo $(RM) $@
	sudo cp -f $< $@
.PHONY: /home/bcplayer1/bcplay
.SILENT: /home/bcplayer1/bcplay

/home/bcplayer1/.Xauthority: /home/lorandi/.Xauthority
	sudo cp -f $< $@
	sudo chown bcplayer1:bcplayer1 $@
.PHONY: /home/bcplayer1/.Xauthority
.SILENT: /home/bcplayer1/.Xauthority

/home/bcplayer1/xstartup: src/sh/xstartup
	sudo $(RM) $@
	sudo cp -f $< $@
	sudo chown bcplayer1 $@
	sudo chmod 0755 $@
.PHONY: /home/bcplayer1/xstartup
.SILENT: /home/bcplayer1/xstartup

BCPLAY_HEADERS=src/c/bcplay_conf.h
BCPLAY_SOURCES=src/c/bcplay.c

bcplay: $(BCPLAY_SOURCES) $(BCPLAY_HEADERS)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o bcplay $(BCPLAY_SOURCES)

debug:
	$(MAKE) clean
	$(MAKE) CFLAGS='-lX11 -ggdb -DBC_DEBUG' all
	sudo $(RM) ~bcplayer1/bcplay.pid
	$(MAKE) start
	until test -e ~bcplayer1/bcplay.pid ; do true ; done
	sudo gdb -p $$(sudo cat ~bcplayer1/bcplay.pid)
.PHONY: debug

