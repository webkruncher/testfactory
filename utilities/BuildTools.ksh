

#TestHost="`hostname`"
TestHost="jackmthompson.ninja"



function ProjectList
{
cat <<EOF
testfactory
krunchercore
exexml
datakruncher
informationkruncher
tradekruncher
tradekruncher/db
tickerkruncher
infodata
webkruncher
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
	export KRUNCHER_DEV_VERBOSE=${1-1}
}

function QuietCmake
{
	unset KRUNCHER_DEV_VERBOSE
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


function GetLibMidFix
{
	export LibMidFix=`cmake --trace-expand 2>&1 | grep LIBMIDFIX | cut -d '(' -f3 | cut -d ')' -f1 | cut -d ' ' -f2`
}

function GetLibPath
{
	export LibPath=`cmake --trace-expand 2>&1 | grep LIBPATH | cut -d '(' -f3 | cut -d ')' -f1 | cut -d ' ' -f2`
}

function GetCmakeLinkage
{
	cmake --trace-expand 2>&1 | grep -e "^`pwd`.*" | grep "target_link_libraries("  | cut -d '(' -f3 | cut -d ')' -f1 | tr ';' ' ' | grep -v INTERFACE | cut -d ' ' -f1,3-
}

function RecordLibTimes
{
	target=${1}
	shift
OFS=$IFS
IFS=$' '
	list="|"
	for lib in $@; do
		#echo -ne "\033[33mfind ${LibPath} -name lib${lib}.a \033[0m\n" 2>>/dev/stderr
		dota=`find ${LibPath} -name "lib${lib}.a" ` 2>>/dev/null
		if [ ! -z ${dota} ]; then
			mtime=`stat -s ${dota} | sed -n -e 's/^.*\(st_mtime=\)/\1/p' | cut -d '=' -f2 | cut -d ' ' -f1`
			echo -ne "\033[45m\033[37m${target} ${dota} ${mtime}\033[K\033[0m\r"
			list="${list}${dota};${mtime}|"
			export Libs_${target}=${list}
		fi
	done
IFS=$OFS
}

function TargetLinkage
{
	depencencies=`GetCmakeLinkage`
	for depline in ${depencencies}; do
		target=`echo "${depline}" | cut -d ' ' -f1` 
		liblist=`echo "${depline}" | cut -d ' ' -f2-` 
		if [[ ${liblist:0:1} != "/" ]] ; then
			logger "Scanning ${target}"
			RecordLibTimes ${target} ${liblist}
		fi
	done
}


function CollectProjectDependencies
{
	echo -ne "\033[43m\033[34mScanning project libraries\033[0m\n"
	CurrentProject=`pwd`
	for project in `ProjectList | tr '\n' ' ' `; do
		logger "Collecting ${project}"
		pushd ~/Info/${project}/src 2>&1 >> /dev/null
			if [ -z ${LibPath} ]; then
				GetLibMidFix
				GetLibPath
			fi
			TargetLinkage
			ThisProject=`pwd`
		popd 2>&1 >> /dev/null
		[ "${ThisProject}" == "${CurrentProject}" ] && break;
	done
	echo -ne "\033[K\033[0m\r"
}



function BuildAll
{
	[ -z `env | grep -e "^Libs_" | head -1` ] && CollectProjectDependencies
	for project in `ProjectList`; do
		pushd ~/Info/${project}/src 2>&1 >> /dev/null
		echo -ne "\r\033[3m\033[36m${project}\033[0m\033[K"
		#Build -install 2>&1>> /dev/null
		Build -install #1>> /dev/null
		if [ "$?" != "0" ] ; then
			echo -ne "\033[31m\t${project} Failed\033[K\033[0m\n" && return 1
		fi
		popd 2>&1 >> /dev/null
	done
	echo -ne "\r\033[3m\033[36mfinished\033[0m\033[K\n"
	return 0
}




function CleanAll
{
	sudo rm -rf /usr/local/lib/infokruncher/ && sudo rm -rf /usr/local/include/infokruncher
	for project in `ProjectList`; do
		pushd ~/Info/${project}/src
		Clean
		popd
	done
}


function GitClean
{
	sudo rm -rf /usr/local/lib/infokruncher/ && sudo rm -rf /usr/local/include/infokruncher
	for project in `ProjectList`; do
		pushd ~/Info/${project}/src
		git clean -dfx
		popd
	done
}


