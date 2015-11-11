src=$(pwd)
mkdir /build/RTIMU
pushd /build/RTIMU
cmake $src
make -j4 
sudo make install
sudo ldconfig

