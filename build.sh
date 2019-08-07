mkdir build
cd build
cmake ..
make clean
make
cd ..
time argos3 -c experiments/Fault-Tolerance.xml
