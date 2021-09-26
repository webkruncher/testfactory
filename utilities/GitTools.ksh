

function gitCommit
{
	pushd ~/Info/${1}
	git commit -am "${2}"
	popd
}

function GitCommit
{
	gitCommit krunchercore/src ${1}
	gitCommit informationkruncher/src ${1}
	gitCommit webkruncher/src ${1}
	gitCommit testfactory/Restful ${1}
}



function gitStatus
{
	pushd ~/Info/${1}
	git status
	popd
}

function GitStatus
{
	gitStatus krunchercore/src ${1}
	gitStatus informationkruncher/src ${1}
	gitStatus webkruncher/src ${1}
	gitStatus testfactory/Restful ${1}
}


function gitPush
{
	pushd ~/Info/${1}
	git push
	popd
}

function GitPush
{
	gitPush krunchercore
	gitPush informationkruncher
	gitPush webkruncher
	gitPush testfactory
}

