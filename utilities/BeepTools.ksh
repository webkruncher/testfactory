#!/usr/local/bin/bash

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
OFS=$IFS
IFS=$'\n'
        while read beeper; do
		much=`echo "${beeper}" | cut -d ' ' -f4`
		what=`echo "${beeper}" | cut -d ' ' -f5-`
                echo -ne "Beeped \033[7m${much} - \033[0m\033[35m${what}\033[0m\r\n"
                echo "Beeped ${much} ${what}" | wall
		echo -ne "\007"
        done
IFS=$OFS
fi


if [ "${1}" == "-homebeeper" ]; then
	ssh tk -t ". /home/jmt/.bashrc && BeepTailer "  | /home/jmt/Info/testfactory/utilities/BeepTools.ksh -beep
fi


function BeepHelp
{
cat <<EOF
# ssh localhost -t ". ~/.bashrc && BeepTailer "  | ~/Info/testfactory/utilities/BeepTools.ksh -beep
# while [ 1 ]; do ssh tk -t ". ~/.bashrc && BeepTailer " | ./BeepTools.ksh -beep; done;

su -m jmt -c "ksh /home/jmt/Info/testfactory/utilties/BeepTools.ksh -homebeeper"

#ssh tk -t ". ~/.bashrc && BeepTailer "  | ./BeepTools.ksh -beep

##Beeper.ksh
#
#        while read beeper; do
#                echo "Beeped ${beeper}"
#                echo -ne "\007"
#        done
EOF
}




