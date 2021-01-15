#!/bin/bash

for MD in $(find . -name "notes.md")
do
    DN=$(dirname $MD)
    mv "$MD" "$DN/_index.md"
done
