

function gitCommit
{
	pushd ~/Info/${1} >> /dev/null
	echo -ne "\033[33m`pwd`\n\033[0m"
	git commit -am "${2}"
	popd 2>&1 >> /dev/null
}

function GitCommit
{
	where=${1}
	shift
	gitCommit krunchercore/src ${where} "$@"
	gitCommit exexml/src ${where} "$@"
	gitCommit datakruncher/src ${where} "$@"
	gitCommit informationkruncher/src  ${where} "$@"
	gitCommit webkruncher/src ${where} "$@"
	gitCommit testfactory/Restful ${where} "$@"
}



function gitStatus
{
	pushd ~/Info/${1} >> /dev/null
	echo -ne "\033[33m`pwd`\n\033[0m"
	git status
	popd 2>&1 >> /dev/null
}

function GitStatus
{
	gitStatus krunchercore/src ${1}
	gitStatus exexml/src ${1}
	gitStatus datakruncher/src ${1}
	gitStatus informationkruncher/src ${1}
	gitStatus webkruncher/src ${1}
	gitStatus testfactory/Restful ${1}
}


function gitPush
{
	pushd ~/Info/${1} >> /dev/null
	echo -ne "\033[33m`pwd`\n\033[0m"
	git push
	popd 2>&1 >> /dev/null
}

function GitPush
{
	gitPush krunchercore
	gitPush exexml
	gitPush datakruncher
	gitPush informationkruncher
	gitPush webkruncher
	gitPush testfactory
}

function gitPull
{
	pushd ~/Info/${1} >> /dev/null
	echo -ne "\033[33m`pwd`\n\033[0m"
	git pull
	popd 2>&1 >> /dev/null
}

function GitPull
{
	gitPull krunchercore
	gitPull exexml
	gitPull informationkruncher
	gitPull datakruncher
	gitPull webkruncher
	gitPull testfactory
}

function gitClone
{
	origin=git@github.com:webkruncher
	pushd ~/Info >> /dev/null
	[ ! -d ${1} ] && git clone ${origin}/${1}
	popd 2>&1 >> /dev/null
}

function GitClone
{
	gitClone krunchercore
	gitClone exexml
	gitClone datakruncher
	gitClone informationkruncher
	gitClone webkruncher
	gitClone testfactory
	gitClone websites
}
