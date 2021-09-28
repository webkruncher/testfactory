

function gitCommit
{
	echo -ne "\033[33m" && pushd ~/Info/${1} && echo -ne "\033[0m"
	git commit -am "${2}"
	popd 2>&1 >> /dev/null
}

function GitCommit
{
	gitCommit datakruncher/src ${1}
	gitCommit krunchercore/src ${1}
	gitCommit informationkruncher/src ${1}
	gitCommit testsites/src ${1}
	gitCommit webkruncher/src ${1}
	gitCommit TestFactory/Restful ${1}
}



function gitStatus
{
	echo -ne "\033[33m" && pushd ~/Info/${1} && echo -ne "\033[0m"
	git status
	popd 2>&1 >> /dev/null
}

function GitStatus
{
	gitStatus datakruncher/src ${1}
	gitStatus krunchercore/src ${1}
	gitStatus informationkruncher/src ${1}
	gitStatus testsites/src ${1}
	gitStatus webkruncher/src ${1}
	gitStatus TestFactory/Restful ${1}
}


function gitPush
{
	echo -ne "\033[33m" && pushd ~/Info/${1} && echo -ne "\033[0m"
	git push
	popd 2>&1 >> /dev/null
}

function GitPush
{
	gitPush datakruncher
	gitPush krunchercore
	gitPush informationkruncher
	gitPush testsites
	gitPush webkruncher
	gitPush TestFactory
}

