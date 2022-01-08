

function Repos
{
cat <<EOF
krunchercore
exexml
datakruncher
informationkruncher
infodata
webkruncher
testfactory
tradekruncher
websites
EOF
}


function gitCommit
{
	pushd ~/Info/${1} >> /dev/null
	echo -ne "\033[33m`pwd`\n\033[0m"
	git commit -am "${2}"
	popd 2>&1 >> /dev/null
}

function GitCommit
{
	for item in `Repos`; do
		gitCommit "${item}" "${1}"
	done
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
	for item in `Repos`; do
		gitStatus ${item}
	done
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
	for item in `Repos`; do
		gitPush ${item}
	done
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
	for item in `Repos`; do
		gitPull ${item}
	done
}

function gitClone
{
	origin=git@github.com:webkruncher
	pushd ~/Info >> /dev/null
	[ ! -d ${1} ] && git clone ${origin}/${1} || echo -ne "\033[33m~/Info/${1} already exists\033[0m\n"
	popd 2>&1 >> /dev/null
}

function GitClone
{
	for item in `Repos`; do
		gitClone ${item}
	done
}

