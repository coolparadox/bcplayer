override CFLAGS += -lm -lX11 -lxdo

all: bcplay
.PHONY: all

clean:
	$(RM) bcpack bcplay check src/ppm/bcplay_pack_vars.c
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
	sudo tail -n1000 -f /var/log/messages | sed -r -e '/ bcplay\[/!d' -e 's/.* bcplay\[[^ ]* //'
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
	src/c/bcplay_canvas.h \
	src/c/bcplay_canvas_types.h \
	src/c/bcplay_conf.h \
	src/c/bcplay_hint_types.h \
	src/c/bcplay_kiosk.h \
	src/c/bcplay_log.h \
	src/c/bcplay_screenshot.h \
	src/c/bcplay_perception.h \
	src/c/bcplay_perception_types.h \
	src/c/bcplay_perform.h \
	src/c/bcplay_random.h \
	src/c/bcplay_planning.h \
	src/c/bcplay_planning_types.h \

BCPLAY_LIB_SOURCES = \
	src/c/bcplay_canvas.c \
	src/c/bcplay_canvas_vars.c \
	src/c/bcplay_kiosk.c \
	src/c/bcplay_kiosk_vars.c \
	src/c/bcplay_screenshot.c \
	src/c/bcplay_perception.c \
	src/c/bcplay_perform.c \
	src/c/bcplay_random.c \
	src/c/bcplay_planning.c \
	src/c/bcplay_planning_vars.c \
	src/ppm/bcplay_pack_vars.c \

BCPACK_PPM_SOURCES = \
	src/ppm/appsite_connect_wallet.ppm \
	src/ppm/automatic_exit_label.ppm \
	src/ppm/game_characters_full_selected1.ppm \
	src/ppm/game_characters_full_selected2.ppm \
	src/ppm/game_characters_full_unselected.ppm \
	src/ppm/game_characters_title.ppm \
	src/ppm/game_error_title.ppm \
	src/ppm/game_exit.ppm \
	src/ppm/game_kiosk_fullscreen.ppm \
	src/ppm/game_kiosk_title.ppm \
	src/ppm/game_paused_heroes.ppm \
	src/ppm/game_selection_treasure_hunt.ppm \
	src/ppm/kiosk_updated_not_now.ppm \
	src/ppm/loading.ppm \
	src/ppm/metamask_unlock_button.ppm \
	src/ppm/metamask_unlock_mascot.ppm \
	src/ppm/metamask_signature_request_origin.ppm \
	src/ppm/metamask_signature_request_sign_button.ppm \
	src/ppm/metamask_signature_request_title.ppm \

BCPLAY_SOURCES = src/c/bcplay.c $(BCPLAY_LIB_SOURCES)

bcplay: $(BCPLAY_SOURCES) $(BCPLAY_HEADERS)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o bcplay $(BCPLAY_SOURCES)

CHECK_SOURCES = test/c/check.c $(BCPLAY_LIB_SOURCES)

check: $(CHECK_SOURCES)
	$(CC) $(CPPFLAGS) -DDEBUG -Isrc/c $(CFLAGS) -lcmocka -ggdb -O0 -o check $(CHECK_SOURCES)
	./check
.PHONY: check

BCPACK_SOURCES = \
	src/c/bcplay_pack.c \
	src/c/bcplay_canvas.c \
	src/c/bcplay_canvas_vars.c \

bcpack: $(BCPACK_SOURCES) $(BCPLAY_HEADERS)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o bcpack $(BCPACK_SOURCES)

src/ppm/bcplay_pack_vars.c: bcpack $(BCPACK_PPM_SOURCES)
	xargs -n1 ./bcpack >$@ <<<'$(BCPACK_PPM_SOURCES)'

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

pause:
	sudo -iu bcplayer1 touch /home/bcplayer1/bcplay.pause
.PHONY: pause

unpause:
	sudo -iu bcplayer1 $(RM) /home/bcplayer1/bcplay.pause
.PHONY: unpause
