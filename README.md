# Realtime Pothole Detection

## Idea
* Mobile phone centric detection 
* With accelerometer and gyroscope data
* 50Hz, 20ms sensor refresh rate


## Data Acquisition
__Format:__ Time-stamp, Accel X, Accel y, Accel Z, Gyro X, Gyro y, Gyro z 
![alt Data Acquisition](https://user-images.githubusercontent.com/7314342/84469528-debd9c80-ac9e-11ea-8f25-9d5a15c2948d.png)


## Data modeling 
Accelerometer data plot Raw data plot, Mean, Standard Deviation
![alt Accelerometer data plot](https://user-images.githubusercontent.com/7314342/84470416-f26a0280-aca0-11ea-8fc1-d0a50099a787.png)

Gyroscope data plot Raw data plot, Mean, Standard Deviation
![alt Gyroscope data plot](https://user-images.githubusercontent.com/7314342/84470469-0a418680-aca1-11ea-9ade-d4b96ba2bf5a.png)

Confusion matrix
![alt Confusion matrix](https://user-images.githubusercontent.com/7314342/84469612-1af0fd00-ac9f-11ea-8e7d-966416111ba9.png)

## Export model from MATLAB to C code
Export trained model to C code using MATLAB coder

## Import model C code in Android Studio
Use JNI to access the svmPredict() function

## TODO 
Calibration for running the trained model based on initial orientation of the smartphone.

Calibration shall provide a rotation matrix for coordinate transformation between the orientation used while obtaining the data and the orientaiton of the phone during the detection.


## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.

Presentation slides:
https://docs.google.com/presentation/d/1QHmCzykV8gG-gyEe1Cde11WV_VeJjogQ9z2-AZNWqvg/edit#slide=id.p

