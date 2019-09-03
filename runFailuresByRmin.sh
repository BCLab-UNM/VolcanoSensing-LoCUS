#!/bin/bash

echo -e "\nfailures"

for ((rmin = 1; rmin <= 4; rmin++));
do
  echo -e "\nrmin = $rmin"
  echo -e "\nfailures = 0"
  echo -e "\ntrial = 1"
  argos3 -c experiments/coverage/failures/Fault-Tolerance_$rmin\_4_0.xml
  for ((failures = 5; failures <= 72; failures = failures += 5));
  do
    echo -e "\nfailures = $failures"
    for ((t = 1; t <= 10; t++));
    do
      echo -e "\ntrial = $t"
      argos3 -c experiments/coverage/failures/Fault-Tolerance_$rmin\_4_$failures.xml
    done
  done

  echo -e "\nfailures = 72"
  for ((t = 1; t <= 10; t++));
  do
    echo -e "\ntrial = $t"
    argos3 -c experiments/coverage/failures/Fault-Tolerance_$rmin\_4_72.xml
  done
done