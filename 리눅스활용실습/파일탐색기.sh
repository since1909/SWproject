#! /bin/bash

Template() {
    clear
    tput cup 0 0
    echo "=============================================== 2017203056 Jiwon Park =================================================="
    tput cup 1 0
    echo "====================================================== List ============================================================"
    tput cup 29 0
    echo "===================================================== Impormation ======================================================"
    tput cup 36 0
    echo "======================================================== Total ========================================================="
    tput cup 38 0
    echo "======================================================= End ============================================================"
    for (( i=2; i<29; i++ ))
    do
    	tput cup $i 0 
	    echo "|"
    	tput cup $i 28 
	    echo "|"
    	tput cup $i 78 
    	echo "|"
    	tput cup $i 119 
    	echo "|"
    done
    tput cup 2 0

}

#-----------------------------------------------------------------------------------------------------------------
#파일목록 만들기
fileList() {
    
    array=(`ls -1`)
    list[0]=".."
    n=1
    for i in ${array[@]}
    do
        if [ "$n" -lt 27 ]
        then
            if [ -d $i ]
            then
                list[$n]=$i
                n=`expr $n + 1`
            fi
        fi
    done
    dir_count=`expr $n - 1`
    for i in ${array[@]}
    do
        if [ "$n" -lt 27 ]
        then
            if [ -f $i ]
            then
                list[$n]=$i
                n=`expr $n + 1`
            fi
        fi
    done

    printList
}

printList() {

    tput cup 2 0
    dir_count=0
    Sfile_count=0
    file_count=0

    for i in ${list[@]}
    do
        Extension="${i##*.}"
        if [ "$i" == '..' ]
        then
            echo "|[34m$i[0m"
        elif [ -d $i ]
        then
            echo "|[34m$i[0m"
            dir_count=`expr $dir_count + 1`
        elif [ -f $i ]
        then
            if [ "$Extension" == "zip" ] || [ "$Extension" == "gz" ]
            then
                echo "|[31m$i[0m"
                Sfile_count=`expr $Sfile_count + 1`
            elif [ "$Extension" == "sh" ] || [ "$Extension" == "out" ]
            then
                echo "|[32m$i[0m"
                Sfile_count=`expr $Sfile_count + 1`
            else
                echo "|$i"
                file_count=`expr $file_count + 1`
            fi
        fi
    done

    tput cup 2 1
    Total

}


#----------------------------------------------------------------------------------------------------------------
#파일 정보출력(information)

fileInfo() {
    tput cup 30 0
    tput el

    index=`expr $x - 2`

    if [ -n "${list[index]}" ]
    then
        echo "|file name : `stat -c %n ${list[$index]}`"
        tput el
        if [ -d "${list[index]}" ]
        then
            echo "|[34mfile type : directory[0m"
        elif [ "${list[index]##*.}" == "zip" ] || [ "${list[index]##*.}" == "gz" ]
        then
            echo "|[31mfile type : compressed file[0m"
        elif [ "${list[index]##*.}" == "sh" ] || [ "${list[index]##*.}" == "out" ]
        then
            echo "|[32mfile type : execute file[0m"
        else
            echo "|file type : regular file"
        fi
        tput el
        echo "|file size : `stat -c %s ${list[$index]}`"
        tput el
        echo "|creation time : `stat -c %x ${list[$index]}`"
        tput el
        echo "|permission : `stat -c %a ${list[$index]}`"
        tput el
        echo "|absolute path : `pwd ${list[$index]}`"
    else
        break
    fi

}
#----------------------------------------------------------------------------------------------------------------
#키보드 입력으로 커서 움직이기
moveCursor() {

        read -n 1 -s esc
        if [ "$esc" == "" ]
        then
            read -n 2 -s key
        
            if [ "$key" == "[B" ]
            then
                if [ "$x" == "28" ]
                then
                    continue
                else
                    x=`expr $x + 1`
                    fileInfo $x
                    tput cup $x $y
                fi
            elif [ "$key" == "[A" ]
            then
                if [ "$x" == "2" ]
                then
                    continue
                else
                    x=`expr $x - 1`
                    fileInfo $x
                    tput cup $x $y
                fi
            fi
          
        elif [ -z $esc ]
        then
            if [ -d "${list[$index]}" ]
            then
                cd "`pwd`/${list[$index]}"
                unset list
                break
            else
                tput cup 2 29
                line_num=1
                temp=$x
                x=2

                while read -r LINE
                do

                    echo "${line_num} ${LINE}"
                    x=`expr $x + 1`
                    line_num=`expr $line_num + 1`
                    tput cup $x 29
                done < ${list[$index]}
                tput cup $temp 1
            fi
        elif [ "$esc" == "q" ]
        then
            break
        fi
}

Total() {
    tput cup 37 20
    total_count=`expr $n - 1`
    tput el
    SIZE=`du -s | cut -f 1` 
    echo "${total_count} total   ${dir_count} dir   ${file_count} file   ${Sfile_count} Sfile  ${SIZE} byte "
}

main(){
    declare -i x
    declare -i y

    x=2
    y=1

    Template
    fileList
    fileInfo $x
    tput cup 2 1
    
    while(true)
    do
        moveCursor $x $y
    done
}

while(true)
do
    main
done

