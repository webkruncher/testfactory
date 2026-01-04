

lastzombies=`tail -1 /home/jmt/zombie.log | cut -d ' ' -f2`

numzombies=`sudo ps ax -o state,rss,command | grep webkruncher | grep "^Z" | wc -l | tr -d ' '`
if [ "${lastzombies}" != "${numzombies}" ]; then
	echo -ne "Found ${numzombies} at `date`\n" >> /home/jmt/zombie.log
	if [ "${numzombies}" -ge 8 ]; then
		echo -ne "restarting at `date`\n" >> /home/jmt/zombie.log
		sudo pkill webkruncher
		sleep 2
		sudo webkruncher --xml /home/jmt/websites/sites/webkruncher.xml --node site --filter jackmthompson.ninja	
	fi
fi
