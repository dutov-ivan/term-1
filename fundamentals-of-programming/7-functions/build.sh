export CC=clang

mkdir -p build
cd build
cmake ..
make

./main
