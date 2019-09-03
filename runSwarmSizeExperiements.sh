#!/bin/bash
echo -e "\nArena Size"
for ((i=10; i <= 100; i = i + 10));
do
  echo -e "\narena size = $i:"
  argos3 -c experiments/coverage/arena_size/Fault-Tolerance_$i.xml
done

echo -e "\nSwarm Size"
for i in 1 11 12 35 36 72 73 122 123 184 185
do
  echo -e "\nswarm size = $i:"
  argos3 -c experiments/coverage/swarm_size/$i.xml
done

echo -e "\nho"
for rmax in 2 4 8 16
do
  echo -e "\nrmax = $rmax"
  for ((i = 1; i <= $rmax; i++));
  do
    echo -e "\nrmin = $i"
    argos3 -c experiments/coverage/rho/Fault-Tolerance_$i\_$rmax.xml
  done
done