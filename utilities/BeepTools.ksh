#!/usr/local/bin/bash

#function usleep
#{
#        usleepselect="select(undef,undef,undef,${1})"
#        perl -e "${usleepselect}"
#}
#
#function Pause
#{
#        for ((i=1;i<=${1};i++)); do
#                usleep "0.001"
#		logger "buildbeeper ${i} sleeper"  
#        done
#}
#
#
#function Beep
#{
#	logger "buildbeeper  $(( ( RANDOM % 10000 ) + 1 )) $@"  
#
#}
#
#
#function BeepTailer
#{
#	tail -f /var/log/messages | grep --line-buffered "buildbeeper"  | cut -d' ' -f 6-  
#}
#
#
#
#if [ "${1}" == "-beep" ]; then
#	startedbeeper=`date +%s`
#	(( startedbeeper++ ))
#	(( startedbeeper++ ))
#OFS=$IFS
#IFS=$'\r'
#	shift
#        while read beeper; do
#		much=`echo "${beeper}" | cut -d ' ' -f4`
#		what=`echo "${beeper}" | cut -d ' ' -f5-`
#		nowbeep=`date +%s`
#		if [ ${nowbeep} -gt ${startedbeeper} ]; then
#			echo "${what}" | /usr/bin/wall
#			for (( ii=1; ii<=${much}; ii++ )); do
#				Pause 1 
#				echo -ne "\007"
#			done
#		fi
#        done
#IFS=$OFS
#fi
#
#
#if [ "${1}" == "-homebeeper" ]; then
#	echo "HomeBeeper" | wall
#OFS=$IFS
#IFS=$'\n'
#	while [ 1 ]; do
#		echo "at `date`, HomeBeeper, as `id` " >> /home/jmt/log.txt
#		ssh -t tk  ". /home/jmt/.bashrc && BeepTailer "  | /home/jmt/Info/testfactory/utilities/BeepTools.ksh -beep
#		sleep 1
#	done;
#IFS=$OFS
#fi
#
#
#function BeepHelp
#{
#cat <<EOF
## ssh localhost -t ". ~/.bashrc && BeepTailer "  | ~/Info/testfactory/utilities/BeepTools.ksh -beep
## while [ 1 ]; do ssh tk -t ". ~/.bashrc && BeepTailer " | ./BeepTools.ksh -beep; done;
#
#su -m jmt -c "ksh /home/jmt/Info/testfactory/utilties/BeepTools.ksh -homebeeper"
#
##ssh tk -t ". ~/.bashrc && BeepTailer "  | ./BeepTools.ksh -beep
#
###Beeper.ksh
##
##        while read beeper; do
##                echo "Beeped ${beeper}"
##                echo -ne "\007"
##        done
#EOF
#}
#
#
#
##
