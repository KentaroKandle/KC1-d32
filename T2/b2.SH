echo "Building State Save Module, before first."
mv t2.exe t2.bak
time gxx -g -c state.cpp -o state.o > log2.txt 2>&1
time gxx -g -c t2.cpp -o t2.o > log2.txt 2>&1
time gxx t2.o state.o -o t2.exe
echo "We totally got that shit done, like Assholes!"
echo "Done, ready or not. see log2.txt!"
echo "Now, to test it!"
t2.exe

