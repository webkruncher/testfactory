

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
tickerkruncher
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


function GitStatusPretty
{
IFS=$'\n'
	status=$(</dev/stdin)
	lc=`echo "${status}" | wc -l`;
	pwd=`pwd` 
	pwd=`echo "${pwd}" | tr '/' '\n' | tail -n -1 | head -1| tr '\n' '/' `
	#echo -ne "\033[37m\033[45m|${pwd}\033[7m|"
	printf "\033[37m\033[45m|%22s\033[7m|" "${pwd}"
	echo -ne "${status}" | head -2 | tr '\n' ' ';
	echo -ne "\033[0m\n"
	if [ ${lc} -gt 4 ]; then 	
		for stline in `echo -ne "${status}\n" | grep "new file:"`; do
			echo -ne "\033[41m\033[31m\033[40m\033[1m${stline}\033[0m\n";
		done
		for stline in `echo -ne "${status}\n" | tail -n +6  | sed  '$ d'  | grep -v -e "\s*("`; do
			echo -ne "\033[41m\033[32m\033[1m${stline}\033[0m\n";
		done
	fi;
}

function gitStatus
{
	pushd ~/Info/${1} >> /dev/null
	#echo -ne "\033[33m`pwd`\n\033[0m"
	git status | GitStatusPretty
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
	[ ! -d ~/Info ] && mkdir ~/Info
	for item in `Repos`; do
		gitClone ${item}
		pushd ~/Info/${item}
		git checkout master
		popd
	done
}

