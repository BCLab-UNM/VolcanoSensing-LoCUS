rm -r build
mkdir build
cd build
cmake ..
make
cd ..
echo argos3 -c experiments/Fault-Tolerance.xml
