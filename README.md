### These codes are prototypes of the service to provide comprehensive care for the driver.


## Functions of Each Codes


### drowsy_and_steering.ino, mediapipe_pyserial


As a function for drowsy driving and negligence of driving, it appears to be normal in lcd through I2C communication in normal state, 
and when it is abnormal after critical time, warning text output, warning sound output, and window opening are executed. 
The warning sound was a piezo buzzer and the window opening was a servomotor. 
In the case of negligence of driving, the touch sensor was used because the conditions were not right. 
Communicate with Arduino and Python through Pyserial. For eye recognition, opencv, mediapipe library was used.



### pedal.ino, pedal_save.py


A code that allows you to log the record of the pedal you stepped on to log so that you can leave it as evidence when it comes to legal evidence related to driving assistance and sudden acceleration of beginners.
You can check which pedal you can press by the color of the LED, and in consideration of the skilled person, the Arduino switch was used to enable the LED to be turned on and off. 
You can save it as a log of which pedal you pressed through Pyserial. However, you need an appropriate file path and directory to save the log. 
The reason for creating the Numpy file separately is that the txt file is operable, so the non-operable Numpy is stored separately.
