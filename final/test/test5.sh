make all
port=5678
clients=10
echo -e "starting gateway "
./sensor_gateway $port $clients &
sleep 3
echo -e 'starting 5 sensor nodes'
./sensor_node 15 1 127.0.0.1 $port &

./sensor_node 21 3 127.0.0.1 $port &

./sensor_node 37 2 127.0.0.1 $port &

./sensor_node 132 3 127.0.0.1 $port &

./sensor_node 142 3 127.0.0.1 $port &

./sensor_node 49 3 127.0.0.1 $port &

./sensor_node 129 3 127.0.0.1 $port &

./sensor_node 149 3 127.0.0.1 $port &

./sensor_node 154 3 127.0.0.1 $port &

./sensor_node 170 10 127.0.0.1 $port &
sleep 11
killall sensor_node
sleep 3
killall sensor_gateway
