#!/bin/bash
echo -e "\nrho"
for rmax in 2 4 8 16
do
  echo -e "\nrmax = $rmax"
  for ((i = 1; i <= $rmax; i++));
  do
    echo -e "\nrmin = $i"
    argos3 -c experiments/coverage/rho/Fault-Tolerance_$i\_$rmax.xml
  done
done