
function Build
{
	pushd ~/Info/${1}
	./go
	if [ $? != 0 ]; then
		echo -ne "\033[41m\033[33m`pwd` build failed\033[0m\n"
		exit -1 
	fi
	popd
}

function ReBuild
{

	Build informationkruncher/src
	Build testsites/src
	Build webkruncher/src
	Build TestFactory/Restful
}

