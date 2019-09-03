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

echo -e "\nfailures = 0"
echo -e "\ntrial = 1"
argos3 -c experiments/coverage/failures/Fault-Tolerance_0.xml

for ((failures = 5; failures <= 72; failures = failures += 5));
do
  echo -e "\nfailures = $failures"
  for ((t = 1; t <= 300; t++));
  do
    echo -e "\ntrial = $t"
    argos3 -c experiments/coverage/failures/Fault-Tolerance_$failures.xml
  done
done

echo -e "\nfailures = 72"
for ((t = 1; t <= 300; t++));
do
  echo -e "\ntrial = $t"
  argos3 -c experiments/coverage/failures/Fault-Tolerance_72.xml
done