#!/bin/bash
i=0
while [ $i -lt 200 ]; do
  ./outop
  i=$((i+1))
done
