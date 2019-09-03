#!/bin/bash

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