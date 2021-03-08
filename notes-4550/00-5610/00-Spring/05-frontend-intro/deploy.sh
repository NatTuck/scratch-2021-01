#!/bin/bash

echo "in class, we're typing the commands"
exit 3

export PORT=5100
export MIX_ENV=prod
export GIT_PATH=/home/memory/src/memory 

PWD=`pwd`
if [ $PWD != $GIT_PATH ]; then
	echo "Error: Must check out git repo to $GIT_PATH"
	echo "  Current directory is $PWD"
	exit 1
fi

if [ $USER != "memory" ]; then
	echo "Error: must run as user 'memory'"
	echo "  Current user is $USER"
	exit 2
fi

mix deps.get
(cd assets && npm install)
(cd assets && ./node_modules/brunch/bin/brunch b -p)
mix phx.digest
MIX_ENV=prod mix release --env=prod

mkdir -p ~/www
mkdir -p ~/old

NOW=`date +%s`
if [ -d ~/www/memory ]; then
	echo mv ~/www/memory ~/old/$NOW
	mv ~/www/memory ~/old/$NOW
fi

mkdir -p ~/www/memory
REL_TAR=~/src/memory/_build/prod/rel/memory/releases/0.0.1/memory.tar.gz
(cd ~/www/memory && tar xzvf $REL_TAR)

crontab - <<CRONTAB
@reboot bash /home/memory/src/memory/start.sh
CRONTAB

#. start.sh
