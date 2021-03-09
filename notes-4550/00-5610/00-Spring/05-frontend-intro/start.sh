#!/bin/bash

export PORT=5100

cd ~/www/memory
./bin/memory stop || true
./bin/memory start

