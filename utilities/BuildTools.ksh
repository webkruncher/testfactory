

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
	Build TestFactory/Restful
	#Build testsites
}

function Install
{
	pushd ~/Info/${1}/src
	sudo ./go install
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
	ReBuild
	sudo pkill webkruncher
	#sudo pkill testsite
	sudo pkill restful
	sleep 1
	InstallSites
	sudo webkruncher --http --https
	#sudo testsite
	restful --http --https
}	

