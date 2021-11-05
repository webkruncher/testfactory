
function mem
{                                                                                                      
	sudo ps ax -o rss,command | cut -d ' ' -f1,2,3,4 | grep -v grep | grep ${1} |  tr -s ' ' | awk -F' ' '{sum+=$1;}END{print sum;}'
}

function PidsFor
{
OFS=$IFS
IFS=$'\n'
	for process in `ps aux -o pid,command | tr -s ' ' | cut -d ' ' -f2,11 | grep "${1}"`; do
		echo "${process}"
	done
IFS=${OFS}
}

function GetOpenFiles
{
	process=${1}
	shift
	schemer=${1}
	shift
	for pid in `PidsFor ${process} | cut -d ' ' -f1`; do 
		files=`sudo fstat -p ${pid} 2>>/dev/null| grep "internet stream" | grep ":${schemer}$" `
		[ ! -z "${files}" ] && echo "${files}"
	done;
}

function Status
{
	echo -ne "\r\033[Kdate|time|Failed|Succeeded|\033[35mwkmem\033[0m|\033[36mrestfulHTTPfiles|webkruncherHTTPfiles|restfulHTTPSfiles|webkruncherHTTPSfiles\033[0m\n"
	while [ 1 ]; do
		when=`date "+%Y-%m-%d|%H:%M:%S"`
		Failed=`cat /var/log/messages | grep "|FAIL|" | wc -l`
		Succeeded=`cat /var/log/messages | grep "|SUCCESS|" | wc -l`
		PassFail=$(printf "\033[31m%05d\033[0m|\033[32m%05d\033[0m|" ${Failed} ${Succeeded})
		wkmem=`mem webkruncher | tr -d ' ' | tr -d '\t'`
		#msgtail=`tail -1 /var/log/messages | awk -F "|" '{ for (i=2; i<=NF; i++)   printf( "|%s", $i ); }'`
		#echo -ne "\r\033[K${when}|${PassFail}\033[35m${wkmem}\033[0m\033[36m${msgtail}\033[0m"

		restfulHTTPfiles=`GetOpenFiles restful 80 | wc -l | tr -d ' '`
		webkruncherHTTPfiles=`GetOpenFiles webkruncher 80 | wc -l | tr -d ' '`
		restfulHTTPSfiles=`GetOpenFiles restful 443 | wc -l | tr -d ' '`
		webkruncherHTTPSfiles=`GetOpenFiles webkruncher 443 | wc -l | tr -d ' '`
		echo -ne "\r\033[K${when}|${PassFail}\033[35m${wkmem}\033[0m|\033[36m${restfulHTTPfiles}|${webkruncherHTTPfiles}|${restfulHTTPSfiles}|${webkruncherHTTPSfiles}\033[0m"
		sleep 1
	done
}

