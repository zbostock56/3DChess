#!/bin/bash
i=0
while [ $i -lt 10000 ]; do
  ./outop
  i=$((i+1))
  echo "Run: $i"
done
