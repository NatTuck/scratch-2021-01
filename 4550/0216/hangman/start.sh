#!/bin/bash

export MIX_ENV=prod
export PORT=4801

CFGD=$(readlink -f ~/.config/hangman)

if [ ! -e "$CFGD/base" ]; then
    echo "run deploy first"
    exit 1
fi

SECRET_KEY_BASE=$(cat "$CFGD/base")
export SECRET_KEY_BASE

_build/prod/rel/hangman/bin/hangman start
