#!/bin/bash

./pcapreplay -p 50101 ~/Documents/MRR/new_arhchitecture/MRR_mvp_one_eth0_50101.pcap &
./pcapreplay -p 50102 ~/Documents/MRR/new_arhchitecture/MRR_mvp_one_eth0_50102.pcap & 
./pcapreplay -p 50103 ~/Documents/MRR/new_arhchitecture/MRR_mvp_one_eth0_50103.pcap &

wait
echo all processes complete

