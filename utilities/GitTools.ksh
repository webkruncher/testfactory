

function gitCommit
{
	pushd ~/Info/${1}
	git commit -am "${2}"
	popd
}

function GitCommit
{
	gitCommit informationkruncher/src ${1}
	gitCommit testsites/src ${1}
	gitCommit webkruncher/src ${1}
	gitCommit TestFactory/Restful ${1}
}

