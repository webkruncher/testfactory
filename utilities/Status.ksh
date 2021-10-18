
ps aux -o rss,command| grep -e restful -e webkruncher -e restful | sed '/grep/d'; 
find . -name "*.core"
