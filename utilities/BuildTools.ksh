

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
	Install webkruncher
	Install TestFactory/Restful
	#Install testsites
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

	sleep 1
	Status.ksh
	sleep 1	
	#sudo tail -f /var/log/messages
	while [ 1 ]; do
		wkmem=`mem webkruncher`
		msgtail=`tail -1 /var/log/messages`
		echo -ne "\033[35m${wkmem}\033[30m|\033[36m${msgtail}\033[0m\r"
	done
	

}	

