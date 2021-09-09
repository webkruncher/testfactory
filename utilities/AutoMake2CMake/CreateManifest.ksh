
buildlog=$(mktemp /tmp/cmakizer.XXXXXXXXXX)


#export LDFLAGS="-lpthread -lm"
#export CC="clang"



pushd ${1}

sudo chown -R jmt .
rm -rf build_unix/.libs
rm -rf build_unix/Makefile
rm -f build_unix/*.lo
rm -f build_unix/*.o



pushd build_unix

echo "Configuring in `pwd`" >> /dev/stderr


../dist/configure \
                --prefix=/usr \
                --enable-cxx \
		--enable-static  2>&1 | tee ${buildlog}



make 2>&1 | tee -a ${buildlog}

manifest=$(mktemp /tmp/cmakizer.manifest.XXXXXXXXXX)
cat ${buildlog} | grep -e "mode=compile" -e "mode=link" -e "link: ar"> ${manifest}

echo "MANIFEST:${manifest}"

popd

