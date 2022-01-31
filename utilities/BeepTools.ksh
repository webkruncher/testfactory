
function usleep
{
        usleepselect="select(undef,undef,undef,${1})"
        perl -e "${usleepselect}"
}

function Pause
{
        for ((i=1;i<=${1};i++)); do
                usleep "0.001"
		logger "buildbeeper ${i} sleeper"  
        done
}


function Beep
{
	#for (( ii=1; ii<=10; ii++ )); do
	#	Pause 10 
	#done
	logger "buildbeeper  $(( ( RANDOM % 10000 ) + 1 )) $@"  

}


function BeepTailer
{
	tail -f /var/log/messages | grep --line-buffered "buildbeeper"  | cut -d' ' -f 6-  
}


if [ "${1}" == "-beep" ]; then
	shift
        while read beeper; do
		much=`echo "${beeper}" | cut -d ' ' -f4`
		what=`echo "${beeper}" | cut -d ' ' -f5-`
                echo -ne "Beeped \033[7m${much} - \033[0m\033[35m${what}\033[0m\r\n"
		echo -ne "\007"
		#for (( ii=1; ii<=10; ii++ )); do
		#	Pause 10 
		#	echo -ne "\007"
		#done
        done
fi

function BeepHelp
{
cat <<EOF
ssh localhost -t ". ~/.bashrc && BeepTailer "  | ~/Info/testfactory/utilities/BeepTools.ksh -beep
#ssh tk -t ". ~/.bashrc && BeepTailer "  | ./BeepTools.ksh -beep

##Beeper.ksh
#
#        while read beeper; do
#                echo "Beeped ${beeper}"
#                echo -ne "\007"
#        done
EOF
}



