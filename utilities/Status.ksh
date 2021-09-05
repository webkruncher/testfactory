
ps aux | grep -e restful -e webkruncher -e testsite | sed '/grep/d'; find . -name "*.core"
