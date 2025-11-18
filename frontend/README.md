//*******************************************************************
sudo apt-get install -y build-essential cmake gcc-aarch64-linux-gnu g++-aarch64-linux-gnu pkg-config
//*******************************************************************
ssh root@192.168.100.50 'tar -C / -cpf - lib usr/lib usr/include 2>/dev/null' \
| tar -C /opt/rpi5-sysroot -xpf -
//*******************************************************************
ssh root@192.168.100.50 'tar -C / -cpf - lib64 2>/dev/null' \
| tar -C /opt/rpi5-sysroot -xpf - || true

//*******************************************************************
export RPI_SYSROOT=/opt/rpi5-sysroot
export PKG_CONFIG_DIR=
export PKG_CONFIG_LIBDIR=$RPI_SYSROOT/usr/lib/aarch64-linux-gnu/pkgconfig:$RPI_SYSROOT/usr/lib/pkgconfig:$RPI_SYSROOT/usr/share/pkgconfig
export PKG_CONFIG_SYSROOT_DIR=$RPI_SYSROOT


//*******************************************************************
rm -rf build_rpi5
mkdir build_rpi5 && cd build_rpi5
cmake -DCMAKE_TOOLCHAIN_FILE=../toolchain-aarch64-rpi5.cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . -j

