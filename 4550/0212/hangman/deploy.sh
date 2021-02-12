#!/bin/bash

export MIX_ENV=prod
# Common port range for this is 4000-10,000
# Valid port range for a user app to listen
# on is something like 1025-32767
export PORT=4801
export SECRET_KEY_BASE=insecure

mix deps.get --only prod
mix compile

CFGD=$(readlink -f ~/.config/hangman)

if [ ! -d "$CFGD" ]; then
    mkdir -p "$CFGD"
fi

if [ ! -e "$CFGD/base" ]; then
    mix phx.gen.secret > "$CFGD/base"
fi

SECRET_KEY_BASE=$(cat "$CFGD/base")
export SECRET_KEY_BASE

npm install --prefix ./assets
npm run deploy --prefix ./assets
mix phx.digest

mix release
