rm -f fs.out

g++ -std=c++11 -Wall -Wextra -O2 main.cpp -o fs.out

if [ $? -eq 0 ]; then
    echo "Compilation successful. Run ./fs.out to start the file system."
else
    echo "Compilation failed."
fi
