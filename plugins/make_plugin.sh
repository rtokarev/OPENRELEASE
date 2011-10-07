rm *.o *.so
mipsel-linux-uclibc-gcc -fPIC -c plugin_sample.c -I../include 
mipsel-linux-uclibc-gcc -shared plugin_sample.o ../libopenrelease/libopenrelease.so -o plugin_sample.so