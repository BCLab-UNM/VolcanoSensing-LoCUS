#!/bin/bash
echo -e "\nArena Size"
for ((i=10; i <= 100; i = i + 10));
do
  echo -e "\n$i:"
  argos3 -c experiments/coverage/arena_size/Fault-Tolerance_$i.xml
done

echo -e "\nSwarm Size"
for i in 1 12 36 73 123 185
do
  echo -e "\n$i:"
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

echo -e "\nfailures"
for failures in 10 20 30 40 50 60 70 80 90 100 110 120 122
do
  echo -e "\nfailures = $failures"
  for ((t = 1; t <= 10; t++));
  do
    echo -e "\ntrial = $t"
    argos3 -c experiments/coverage/failures/Fault-Tolerance_$failures.xml
  done
done