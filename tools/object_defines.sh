#!/bin/sh
# converts objname2.txt to defines.
# TODO: fixup duplicates!
awk -F : '{print $2 " (" $1 ")"}' $* | sed -e "s/-/_/g" -e "s/