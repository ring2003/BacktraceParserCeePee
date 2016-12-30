#!/bin/csh

git clone https://github.com/abrt/btparser.git 3rd
cd 3rd
./autogen.sh
sed -i 's/PKG_CHECK_MODULES.*//g' configure
./configure CFLAGS="-I/grid/common/pkgsData/glib-v2.48.0/Linux/RHEL5.0-2010-x86_64/lib/glib-2.0/include -I/grid/common/pkgsData/glib-v2.48.0/Linux/CentOS6.0-2015-x86_64/include/glib-2.0 -L/include -L/grid/common/pkgsData/glib-v2.48.0/Linux/CentOS6.0-2015-x86_64/lib -Wl,-rpath -Wl,/grid/common/pkgsData/glib-v2.48.0/Linux/CentOS6.0-2015-x86_64/lib"
make
cd ..
ln -s 3rd/lib/.lib/
