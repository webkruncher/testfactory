

cd ~/libdb
#../dist/configure --prefix=/usr/local/berkeleydb --enable-compat185 --enable-cxx --enable-debug_rop --enable-debug_wop --enable-rpc
#make


pushd ~/libdb
echo -ne "\033[32mBuilding bdb\033[0m\n"

if [ "${1}" == "-clean" ]; then
	shift
	git clean -dfx
	git checkout -f
	rm -rf build_unix/.libs
	rm -rf build_unix/Makefile
	rm -f build_unix/*.lo
	rm -f build_unix/*.o
	[ -d ./src.build ] && rm -rf ./src.build
fi

if [ "${1}" == "-patch" ]; then
	shift
	sed -i -e 's/\(__atomic_compare_exchange\)/\1_db/' src/dbinc/atomic.h
	sed -i -e 's/\(atomic_init\)/\1_db/' src/dbinc/atomic.h
	for file in mp/mp_fget.c mp/mp_mvcc.c mp/mp_region.c mutex/mut_method.c mutex/mut_tas.c; do
		sed -i -e 's/\(atomic_init\)/\1_db/' src/${file}
	done




fi

if [ "${1}" == "-configure" ]; then
	shift
	sed -i 's/\(__atomic_compare_exchange\)/\1_db/' src/dbinc/atomic.h
	mkdir -p build_unix
	pushd build_unix
	#../dist/configure --with-cxx11-stl CXXFLAGS=-std=c++11 \
	#		--prefix=/usr \
	#		--enable-static  \
	#		--enable-cxx 

	../dist/configure --prefix=/usr/local/berkeleydb \
			--enable-compat185 \
			--enable-cxx \
			--enable-debug_rop \
			--enable-debug_wop 



	[ $? != 0 ] && exit -1


	 
			#--with-mutex=POSIX/pthreads/library


			#--enable-dbm \
			#--enable-shared=bfd,gdb \
			#--enable-compat185  \
	#make CXX=g++
	#ar cru libdb_cxx.a  `find . -name "*.o" | xargs `

	popd
fi

if [ "${1}" == "-make" ]; then

	pushd build_unix
	# https://community.oracle.com/tech/developers/discussion/3952592/bug-build-failure-on-linux-with-clang-libc-atomic-issues


	# ifdef HAVE_DBM
	sed -i -e '/DB_DBM_HSEARCH/d' db_int.h

	echo -ne "\033[33mMake in `pwd`\033[0m\n"
	make -d A 
	#ar cru libdb_cxx.a  `find . -name "*.o" | xargs `
	popd
fi

popd



