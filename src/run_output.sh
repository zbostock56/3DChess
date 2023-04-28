#!/bin/bash
i=0
while [ $i -lt 100000 ]; do
  ./outop
  i=$((i+1))
  echo "Run: $i"
done
