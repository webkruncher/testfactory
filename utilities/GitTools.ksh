

function gitCommit
{
	pushd ~/Info/${1} >> /dev/null
	echo -ne "\033[33m`pwd`\033[0m"
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
	pushd ~/Info/${1} >> /dev/null
	echo -ne "\033[33m`pwd`\033[0m"
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
	pushd ~/Info/${1} >> /dev/null
	echo -ne "\033[33m`pwd`\033[0m"
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

function gitPull
{
	pushd ~/Info/${1} >> /dev/null
	echo -ne "\033[33m`pwd`\033[0m"
	git pull
	popd 2>&1 >> /dev/null
}

function GitPull
{
	gitPull datakruncher
	gitPull krunchercore
	gitPull informationkruncher
	gitPull webkruncher
	gitPull testfactory
}

