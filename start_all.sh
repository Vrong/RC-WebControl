#!/bin/bash

cd native
./start_video_server.sh &
./start_control_server.sh &
cd ..
cd node;
sleep 3
nodejs main.js &
