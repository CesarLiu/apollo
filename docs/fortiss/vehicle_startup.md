This document will guide you through the process of bringing up the apollo system on the fortiss research vehicle.

# Vehicle prerequisites
1. Unplug the high voltage charger
2. Plug the wheel odometer (if you want)

# Boot the vehicle
1. Iginition is off, Measurement system is off (red LED Klemme30 is off)
2. Unpress the emergency stop button
3. Turn the MAIN key switch for approx. 1s (red LED Klemme30 is on)
4. Wait until the system has fully booted (Statusdisplay shows six status displays) 
5. Turn on the engine


# Turn the vehicle off
(reverse order than booting)
1. Turn of the engine
2. Wait 10s 
3. Turn MAIN key switch for approx. 1s 
4. Statusdispaly shows countdown 60s
5. Wait until shutdown is completed
6. Press emergency stop button

# Apollo prerequisites
1. (optional) start PTP time synchronization setup
2. Start gps manual alignment
   - You have to have gps reception for that
   - open imar gui on PC1: `bash ~/iXCOM_CMD/run.sh`
   - Klick the connect button
   - Klick the static align button 
   - Once you have gps reception, a counter starts from 200s
   - Don't move the vehicle in this period
   - Afterwards, you may move the vehicle and after some meters you should have an accurate orientation

# Apollo system setup
1. Navigate to `~/fortuna_ws/apollo_opensource` on either PC and start apollo (dev_start, dev_into)
2. Start the driver components on PC1: `bash scripts/fortiss/start_drivers_PC1.sh`

You are now free to start the apollo components as you like. You can use one PC, or PC1 and PC2 and a laptop. I usually start the components in different terminal tabs, but you can also use tmux, specify a launch file, etc. 

3. Start dreamview: `bash scripts/bootstrap.sh start`
4. Start autobox gateways (if you need them): `bash scripts/fortiss/start_autoBox_channels_dspace.sh`
5. Start lidar perception (if you need it)
6. Start routing
7. Start prediction of fake prediction
8. Start planning


# Troubleshoot
1. I dont see anything in the cyber_monitor but I started some components --> Are your consoles sourced correctly? check `cyber/setup.bash`, CYBER_IP. This IP should be the IP of the PC at hand in the network (aka.  192.168.140.* or localhost). If you have a distributed setup, you have to set a network IP. Source the consoles via `source cyber/setup.bash`

# Turn apollo off
1. You can stop the components, but you dont have to

# Post steps
Once you have switched off the measurement system:
1. Unplug the odometer and close the window
2. Plug the high voltage charger
3. Lock the car
4. Trigger charging the HV battery with the charging card on the wallbox