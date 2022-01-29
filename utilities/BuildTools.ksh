


TestHost="jackmthompson.ninja"



function ProjectList
{
cat <<EOF
krunchercore
exexml
datakruncher
informationkruncher
tradekruncher
tradekruncher/db
tickerkruncher
testfactory
infodata
webkruncher
EOF
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
	cmake --trace-expand 2>&1 | grep LIBPATH | cut -d '(' -f3 | cut -d ')' -f1 | cut -d ' ' -f2
}

function GetCmakeLinkage
{
	cmake --trace-expand 2>&1 | grep -e "^`pwd`.*" | grep "target_link_libraries("  | cut -d '(' -f3 | cut -d ')' -f1 | tr ';' ' ' | grep -v INTERFACE | cut -d ' ' -f1,3-
}

function RecordLibTimes
{
#echo "RecordLibTimes:$1"
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
			list="${list}${dota};${mtime}|"
			#logger "export Libs_${target}_${lib}=${mtime}"
			export Libs_${target}_${lib}=${mtime}
		else
			echo -ne "\033[31m${lib} was not found in ~/Info\033[0m\n"
		fi
	done
IFS=$OFS
}

function TargetLinkage
{
	depencencies=`GetCmakeLinkage`

	if [ "${depencencies}" != "" ]; then
		#logger "In `pwd`, depencencies:..."
		#echo -ne "\033[35m`pwd` Cmake Depends:\n${depencencies}\033[0m\n" >> /dev/stderr
		target=`echo "${depencencies}" | cut -d ' ' -f1` 
		depline=`echo "${depencencies}" | cut -d ' ' -f2-` 
		#echo -ne "Targets\n${target}\n" | cat -n >> /dev/stderr
		#echo -ne "\t\tDepends on\n"  >> /dev/stderr
		deplist=`echo "${depline}" | tr ' ' '\n' | sort | uniq | tr '\n' ' '`
		#echo "${deplist}" >> /dev/stderr
		for depline in ${deplist}; do
			if [[ ${liblist:0:1} != "/" ]] ; then
				#echo "find ${LibPath} -name lib${depline}.a"  >> /dev/stderr
				dota=`find ${LibPath} -name "lib${depline}.a" ` 2>>/dev/null
				mtime=`stat -s ${dota} | sed -n -e 's/^.*\(st_mtime=\)/\1/p' | cut -d '=' -f2 | cut -d ' ' -f1`
				[ "${dota}" != "" ] && echo -ne "${dota};${mtime}|" 
			fi
		done
	fi
}


function CollectProjectDependencies
{
	#echo -ne "\033[43m\033[34mScanning project libraries\033[0m\n"
	CurrentProject=`pwd`
	liblist="|"
	for project in `ProjectList | tr '\n' ' ' `; do
		#logger "Collecting ${project}"
		pushd ~/Info/${project}/src 2>&1 >> /dev/null
			liblist="${liblist}`TargetLinkage`"
			ThisProject=`pwd`
		popd 2>&1 >> /dev/null

		envname=`echo "${project}" | tr '/' '_' `
		export LibList_${envname}=`echo "${liblist}" | tr '|' '\n' | sort | uniq | tr '\n' '|'`
		#[ "${ThisProject}" == "${CurrentProject}" ] && break;
	done
	
}


function CleanTrigger
{
	#echo -ne "\033[37m\033[43mChecking triggers:$@\033[0m\n" >> /dev/stderr
	echo "1"
}

function CheckAndUpdateLinkLibraryDate 
{
	dota=`echo "${1}" | cut -d ';' -f1`
	#logger "Getting lastupdate for ${2}, ${dota}" 
	lastupdate=`echo "${1}" | cut -d ';' -f2 | cut -d '|' -f1`
	mtime=`stat -s ${dota} | sed -n -e 's/^.*\(st_mtime=\)/\1/p' | cut -d '=' -f2 | cut -d ' ' -f1`
	if [ "${mtime}" != "${lastupdate}" ]; then
		#logger "${dota} was last updated at ${lastupdate}, and the current timestamp is ${mtime}"
		echo "${2}"
	fi

}

function CheckLibs 
{
	needsupdate="0"
	while read liblin; do
		if [ "${liblin}" != "" ]; then
			dota=`echo "${liblin}" | cut -d ';' -f1`
			when=`echo "${liblin}" | cut -d ';' -f2`
			#logger "CheckLibs ${liblin} ${dota} ${when}"
		
			mtime=`stat -s ${dota} | sed -n -e 's/^.*\(st_mtime=\)/\1/p' | cut -d '=' -f2 | cut -d ' ' -f1`
			#echo "Checking ${dota}, ${when} == ${mtime}"  >> /dev/stderr
			if [ "${mtime}" != "${when}" ]; then
				logger "${dota} was last updated at ${when}, and the current timestamp is ${mtime}"
				echo "${dota}"
			fi
		fi
	done
}

function UpdateTimestamps
{
	envname=`echo "${1}" | tr '/' '_' `
	#echo -ne "\033[31m"
	#echo `env | grep -e "^LibList_${envname}" `
	#echo -ne "\033[0m"

	Libs=`env | grep -e "^LibList_${envname}" | cut -d '|' -f2- | tr '|' '\n'` 

	#echo "Updating timestamps for ${1}"
	#echo "Current List:"
	#echo -ne "${Libs}"
	#echo


	while read liblin; do
		#echo "Removing:${liblin};" 
		Libs=`echo "${Libs}" | grep -v "${liblin}"`

		mtime=`stat -s ${liblin} | sed -n -e 's/^.*\(st_mtime=\)/\1/p' | cut -d '=' -f2 | cut -d ' ' -f1`
		Libs=`echo -ne "${Libs}\n${liblin};${mtime}\n"`
	done


	#echo "Updated List:"
	#echo -ne "${Libs}"
	#echo

	NewEnv=`echo "${Libs}" | tr '\n' '|'`
	
	#echo -ne "\033[32mLibList_${envname}=|"
	#echo -ne "${NewEnv}"
	#echo -ne "\033[0m"

	#export LibList_${envname}="|${NewEnv}"
	#export LibList_${envname}="empty"
	echo "|${NewEnv}"
}


function BuildAll
{
	echo -ne "\033[45m\033[34mBuildAll in `pwd`\033[0m\n"
	ReLinkedFor=""
	CurrentProject=`pwd`
	for project in `ProjectList`; do
		pushd ~/Info/${project}/src 2>&1 >> /dev/null
		#echo "Project:${project}" >> /dev/stderr
		[ "${LibPath}" == "" ] && export LibPath=`GetLibPath`
		envname=`echo "${project}" | tr '/' '_' `
		needsscanner=`env | grep -e "^LibList_${envname}"`
		if [ "${needsscanner}" == "" ]; then
			#echo -ne "Loading ${envname}"
			logger "Scanning ${envname}"
			CollectProjectDependencies
		fi


		#echo -ne "\033[45mBuilding in `pwd`\033[0m\n"
		
		Libs=`env | grep -e "^LibList_${envname}" | cut -d '|' -f2- | tr '|' '\n'`

		needsUpdate=`echo "${Libs}" | sort | uniq | CheckLibs `

		if [ "${needsUpdate}" != "" ]; then
			#pwdd="^`pwd`.*"
			#exes=`cmake --trace-expand 2>&1 | grep -e ${pwdd} | grep -e "add_executable(" | cut -d '(' -f3 | cut -d ')' -f1 | cut -d ' ' -f1`
			#if [ "${exes}" != "" ]; then
			#	#echo "Updating ${exes}"
			#	for exe in ${exes}; do
			#		logger "removing ${exe}"
			#		find ../src.build -name "${exe}" -exec rm {} \;
			#	done
			#fi
			logger "`pwd`|Build -clean -install"
			Build -clean -install 2>&1>> /dev/null
		else
			logger "`pwd`|Build -install"
			Build -install 2>&1>> /dev/null
		fi



		if [ "$?" != "0" ] ; then
			echo -ne "\033[31m\t${project} Failed\033[K\033[0m\n" && return 1
		fi
		ThisProject=`pwd`


		if [ "${needsUpdate}" != "" ]; then
			#echo -ne "\nUpdating records for ${project}:\n"
			#echo "${needsUpdate}" | cat -n

			NewList=`echo "${needsUpdate}" | UpdateTimestamps ${project}`
			#echo -ne "Updated List:\n${NewList}\n"
			export LibList_${envname}=${NewList}
		fi


		popd 2>&1 >> /dev/null




		[ "${ThisProject}" == "${CurrentProject}" ] && break;
	done
	echo -ne "\r\033[3m\033[36mfinished in `pwd`\033[0m\033[K\n"


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

	bins=/usr/local/bin
	sudo rm -rf ${bins}/datafactory
	sudo rm -rf ${bins}/datatester
	sudo rm -rf ${bins}/krestdb
	sudo rm -rf ${bins}/informationkrunchers
	sudo rm -rf ${bins}/kruncher
	sudo rm -rf ${bins}/exeut
	sudo rm -rf ${bins}/tradekruncher
	sudo rm -rf ${bins}/ingest
	sudo rm -rf ${bins}/traderdata
	sudo rm -rf ${bins}/tickerkruncher
	#sudo rm -rf webkruncher

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


