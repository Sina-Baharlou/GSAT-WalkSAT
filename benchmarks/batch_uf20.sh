clear
echo ">> GSAT Solver benchmark <<"
echo ">> by Sina M.Baharlou <<"
echo ">> October 2016 <<"


folder="uf20/"
logfile="log_uf20.txt"
resultlog="log_uf20l-"
iterations=$((100))
flip_max=$((32))
rnd_method=$((-1))


gsat()
{
	count=$((0));
	echo ""> $logfile
	start_time=$(date +%s%N) ; 
	for req in $(ls $folder);
	do echo 'Solving : '$req;
	./GSAT -i $folder$req  --iterations $iterations --flip_max $flip_max --rnd_method $rnd_method --heuristic $heuristic_method  >> $logfile;
	count=$(($count+$?));
	echo "\n\n">>$logfile
	done

	end_time=$((($(date +%s%N) - $start_time)/1000000)) ; 
	echo "Time elapsed : $end_time ms." >> $resultlog$heuristic_method".txt"
	echo "Solved "$count" formulas." >>  $resultlog$heuristic_method".txt"
}

#heuristic_method=$((0))
#gsat

#heuristic_method=$((1))
#gsat

#heuristic_method=$((2))
#gsat

#heuristic_method=$((3))
#gsat

#heuristic_method=$((4))
#gsat


#heuristic_method=$((-1))
#gsat


#heuristic_method=$((5))
#gsat

#heuristic_method=$((6))
#gsat

