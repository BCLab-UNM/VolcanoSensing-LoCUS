#!/bin/bash
echo -e "\nArena Size"
for ((i=10; i <= 100; i = i + 10));
do
  echo -e "\narena size = $i"
  argos3 -c experiments/coverage/arena_size/Fault-Tolerance_$i.xml
done