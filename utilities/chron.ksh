

numzombies=`sudo ps ax -o state,rss,command | grep webkruncher | grep "^Z" | wc -l | tr -d ' '`
echo -ne "Found ${numzombies} at `date`" >> /home/jmt/zombie.log
if [ "${numzombies}" -ge 3 ]; then
	echo -ne "restarting at `date`" >> /home/jmt/zombie.log
	sudo pkill webkruncher
	sleep 2
	sudo webkruncher --xml /home/jmt/websites/sites/webkruncher.xml --node site --filter jackmthompson.ninja	
fi
