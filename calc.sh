#!/bin/bash
ANS_FILE="calc.log"
if [[ ! -f "$ANS_FILE" ]]; then
    echo "0" > "$ANS_FILE"
fi
#Ham update file
update_file(){
    local entry="$1"
    echo $entry >> "$ANS_FILE"
    tail -n 5 "$ANS_FILE" > "$ANS_FILE.tmp" && mv "$ANS_FILE.tmp" "$ANS_FILE" 

}


while true; do
    # ANS= tail -n 1 
    ANS=$( awk '{last=$NF} END {print last}' $ANS_FILE ) 

    echo -n ">> "
    read -r input
    input=$( echo "$input" | sed  's/ANS/'$ANS'/g')

    # Lệnh thoát
    if [[ "$input" == "EXIT" ]]; then
        exit 0
    fi

    # Lệnh hiển thị lịch sử
    if [[ "$input" == "HIST" ]]; then
        cat "$ANS_FILE"
        continue
    fi
#---------------      
        set -- $input
        case "$2" in 
        
            "+") ANS=$(echo "$1 + $3" | bc)
            ;;
            "-") ANS=$(echo "$1 - $3" | bc)
            ;;
            "%") ANS=$( echo "$1 % $3" | bc)
            ;;
             "x") ANS=$(echo "scale=3; $1 * $3" | bc) 
                  ANS=$(printf "%.2f" "$ANS")
            ;;
            "/") if [ $3 -eq 0 ] 
                then echo "MATH ERROR"; read -r ss; continue 
                else ANS=$(echo "scale=3; $input" | bc)
                     ANS=$(printf "%.2f" "$ANS")
                fi
            ;;
            *) echo "SYNTAX ERROR"; read -r ss; continue 
            
        esac
        echo  $ANS
        entry=$input" = "$ANS
        update_file "$entry"
done