#!/bin/sh
# converts objname2.txt to defines.
# TODO: fixup duplicates!
awk -F : '{print $2 " (" $1 ")"}' $* | sed -e "s/-/_/g" -e "s///" -e "s/([^ 1-9)]\+/(/" -e "s/ /_/g" | sed "s/_(/ (/" | tr a-z A-Z | sed -e "s/^/#define OBJ/"
