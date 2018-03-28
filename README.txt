******Implemented in duration October 2015 to February 2016 in Embedded C******

Coding for IIT Bombay's E-Yantra Robotics Programme
Hardware platform used is Firebird V, Microcontroller is Atmega 2560
Environment used : Atmel Studio 7
Run the main.c file

Background :
e-Yantra robotics involved a simulation of pizza delivery system.
The scenario involved picking colour coded mock pizza boxes from the mock delivery stations,
and delivering them to houses spread out on the flex.  

Challenges involved designing a robotic arm, choosing the pizzas to deliver keeping the time constraint
and order of delivery in mind.
Robotic arm involved use of 2 hobby servo motors and a micro servo motor for "pinching action" to pick the
pizza boxes. 
Distance sensing was enabled using Sharp sensors, with color sensors mounted on the robotic arm to determine
the colour code of the pizza box. Various counters and timers were initialized to trigger interrupts (such as to
increment time on SSD)

More details are mentioned as comments in the main.c file
(Functions with comments associated with it have been written by me, the remaining few functions have been
provided by e-Yantra team to ease our programming efforts. Details are mentioned in the code)