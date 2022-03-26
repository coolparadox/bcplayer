# bcplayer
_A(nother) Bombcrypto bot_

This is a personal project for a bot in pure C for Linux.

## Requirements
- libX11 1.7.3
- xdotool 3
- TigerVNC 1.9.0
- Firefox 91.7.0
- GNU make
- gcc
- sudo

## Setup
- Create user bcplayer1.
- Make sure it can play Bombcrypto manually; see doc/metamask/smart_chain.txt
- Update BCPLAYER_USER_ID in src/c/bcplay_conf.h to bcplayer1's user ID.
- Configure sudo superpowers for the user that will start the bot (NOT the bcplayer1 user).

## Run

### Start play session
- make debug play

### Watch play session
- make watch

### Stop play session
- make stop

### Pause/unpause bot temporarily
- make pause
- make unpause

## Tested under / with
- Gentoo Linux 64 bit under WSL 2
- Bombcrypto version 37

# Licensing
GPL-v3; see https://www.gnu.org/licenses/gpl-3.0.en.html
