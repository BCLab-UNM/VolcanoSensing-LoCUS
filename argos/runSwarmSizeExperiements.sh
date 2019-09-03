#!/bin/bash
echo -e "\nSwarm Size"
for i in 1 11 12 35 36 72 73 122 123 184 185
do
  echo -e "\nswarm size = $i"
  argos3 -c experiments/coverage/swarm_size/$i.xml
done