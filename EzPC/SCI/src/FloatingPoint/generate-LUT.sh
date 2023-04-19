g++ -O3 -std=c++17 -I/usr/local/include -L/usr/local/lib -lmpfr generate-LUT.cpp -o generate-LUT.o
./generate-LUT.o
rm *.o
