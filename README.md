# CV Data Set Tests

Tests of reading in face image data sets using OpenCV.

## INRIA Head Pose Image Database

* [Webpage](http://www-prima.inrialpes.fr/perso/Gourier/Faces/HPDatabase.html)
* [Source paper](http://www-prima.inrialpes.fr/perso/Gourier/Pointing04-Gourier.pdf)

2790 total color face images. There are two series of images for each of fifteen people; each series consists of 
ninety-three images. Within a series, tilt angles can be positive or negative {0, 15, 30, 60, 90}, and pan angles can 
be positive or negative {0, 15, 30, 45, 60, 75, 90}.

## CMU Face Images

* [Webpage](https://archive.ics.uci.edu/ml/machine-learning-databases/faces-mld/faces.data.html)
* [Source text's page](http://www.cs.cmu.edu/afs/cs.cmu.edu/user/mitchell/ftp/faces.html)

640 total grayscale face images. There are thirty-two images for each of two people where the following attributes vary:

* pose: straight, left, right, up
* expression: neutral, happy, sad, angry
* eyes: sunglasses, no glasses
* size

Sixteen data files have _.bad_ extensions instead of _.pgm_ and are not usable; at least twenty-eight images for each 
person are valid.

## NCKU Robotics Lab's Database

* [Webpage](http://robotics.csie.ncku.edu.tw/Databases/FaceDetect_PoseEstimate.htm#Our_Database_)

6660 total color face images. There are seventy-four images for each of ninety people; within a set, the pan angle 
varies in 5&deg; increments from -90&deg;to +90&deg;.
