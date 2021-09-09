

pushd ${1} 2>&1 >> /dev/stderr

Manifest=${2}

echo -ne "\033[33min `pwd`, working on ${Manifest}\033[0m\n" 2>&1 >> /dev/stderr

cat ${Manifest} | grep "mode=compile"
cat ${Manifest} | grep "link: ar"

popd 2>&1 >> /dev/stderr

