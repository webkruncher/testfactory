

#TestHost="`hostname`"
TestHost="jackmthompson.ninja"

function Wip
{
	cd ~/Info/${1}/src
}

function Build
{
	pushd ~/Info/${1}/src
	sudo chown -R jmt ../*
	rm -rf ../src.build
	./go 
	if [ $? != 0 ]; then
		echo -ne "\033[41m\033[33m`pwd` build failed\033[0m\n"
		exit -1 
	fi
	popd
}

function ReBuild
{
	Build krunchercore
	Build datakruncher
	Build informationkruncher
	Install informationkruncher
	Build webkruncher
	Install webkruncher
	#Build TestFactory/Restful
	#Build testsites
}

function Install
{
	pushd ~/Info/${1}/src
	sudo ./go -install
	if [ $? != 0 ]; then
		echo -ne "\033[41m\033[33m`pwd` install failed\033[0m\n"
		exit -1 
	fi
	popd
}


function InstallSites
{
	Install krunchercore
	Install datakruncher
	Install informationkruncher
	Install webkruncher
	Install TestFactory/Restful
	#Install testsites
}

function mem
{                                                                                                      
	sudo ps ax -o rss,command | cut -d ' ' -f1,2,3,4 | grep -v grep | grep ${1} |  tr -s ' ' | awk -F' ' '{sum+=$1;}END{print sum;}'
}

function  Find
{
	find . \( -name "*.cpp" -o -name "*.h" \) | xargs fgrep "$@"
}


function Status
{
	while [ 1 ]; do
		wkmem=`mem webkruncher | tr -d ' ' | tr -d '\t'`
		msgtail=`tail -1 /var/log/messages | awk -F "|" '{ for (i=2; i<=NF; i++)   printf( "|%s", $i ); }'`
		when=`date "+%Y-%m-%d|%H:%M:%S"`
		echo -ne "\r\033[K${when}|\033[35m${wkmem}\033[30m|\033[36m${msgtail}\033[0m"
		sleep 1
	done
}


function Test
{
	sudo pkill tail
	sudo pkill datafactory
	sudo pkill webkruncher
	sudo pkill restful
	if [ "${1}" == "-rebuild" ]; then 
		ReBuild
		InstallSites
		shift
	fi
	sleep 1
	sudo webkruncher --xml /home/jmt/websites/sites/webkruncher.xml  --node site --filter ${TestHost}
	sudo restful --xml /home/jmt/websites/sites/webkruncher.xml  --node site --filter ${TestHost}
	#sudo datafactory --xml /home/jmt/websites/sites/webkruncher.xml  --node data --filter ${TestHost}

	Status
	

}	

