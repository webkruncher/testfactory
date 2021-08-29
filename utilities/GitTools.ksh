

function gitCommit
{
	pushd ~/${1}
	git commit -am "${2}"
#	if [ $? != 0 ]; then
#		echo -ne "\033[41m\033[33m`pwd` build failed\033[0m\n"
#		exit -1 
#	fi
	popd
}

function GitCommit
{
	gitCommit informationkruncher/src ${1}
	gitCommit testsites/src ${1}
	gitCommit webkruncher/src ${1}
	gitCommit TestFactory/Restful ${1}
}

