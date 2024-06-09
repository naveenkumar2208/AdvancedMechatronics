import cv2 as cv  # importing openCV library as cv
import cv2.aruco as aruco  # importing openCV aruco module as aruco
import serial as sr  # importing pyserial library as sr
from picamera.array import PiRGBArray  # importing picamera array
from picamera import PiCamera  # importing picamera
# https://stackoverflow.com/questions/26169633/capturing-video-with-raspberry-using-opencvpicamera-stream-io


class arUcoDetector:  # Defining class
    def __init__(self, resolution = (1920, 1080), fps = 32, minMarkerPerimeterRate = 0.1, maxMarkerPerimeterRate = 4,
                 minDistanceToBorder = 3, port = '/dev/ttyUSB0',
                 baudRate = 115200):  # Defining the initialization parameters with default values
        self.camera = PiCamera()  #  creating object picamera
        self.camera.resolution = resolution  #  setting the resolution 
        self.camera.framerate = fps  # setting the frame rate
        self.rawCapture = PiRGBArray(self.camera, size = resolution)  #  turing it raw data
        self.arUcoDict = aruco.Dictionary_get(aruco.DICT_6X6_50)  # Creating variable for aruco dictionary
        self.arUcoParams = aruco.DetectorParameters_create()  # Creating variable for parameters
        self.arUcoParams.minMarkerPerimeterRate = minMarkerPerimeterRate  # Setting the parameters
        self.arUcoParams.maxMarkerPerimeterRate = maxMarkerPerimeterRate  # Setting the parameters
        self.arUcoParams.minDistanceToBorder = minDistanceToBorder  # Setting the parameters
        while True:  # infinite loop
        	try:  # try block
        		self.ser = sr.Serial(str(port), baudRate)  # Setting the ser object by specifying port and baud rate
        	except (FileNotFoundError, sr.SerialException):  # Except block to allow only these error
        		pass  # Pass the error to continue in the loop
        	else:  # Else statement
        		break  # break statement

    def detect(self):  # defining function detect
        for image in self.camera.capture_continuous(self.rawCapture, format = "bgr", use_video_port = True):  # for loop
            frame = image.array  # reading the video and saving it to a matrix frame for each time step
            frame = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)  # Removing the BGR Channels to grayscale so only 500 x 500 x 1 pixels
            #frame = cv.rotate(frame, cv.ROTATE_180) #rotating frame for easy visuals
            [corners, ids, _] = aruco.detectMarkers(frame, self.arUcoDict, parameters = self.arUcoParams)  # getting corners and ids
            frame = aruco.drawDetectedMarkers(frame, corners, ids = ids)  # Drawing markers on each frame
            if ids is None:  # If no tags are detected
                self.ser.write(str.encode("2"))  # Send 2 to propeller
                print("2")  # Printing 2 for testing
            elif 0 <= ids <= 9:  # If tag ID are in range of 0 to 9
                self.ser.write(str.encode("0"))  # It will send 0 to propeller
                print("0")  # print 0 for testing
            elif 10 <= ids <= 19:  # If tag ID are in the range 10 to 19
                self.ser.write(str.encode("1"))  # It will send 1 to propeller
                print("1")  # print 1 for testing
            cv.imshow("arUco Tags", frame)  # show each frame
            self.rawCapture.truncate(0)  # clearing data from the array for next frame
            if cv.waitKey(1) & 0xFF == ord('q'):  # If wait is done for 1 sec bit wise and is the ordinal value q which is 127 on keyboard
                break  # break statement to exit infinite loop
        cv.destroyAllWindows()  # destroying all windows


while True: #  Infinite loop
    try:  # Try block
        detector = arUcoDetector((240, 160), 90 ,0.4, 100, 3, '/dev/ttyUSB0', 115200)  # Creating an object with parameters
        detector.detect()  # Calling function to detect
    except:  # Except block to only allow these errors
    	del detector  # Deleting the object
    else:  # Else statement
        break #  Break statement
