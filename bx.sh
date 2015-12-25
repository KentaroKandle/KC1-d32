echo "Building first file, first, to log1.txt"
rm kc.o 
echo "Cleaned the object file, first."
echo "Now compiling..."
time gxx -g -c kc.cpp -o kc.o > log1.txt 2>&1
#rem echo "Compile done, summary:"
echo "Now trying to link it."
time gxx kc.o -lalleg -o kc.exe
echo "Done, ready or not. see log1.txt!"
mv *.o BUILT
mv *.O BUILT
mv *.EXE BUILT
echo "NOTE: Run everything from BUILT sub-directory!"
