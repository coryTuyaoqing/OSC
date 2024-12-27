make all
port=5678
clients=3
./sensor_node 15 1 127.0.0.1 $port &
sleep 5
killall sensor_node
