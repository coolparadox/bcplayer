# bcplayer
_A(nother) Bombcrypto bot_

This is a personal project for a bot in pure C that runs in the Linux OS.

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
- Update the following configurations in src/c/bcplay_conf.h:
  - BCPLAYER_USER_ID with the ID of user bcplayer1 in your system.
  - BC_METAMASK_PASSWORD with the password to the Metamask Firefox extension for user bcplayer1. 
- Configure sudo superpowers for the user that will start the bot (ie, NOT the bcplayer1 user).

## Run

### Start play session
- make debug start

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
- [Demonstration video](https://coolparadox.github.io/bcplayer/bcplay_demo_20220326.mp4)

# Licensing
GPL-v3; see https://www.gnu.org/licenses/gpl-3.0.en.html
