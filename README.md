# Tank
Tank Sensor
Summary: Use an ultrasonic sensor to determine the level in a tank of fluid
Primary Functions:
 - Read value from ultrasonic sensor (raw data)
 - interpolate distance from calibrated values (defined fixed variables)
 - Calculate tank level (depth) based on min max settings (adjuctsable values)
 - Display tank level
 - (option) estimate fluid volume/percentage based on container geometry
Secondary Functions
 - Setup mode: pushbutton or switch to enter Setup mode to adjust min max value (either based on real-time readings or incremented with buttons
 - Sleep function: use timer interrrupts to only read at sepcific intervals. alternatively to "wake" unit on button push
 - Logger: store incremental readings over time to determine consumate rate (linear per 7 days?) and project time to empty 
