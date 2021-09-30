

function gitCommit
{
	echo -ne "\033[33m" && pushd ~/Info/${1} && echo -ne "\033[0m"
	git commit -am "${2}"
	popd 2>&1 >> /dev/null
}

function GitCommit
{
	where=${1}
	shift
	gitCommit datakruncher/src ${where} "$@"
	gitCommit krunchercore/src ${where} "$@"
	gitCommit informationkruncher/src  ${where} "$@"
	gitCommit webkruncher/src ${where} "$@"
	gitCommit testfactory/Restful ${where} "$@"
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
	gitStatus webkruncher/src ${1}
	gitStatus testfactory/Restful ${1}
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
	gitPush webkruncher
	gitPush testfactory
}

