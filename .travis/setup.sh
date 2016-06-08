#!/bin/bash

sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y
sudo apt-get update -q
sudo apt-get install lua5.2 liblua5.2-dev qt5-default qtbase5-dev -y

if [ "$CXX" = "g++" ]; then
	sudo apt-get install gcc-5 g++-5 -y
	sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-5 50
	sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-5 50
fi

