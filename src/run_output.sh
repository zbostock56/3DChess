#!/bin/bash
i=0
while [ $i -lt 1000 ]; do
  ./outop
  i=$((i+1))
  echo "Run: $i"
done
