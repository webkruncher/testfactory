
git config --global --edit

curl https://raw.githubusercontent.com/webkruncher/testfactory/master/utilities/GitTools.ksh > infobootstrap.ksh
. ./infobootstrap
mkdir -p ~/Info
GitClone
cd ~/Info/informationkruncher/src
db/go.libdb -configure
ReBuild
cd ~
ln -s Info/websites

cat <EOF

export TICKERSOURCE_APIKEY=""
set -o vi
export PATH=$PATH:~/Info/testfactory/utilities/
. ~/Info/testfactory/utilities/DevTools.ksh
. ~/Info/testfactory/utilities/BuildTools.ksh
. ~/Info/testfactory/utilities/GitTools.ksh

export PS1='\u@\h \W> '
export KRUNCHER_DEV_LIBS=1

EOF
rm ~/infobootstrap.ksh
