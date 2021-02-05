controller_params = {};
controller_params.k4 = 2;
controller_params.k3 = 0.9;
controller_params.k2 = 0.7;
controller_params.k1 = 0.4;
controller_params.J = 5000.0; % unused, only needed to output the force
controller_params.m = 2000.0; % unused, only needed to output the force
controller_params.l_v = 2.786/2.0;
controller_params.l_h = 2.786/2.0;

controller_max_time_diff = 100;

switch_steering_feedback = 1; % 0:hack internal feedback, 1:from can 
switch_velocity_feedback = 1; %0:from can, 1:from loca

% comfortable/safely handleable limitiation, not the technical limitations
% of the vehicle
max_steering_speed_comfort = 2*0.61/0.01; % aka maximum range / dt
max_acceleration_comfort = 2;
max_deceleration_comfort = -4.5;
max_long_jerk_comfort = 200/2;
min_long_jerk_comfort = -450/2;

% convert the steering commands from the controller to steering angle values for the vehicles interfaces
steering_angle_range_rad_to_steering_wheel_angle_range_deg_gain = 14.8828*180/pi;

% Parameters for apollo controller
apollo_control_timeout = 10;

