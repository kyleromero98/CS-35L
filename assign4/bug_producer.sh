#!/bin/bash

tmp=$(mktemp -d)
cd $tmp
touch -d '1918-11-11 11:00 GMT' wwi-armistice
touch now
sleep 1
touch now1
TZ=UTC0 ~/CS35L/assign4/coreutils-with-bug/src/ls -lt --full-time wwi-armistice now now1
cd ~/CS35L/assign4/
rm -fr $tmp
