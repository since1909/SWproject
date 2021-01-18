#!bin/bash
#IFS=$(echo -en "\0\n")
tput civis
declare -a dlist
declare -a sublist
declare -i sizeofdlist
declare -i sizeofsublist
declare -i cursor
declare -i Tcursor
cursor=0
Tcursor=0
myHome=`pwd`
function Screen(){
    clear
    echo "============================================== 2017203056 JiWon Park  =================================================="
    echo "=========================================================== List ======================================================="
    echo "|                        |                                                |                                            |"
    echo "|                        |                                                |                                            |"
    echo "|                        |                                                |                                            |"
    echo "|                        |                                                |                                            |"
    echo "|                        |                                                |                                            |"
    echo "|                        |                                                |                                            |"
    echo "|                        |                                                |                                            |"
    echo "|                        |                                                |                                            |"
    echo "|                        |                                                |                                            |"
    echo "|                        |                                                |                                            |"
    echo "|                        |                                                |                                            |"
    echo "|                        |                                                |                                            |"
    echo "|                        |                                                |                                            |"
    echo "|                        |                                                |                                            |"
    echo "|                        |                                                |                                            |"
    echo "|                        |                                                |                                            |"
    echo "|                        |                                                |                                            |"
    echo "|                        |                                                |                                            |"
    echo "|                        |                                                |                                            |"
    echo "|                        |                                                |                                            |"
    echo "|                        |                                                |                                            |"
    echo "|                        |                                                |                                            |"
    echo "|                        |                                                |                                            |"
    echo "|                        |                                                |                                            |"
    echo "|                        |                                                |                                            |"
    echo "|                        |                                                |                                            |"
    echo "|                        |                                                |                                            |"
    echo "===================================================== Information ======================================================"
    echo "|                                                                                                                      |"
    echo "|                                                                                                                      |"
    echo "|                                                                                                                      |"
    echo "|                                                                                                                      |"
    echo "|                                                                                                                      |"
    echo "|                                                                                                                      |"
    echo "======================================================== Total ========================================================="
    echo "|                                                                                                                      |"
    echo "========================================================= END ============================================= Exit: qqq =="
}

function ScreenList(){
    dlist=(".." $(ls -1 | grep -v "2017203056-TrashBin"))
    if [[ "$myHome" == "`pwd`" ]]
    then
	    dlist=(".." "2017203056-TrashBin" $(ls -1 | grep -v "2017203056-TrashBin"))
    fi
    sizeofdlist=${#dlist[@]}
    sizeofdlist=`expr $sizeofdlist - 1`
    tput cup 2 0
    i=0
    for list in ${dlist[@]}
    do
        if (( i > 27))
        then
            :
        elif (( $cursor == $i ))
        then
            if [ -d $list ]
            then
                if [ "$list" == "2017203056-TrashBin" ]
                then
                    echo "|[43m${list:0:24}[0m"
                else
                    echo  "|[44m${list:0:24}[0m"
                fi
            elif [ -x $list ]
            then
                echo  "|[42m${list:0:24}[0m"
            else
                case "$list"
                in
                    *.gz | *.zip | *.tar | *.xy | *.bz2) echo  "|[41m${list:0:24}[0m";;
                    *) echo  "|[47m${list:0:24}[0m";;
                esac
            fi

        else
            if [ -d $list ]
            then
                if [ "$list" == "2017203056-TrashBin" ]
                then
                    echo "|[33m${list:0:24}[0m"
                else
                    echo  "|[34m${list:0:24}[0m"
                fi
            elif [ -x $list ]
            then
                echo  "|[92m${list:0:24}[0m"
            else
                case "$list"
                in
                    *.gz | *.zip | *.tar | *.xy | *.bz2) echo  "|[31m${list:0:24}[0m";;
                    *) echo  "|[0m${list:0:24}[0m";;
                esac
            fi
        fi
        i=`expr $i + 1`
    done
}

function TypeChecker(){
    if [[ -d $1 ]]
    then
        if [[ "$1" == "2017203056-TrashBin" ]]
        then
            echo -n "|[33mfile type: TrashBin"
        else
            echo -n "|[34mfile type: directory"
        fi
    elif [[ -x $1 ]]
    then
        echo -n "|[92mfile type: executable file"
    else
        case "$1"
        in
            *.gz | *.zip | *.tar | *.xy | *.bz2) echo -n "|[31mfile type: compressed file";;
            *) echo -n "|[0mfile type: regular file";;
        esac
    fi
    echo "[0m"
}

function ScreenContent(){
    if [[ -d $1 ]]
    then
        cd $1
        cursor=0
        Showup
        return
    fi
    case "$1"
    in
        *.txt | *.sh | *.c | *.h) ShowContent $1;;
        *) echo "hello";;
    esac
}

function ShowContent(){
    tput cup 2 26
    declare -i j
    declare -i k
    j=2
    k=1
    while read line
    do
        tput cup $j 26
        if [ -z "$list" ]; then continue; fi
        if (( k > 27 ))
        then
            :
        else
            echo "$k|$line" | cut -c 1-48
            j=`expr $j + 1`
            k=`expr $k + 1`
        fi
    done < $1 
}

function ScreenInformation(){
    tput cup 30 0
    echo -n "|file name: "
    tmpstr=`stat -c %n $1`
    echo ${tmpstr:0:100}
    TypeChecker $1
    echo -n "|file size: "
    du -hs $1 | awk '{print $1}' 2> /dev/null
    echo -n "|creation time: "
    year=`stat -c %y $1 | cut -c 1-4`
    month=`stat -c %y $1 | cut -c 6-7`
    case "$month"
    in
        01) month="January";;
        02) month="Febuary";;
        03) month="March";;
        04) month="April";;
        05) month="May";;
        06) month="June";;
        07) month="July";;
        08) month="August";;
        09) month="September";;
        10) month="October";;
        11) month="November";;
        12) month="December";;
    esac
    day=`stat -c %y $1 | cut -c 9-10`
    time=`stat -c %y $1 | awk '{print $2}'` 
    echo "$month $day $time $year"
    echo -n "|permission: "
    stat -c %a $1
    echo -n "|absolute path: "
    myroot=`pwd $1`
    filename=`stat -c %n $1`
    echo $myroot"/"${filename:0:78}    
}

function ScreenTotal(){
    tput cup 37 25
    total=${#dlist[@]}
    if [[ "${dlist[1]}" == "2017203056-TrashBin" ]]
    then
	    total=`expr $total - 2`
    else
	    total=`expr $total - 1`
    fi
    dir=0
    for count in ${dlist[@]}
    do
	    if [[ -d $count ]]
	    then
		    dir=`expr $dir + 1`
	    fi
    done
    if [[ "${dlist[1]}" == "2017203056-TrashBin" ]]
    then
	    dir=`expr $dir - 2`
    else
	    dir=`expr $dir - 1`
    fi
    sfiles=`ls  | find -maxdepth 1 -name "*.tar" -o -name "*.gz" -o -name "*.zip" -o -name "*.bz2" -o -name "*.xy" -o -executable \! -type d | wc -l`
    files=`expr $total - $dir`
    files=`expr $files - $sfiles`
    dsize=`du -bs`
    echo -n $total "total        "
    echo -n $dir "dir        "
    echo -n $files "file         "
    echo -n $sfiles "SpFile      "
    echo -n $dsize "bytes"
}

function Showup(){
    Screen
    ScreenList
    ScreenInformation ${dlist[$cursor]}
    ScreenTotal
}

function Delete(){
    	if [[ "`pwd`" == "$myHome/2017203056-TrashBin" ]]
    	then
    		rm  -fr $1
    	else
		if [[ -d $1 ]]
		then
			tar -cf $1.tar $1
			tar -xf $1.tar -C $myHome/2017203056-TrashBin
			rm -fr $1
			rm -fr $1.tar
		elif [[ -x $1 ]]
		then
			tar -cf $1.tar $1
			tar -xf $1.tar -C $myHome/2017203056-TrashBin
			rm -fr $1
			rm -fr $1.tar
		else
			case "$1"
        		in
            			*.gz | *.zip | *.tar | *.xy | *.bz2) tar -cf $1.tar $1; tar -xf $1.tar -C $myHome/2017203056-TrashBin; rm -fr $1; rm -fr $1.tar;;
            			*) cat $1 > $myHome/2017203056-TrashBin/$1; rm $1;;	
        		esac	
		fi
    	fi
}

function printTree(){
	if [[ -d $1 ]]
	then
		declare -i a
		declare -i j
		cd `pwd`/$1
		a=$2
		j=0
		sublist=($(ls -1 --group-directories-first))
		sizeofsublist=${#sublist[@]}		
		tput cup 2 74
		echo "|- [34m$1[0m"
		for tlist in ${sublist[@]}
		do
			if (( j > 27 ))
			then
				:
			elif (( $Tcursor == $j ))
			then
				tput cup $a 74
				if [[ -d $tlist ]]
				then
					echo "|---- + [44m$tlist[0m"
				elif [ -x $tlist ]
            			then
                			echo  "|---- [42m$tlist[0m"
            			else
                			case "$tlist"
                			in
                    				*.gz | *.zip | *.tar | *.xy | *.bz2) echo  "|---- [41m$tlist[0m";;
                    				*) echo  "|---- [47m$tlist[0m";;
                			esac
				fi
				a=`expr $a + 1`
			else 
				tput cup $a 74
				if [[ -d $tlist ]]
				then
					echo "|---- + [34m$tlist[0m"
					
				elif [ -x $tlist ]
            			then
                			echo  "|---- [92m$tlist[0m"
            			else
                			case "$tlist"
                			in
                    				*.gz | *.zip | *.tar | *.xy | *.bz2) echo  "|---- [31m$tlist[0m";;
                    				*) echo  "|---- [0m$tlist[0m";;
                			esac
				fi
				a=`expr $a + 1`
			fi
			j=`expr $j + 1`			
		done
	fi
}

function moveTree(){
	while [ true ]
	do
		read -s -n 3 Tkey
		if [[ $Tkey = "[A" ]]
		then
			Tcursor=`expr $Tcursor - 1`
			cd $temp
			Screen
			ScreenList
			ScreenInformation ${dlist[$cursor]}
			ScreenTotal
			printTree ${dlist[$cursor]} 3
		elif [[ $Tkey = "[B" ]]
		then
			Tcursor=`expr $Tcursor + 1`
			cd $temp
			Screen
			ScreenList
			ScreenInformation ${dlist[$cursor]}
			ScreenTotal
			printTree ${dlist[$cursor]} 3
		elif [[ $Tkey = "" ]]
		then
			retree=1
			cd $temp
			Screen
			ScreenList
			ScreenInformation ${dlist[$cursor]}
			ScreenTotal
			printTree ${dlist[$cursor]}
		elif [[ $Tkey = "rrr" ]]
	    	then
			cd $tmp
			Screen
			ScreenList
			ScreenInformation ${dlist[$cursor]}
			ScreenTotal
		    	break
		fi
	done
}

mkdir $myHome/2017203056-TrashBin
Showup
while [ true ]
do
    read -s -n 3 key
    if [[ $key = "[A" ]]
    then
        echo "hello up"
        if (( cursor > 0 )); then
            cursor=`expr $cursor - 1`
        fi
        Screen
        ScreenList
        ScreenInformation ${dlist[$cursor]}
        ScreenTotal

    elif [[ $key = '[B' ]]
    then
        echo "hello down"
        if (( cursor < sizeofdlist )); then
            cursor=`expr $cursor + 1`
        fi
        Screen
        ScreenList
        ScreenInformation ${dlist[$cursor]}
        ScreenTotal
        
    elif [[ $key = '[D' ]]
    then
        echo "hello Left"
        echo ${dlist[3]}
    elif [[ $key = "[C" ]]
    then
        echo "hello Right"
        echo ${dlist[4]}
    elif [[ $key = "qqq" ]]
    then
        clear
        tput cup 0 0
        echo "EXIT"
        tput cnorm
        exit 0
    elif [[ $key = "" ]]
    then
        ScreenContent ${dlist[$cursor]}
    elif [[ $key = "ddd" ]]
    then
	Delete ${dlist[$cursor]}
    elif [[ $key = "ttt" ]]
    then
	tmp=`pwd`
	Screen
        ScreenList
        ScreenInformation ${dlist[$cursor]}
        ScreenTotal
	printTree ${dlist[$cursor]} 3
	moveTree
    fi
done
tput cup 0 0 
tput cup 42 0
tput cnorm
