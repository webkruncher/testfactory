

pushd ${1}

Manifest=${2}

echo -ne "\033[33min `pwd`, working on ${Manifest}\033[0m\n"

echo "Compile: "
cat ${Manifest} | grep "mode=compile"
echo "Link: "
cat ${Manifest} | grep "link: ar"

popd
