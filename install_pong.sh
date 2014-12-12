#!/bin/sh
DIR=$(pwd)

sudo sed -i ".bak" "\$ i\\
su -c ${DIR}/pong  &\\
" /etc/rc.local