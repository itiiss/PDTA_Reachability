#!bin/sh

cmake ../Zone_Based -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local
make -j5
make doc
sudo make install