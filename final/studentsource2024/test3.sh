make all
port=5678
clients=3
echo -e "starting gateway "
valgrind --track-origins=yes -s ./sensor_gateway $port $clients &
sleep 3
echo -e 'starting 3 sensor nodes'
./sensor_node 15 0.1 127.0.0.1 $port &
sleep 0.02
./sensor_node 21 0.3 127.0.0.1 $port &
sleep 0.02
./sensor_node 37 0.2 127.0.0.1 $port &
sleep 1
killall sensor_node
sleep 3
killall sensor_gateway
