#!/bin/bash

# function args: $1 data array
#                $2 max data range
#                $3 num of steps
#                $4 offset X
#                $5 offset Y
function draw_graph() {
    array=($@)
    num_of_steps=${array[-3]}
    
    max=0
    for val in ${array[@]}; do
        if [ $max -le $val ]
        then
            max=$val
        fi
    done

    step=$(($max / $num_of_steps))
    global_offset_x=${array[-2]}
    global_offset_y=${array[-1]}

    unset array[-1]
    unset array[-1]
    unset array[-1]

    for offset_x in $(seq 1 ${#array[@]}); do
        end=${array[$(($offset_x-1))]}
        for offset_y in $(seq 1 20); do
            tput cup $(($global_offset_y-$offset_y)) $global_offset_x
            printf "        "
            tput cup $(($global_offset_y-$offset_y)) $global_offset_x
            printf $(($offset_y*$step))
            tput cup $(($global_offset_y-$offset_y)) $(($global_offset_x+8+$offset_x))
            if [ $end -ge $(($offset_y*$step)) ]
            then
                printf $full_plank
            else
                printf " "
            fi
        done
    done
}



cols=$(tput cols)
rows=$(tput lines)

trap ctrl_c INT

function ctrl_c() {
    tput cnorm
    tput cup $rows 0
    printf "Bye :)\n"
    exit
}

trap on_resize_redraw WINCH 

function on_resize_redraw() {
    tput clear
}

write_base=$(cat /proc/diskstats | grep "0 sda" | awk '{print $10}')
read_base=$(cat /proc/diskstats | grep "0 sda" | awk '{print $6}')
array_rd=()
array_wr=()
array_ld=()
data_unit="KiB"

full_plank=$(printf "\u2588")

number_of_cores=$(cat /proc/cpuinfo | grep processor | wc -l)

tput init
tput clear

while [ 1 ]
do
    tput civis

    new_read=$(cat /proc/diskstats | grep "0 sda" | awk '{print $6}') 
    new_write=$(cat /proc/diskstats | grep "0 sda" | awk '{print $10}')

    array_rd+=( $(( ($new_read - $read_base)*512 )) ) # in B
    array_wr+=( $(( ($new_write - $write_base)*512 )) ) # in B
    array_ld+=($(cat /proc/loadavg | awk '{print $1}'))

    read_base=$new_read
    write_base=$new_write

    # history of top 40 measurements
    if [ ${#array_ld[*]} -eq 41 ]
    then
        unset array_ld[0]
        array_ld=( "${array_ld[@]}" )
    fi

    if [ ${#array_wr[*]} -eq 41 ]
    then
        unset array_wr[0]
        array_wr=( "${array_wr[@]}" )
    fi

    if [ ${#array_rd[*]} -eq 41 ]
    then
        unset array_rd[0]
        array_rd=( "${array_rd[@]}" )
    fi

    read_speed=${array_rd[-1]}
    write_speed=${array_wr[-1]}
    read_unit="B"
    write_unit="B"

    if [ $read_speed -ge 1048576 ]
    then
        read_speed=$(($read_speed/1048576))
        read_unit="MiB"
    elif [ $read_speed -ge 1024 ]
    then
        read_speed=$(($read_speed/1024))
        read_unit="KiB"
    fi

    if [ $write_speed -ge 1048576 ]
    then
        write_speed=$(($write_speed/1048576))
        write_unit="MiB"
    elif [ $write_speed -ge 1024 ]
    then
        write_speed=$(($write_speed/1024))
        write_unit="KiB"
    fi

    tput cup 1 151
    printf "               "
    tput cup 1 140
    printf "Read $read_unit/s: $read_speed "
    tput cup 1 92
    printf "               "
    tput cup 1 80
    printf "Write $write_unit/s: $write_speed"
    tput cup 1 30
    printf "               "
    tput cup 1 10
    printf "Load AVG ($number_of_cores cores): ${array_ld[-1]}"


    for offset_x in $(seq 1 ${#array_ld[*]}); do
        end=$(bc <<< "(${array_ld[$(($offset_x-1))]}*20+0.5)/$number_of_cores")
        for offset_y in $(seq 1 20); do
            tput cup $((4+20-$offset_y)) 0
            printf "$(($offset_y*5))%%"
            tput cup $((4+20-$offset_y)) $((6+$offset_x))
            if [ $(($end+1)) -ge $offset_y ]
            then
                printf $full_plank
            else
                printf " "
            fi
        done
    done

    draw_graph "${array_wr[@]}" 20 60 24

    draw_graph "${array_rd[@]}" 20 120 24

    tput cup $rows $cols
    tput cnorm
    
    sleep 1
done