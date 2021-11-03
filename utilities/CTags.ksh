function Ctags
{
    for item in `find . \( -name "*.c*" -o -name "*.h*" \) | sed '/build/d' | sed '/cmake/d' | sed '/CMakeLists/d' `; do
        wc=`cat ${item} | wc -l `
        echo -ne "\t\t${item}:${wc} lines\n"
OFS=$IFS
IFS=$'\n'
        for function in `ctags -x ${item} | tr -s ' ' | cut -d ' ' -f1,2`; do
            echo -ne "\t\t\t${function};\n"
        done
IFS=$OFS
    done;
}
 

function CtagsSqueeze
{
        Ctags | tr '\t' ' ' | tr -s ' '  | cut -d ' ' -f2,3
}

 
function ListObjects
{
        name="Objects"
        echo -ne "\t\t\033[4m\033[5m\033[3m\033[1m${name}:\033[0m\n\033[7m\033[1m"
        CtagsSqueeze | grep -e " struct" | cut -d ' ' -f1 | tr '\n' '\t' | tr '\t' ' '
        echo -ne "\n\n\033[0m"
}

 

function ListOperators
{
        name="Operators"
        echo -ne "\t\t\033[4m\033[5m\033[3m\033[1m${name}:\033[0m\n\033[7m\033[1m"
        CtagsSqueeze | grep -e "^operator " | cut -d ' ' -f2 | tr '\n' '\t' | tr '\t' ' '
        echo -ne "\n\n\033[0m"
}

 

function ListFunctions
{
        name="Functions"
        echo -ne "\t\t\033[4m\033[5m\033[3m\033[1m${name}:\033[0m\n\033[7m\033[1m"
        CtagsSqueeze | grep -e " function" | cut -d ' ' -f1 | tr '\n' '\t' | tr '\t' ' '
        echo -ne "\n\n\033[0m"
}
 

function CtagsSummarize
{
        ListObjects
        ListOperators
        ListFunctions
        FileCount=`CtagsSqueeze | cut -d' ' -f2 | grep lines | wc -l`
        OperatorCount=`CtagsSqueeze | cut -d' ' -f1 | grep operator | wc -l`
        StructCount=`CtagsSqueeze | cut -d' ' -f2 | grep struct | wc -l`
        FunctionCount=`CtagsSqueeze | cut -d' ' -f2 | grep function | wc -l`
	FunctionOperatorCount=$(($OperatorCount+$FunctionCount))
 
        echo "Number of Files:${FileCount}"
        echo "Number of Objects:${StructCount}"
        echo "Number of operators:${OperatorCount}"
        echo "Number of functions:${FunctionCount}"
        echo "Total Functions:${FunctionOperatorCount}"
}
 

