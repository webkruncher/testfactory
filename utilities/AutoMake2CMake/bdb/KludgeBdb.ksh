


pushd ${1}

echo "Fixing atomic in `pwd`"
git checkout -f
git clean -dfx
sed -i 's/\(__atomic_compare_exchange\)/\1_db/' src/dbinc/atomic.h

popd


