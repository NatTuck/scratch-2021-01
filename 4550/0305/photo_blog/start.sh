#!/bin/bash

export MIX_ENV=prod
export PORT=4793

CFGD=$(readlink -f ~/.config/hangman)

if [ ! -e "$CFGD/base" ]; then
    echo "run deploy first"
    exit 1
fi

DB_PASS=$(cat "$CFGD/db_pass")
export DATABASE_URL=ecto://photos:$DB_PASS@localhost/photo_blog_prod

SECRET_KEY_BASE=$(cat "$CFGD/base")
export SECRET_KEY_BASE

_build/prod/rel/photo_blog/bin/photo_blog start
