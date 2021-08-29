
function Build
{
	pushd ~/Info/${1}
	rm -rf ../src.build
	./go
	if [ $? != 0 ]; then
		echo -ne "\033[41m\033[33m`pwd` build failed\033[0m\n"
		exit -1 
	fi
	popd
}

function Install
{
	pushd ~/Info/${1}
	rm -rf ../src.build
	sudo ./go install
	if [ $? != 0 ]; then
		echo -ne "\033[41m\033[33m`pwd` install failed\033[0m\n"
		exit -1 
	fi
	popd
}

function ReBuild
{
	Build informationkruncher/src
	Install informationkruncher/src
	Build webkruncher/src
	Build TestFactory/Restful
	Build testsites/src
}


function InstallSites
{
	Install webkruncher/src
	install TestFactory/Restful
	Install testsites/src
}


