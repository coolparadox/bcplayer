override CFLAGS += -lX11

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
	sudo -iu bcplayer1 mkdir -p ~bcplayer1/bctrace
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

BCPLAY_HEADERS = \
	src/c/bcplay_conf.h \
	src/c/bcplay_hint_types.h \
	src/c/bcplay_kiosk.h \
	src/c/bcplay_log.h \
	src/c/bcplay_screenshot.h \
	src/c/bcplay_screenshot_types.h \
	src/c/bcplay_perception.h \
	src/c/bcplay_perception_types.h \
	src/c/bcplay_perform.h \
	src/c/bcplay_random.h \
	src/c/bcplay_sm.h \
	src/c/bcplay_sm_types.h \

BCPLAY_LIB_SOURCES = \
	src/c/bcplay_kiosk.c \
	src/c/bcplay_kiosk_vars.c \
	src/c/bcplay_screenshot.c \
	src/c/bcplay_screenshot_vars.c \
	src/c/bcplay_perception.c \
	src/c/bcplay_perform.c \
	src/c/bcplay_random.c \
	src/c/bcplay_sm.c \
	src/c/bcplay_sm_vars.c \

BCPLAY_SOURCES = src/c/bcplay.c $(BCPLAY_LIB_SOURCES)

bcplay: $(BCPLAY_SOURCES) $(BCPLAY_HEADERS)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o bcplay $(BCPLAY_SOURCES)

CHECK_SOURCES = test/c/check.c $(BCPLAY_LIB_SOURCES)

check:
	$(CC) $(CPPFLAGS) -Isrc/c $(CFLAGS) -lcmocka -ggdb -o check $(CHECK_SOURCES)
	./check
.PHONY: check

gdb:
	$(MAKE) CPPFLAGS='$(CPPFLAGS) -DBC_SPINLOCK' CFLAGS='$(CFLAGS) -ggdb' clean all
	sudo $(RM) ~bcplayer1/bcplay.pid
	$(MAKE) start
	until test -e ~bcplayer1/bcplay.pid ; do true ; done
	sudo gdb -p $$(sudo cat ~bcplayer1/bcplay.pid)
.PHONY: debug

debug:
	$(MAKE) CPPFLAGS='$(CPPFLAGS) -DDEBUG' clean all
.PHONY: debug

backup:
	rm -r /tmp/bcplay.tar.gz.gpg
	git archive --format=tar HEAD | gzip -c9 | gpg --output /tmp/bcplay.tar.gz.gpg -e -r coolparadox@gmail.com -
	scp /tmp/bcplay.tar.gz.gpg lorandi@cafeina:src/
.PHONY: backup

shot:
	FILE=$$(src/sh/get_screenshot) && echo "$$FILE" >&2 && xwud <$$FILE
.PHONY: shot
.SILENT: shot

