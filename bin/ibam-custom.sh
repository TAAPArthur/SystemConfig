#!/bin/bash
timeLeft=$(ibam |tail -n1|cut -d ':' -f2-3|tr -d ' ')

status="Char"
hr=$(echo $timeLeft |cut -d ':' -f1)
min=$(echo $timeLeft |cut -d ':' -f2)
if [[ ! $(ibam |grep charge) ]]; then 
	percentBatt=$(ibam --percentcharge |grep -Po "[[:space:]]* \K[0-9]+ %"|cut -d" " -f1)
	if [ "$percentBatt" -lt 3 ] || ([ $hr -eq 0 ] && [ $min -lt 2 ]); then
	        notify-send "Low power"
			#echo -n "Low power"
			#systemctl suspend
			pass
			
	fi
	status="Dis"
fi

percent=$(ibam --percentcharge |head -n1|cut -d ':' -f2-3|tr -d ' ')

format=""
if [ "$1" == "-f" ]; then
	format=true
	if [ $status == "Char" ]; then
		echo -n "<fc=#00FFFF>"	
	elif [ $hr -gt 0 ]; then
		echo -n "<fc=#00FF00>"	
	elif [ $min -lt 30 ]; then
		echo -n "<fc=#FF0000>"	
	else
		echo -n "<fc=#0000FF>"	
	fi
fi
if [ "$1" == "-c" ]; then
	if [ $status == "Char" ]; then
		echo -n "\${color 00FFFF}"	
	elif [ $hr -gt 0 ]; then
		echo -n "\${color 00FF00}"	
	elif [ $min -lt 30 ]; then
		echo -n "\${color FF0000}"	
	else
		echo -n "\${color 0000FF}"	
	fi
fi

printf "%4s%9s" "$percent" "($timeLeft""hr)"
 
if [ $format ]; then
	echo "</fc>"
else 
	echo ""
fi
