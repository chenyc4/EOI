nums=$1
i=1
while [ $i -le $nums ]
do
	client_log=client_$i
	./client > $client_log & 
	echo $client_log" is running"
	i=$(expr $i + 1)
done

