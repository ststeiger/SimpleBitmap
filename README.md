
This is a CLION Project

To use it with Visual-Studio: 


mkdir "D:\username\Documents\Visual Studio 2017\Projects\SimpleBitmap\buildx64"
cd /d "D:\username\Documents\Visual Studio 2017\Projects\SimpleBitmap\buildx64"
 
cmake -G "Visual Studio 15 2017 Win64" -DCMAKE_BUILD_TYPE=Release ..




mkdir "D:\username\Documents\Visual Studio 2017\Projects\SimpleBitmap\buildx86"
cd /d "D:\username\Documents\Visual Studio 2017\Projects\SimpleBitmap\buildx86"
 
cmake -G "Visual Studio 15 2017" -DCMAKE_BUILD_TYPE=Debug ..




To create a unix Makefile
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ..


cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ..