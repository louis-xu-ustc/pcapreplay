#!/bin/bash

./pcapreplay -p 50101 ~/Documents/MRR/old_architecture/MVP6-A3-11082018_50101.pcap &
./pcapreplay -p 50102 ~/Documents/MRR/old_architecture/MVP6-A3-11082018_50102.pcap &
./pcapreplay -p 50103 ~/Documents/MRR/old_architecture/MVP6-A3-11082018_50103.pcap &

wait
echo all processes complete

