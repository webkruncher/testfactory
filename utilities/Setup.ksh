


sudo  ln -sf /usr/share/zoneinfo/America/New_York /etc/localtime


pkg_add -I vim-8.2.3456-no_x11 bash git cmake  sudo-1.9.7.2

while [ -L ~/libdb ]; do
	echo -ne "~/libdb already exists"
	read
done;

while [ ! -f db-18.1.40.tar.gz ]; do
	echo "https://www.oracle.com/database/technologies/related/berkeleydb-downloads.html"
	read ok
done;


pushd ~

tar xzvf db-18.1.40.tar.gz
ln -s db-18.1.40 libdb


pushd ~/libdb/build_unix
../dist/configure --prefix=/usr/local/berkeleydb \
		--enable-compat185 \
		--enable-cxx \
		--enable-debug_rop \
		--enable-debug_wop

make -d A 
popd
popd


