# GraspingTask
This github as the aim to regroup all the codes that were used to make my 3 months' internship possible
The first branch and the more important is the "graspingtask" branch. It is composed of five files.

In order to run everything here is the different steps necessary :

- in a first terminale : roscore 
- make sure that the Arduino code topicsensor.ino is compiled and running
- in a second one : rosrun rosserialpython serial_node.py /dev/tty/USB0 (USB0 should be replaced by the USB port of the arduino uno port)
- in another terminale : rosrun control firsttry
- in a last terminale : ./objectdetection


Two other files are in this branch : the h5 file is the neural network trained with tensorflows and the fingerstests.csv file is the file with the fingers force data
