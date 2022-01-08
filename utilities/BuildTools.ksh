

#TestHost="`hostname`"
TestHost="jackmthompson.ninja"



function ProjectList
{
cat <<EOF
krunchercore
exexml
datakruncher
informationkruncher
webkruncher
tradekruncher
infodata
EOF
}
	#testfactory/Restful


function OldBuild
{
	pushd ~/Info/${1}/src
	shift
	sudo chown -R jmt ../*
	if [ "${1}" == "-clean" ]; then
		shift
		echo -ne "\033[41m\033[33mCleanup in `pwd`\033[0m\n"
		Clean
	fi
	echo -ne "\033[45m\033[34m\033[1mBuilding in `pwd`\033[0m\n"
	Build
	if [ $? != 0 ]; then
		echo -ne "\033[41m\033[33m`pwd` build failed\033[0m\n"
		exit -1 
	fi
	popd
}


function Install
{
	pushd ~/Info/${1}/src
	Build -install
	if [ $? != 0 ]; then
		echo -ne "\033[41m\033[33m`pwd` install failed\033[0m\n"
		exit -1 
	fi
	popd
}


function  Find
{
	find . \( -name "*.cpp" -o -name "*.h" \) | xargs fgrep "$@"
}


function Test
{
	sudo pkill tail
	sudo pkill datafactory
	sudo pkill webkruncher
	sudo pkill restful
	if [ "${1}" == "-rebuild" ]; then 
		ReBuild
		shift
		sudo webkruncher --xml /home/jmt/websites/sites/webkruncher.xml  --node site --filter ${TestHost}&
	else
		if [ "${1}" == "-serve" ]; then 
			shift
			sudo webkruncher --xml /home/jmt/websites/sites/webkruncher.xml  --node site --filter ${TestHost}&
		fi
	fi
	sleep 1
	sudo restful --xml /home/jmt/websites/sites/webkruncher.xml  --node site --filter ${TestHost}&
	#sudo datafactory --xml /home/jmt/websites/sites/webkruncher.xml  --node data --filter ${TestHost}

	Status
	

}	


function Wip
{
	if [ ! -z ${1} ]; then
		cd ~/Info/${1}/src
	else
		select project in `ProjectList`; do
			cd ~/Info/${project}/src
			break
		done 
	fi
}

function Vim
{
	select vim in `ls -lt *.vim | tr -s ' ' | cut -d ' ' -f 9`; do
		vim -S ${vim}
		break
	done 
}

function VerboseCmake
{
	export KRUNCHER_DEV_VERBOSE=1
}

function ReleaseBuild
{
	unset KRUNCHER_DEV_LIBS
}

function DevBuild
{
	export KRUNCHER_DEV_LIBS=1
}

function Clean
{
	[ -d ../src.build ] && rm -rf ../src.build
}

function RecordJournal
{
	export KRUNCHER_JOURNAL="journal"
}

DevBuild

function ReBuild
{
	sudo rm -rf /usr/local/lib/infokruncher/ && sudo rm -rf /usr/local/include/infokruncher
	for project in `ProjectList`; do
		pushd ~/Info/${project}/src
		Build -clean -install
		popd
	done
}

