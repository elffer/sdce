
## Writeup

---

**Vehicle Detection Project**

The goals / steps of this project are the following:

* Apply a color transform and append binned color features, as well as histograms of color, and a Histogram of Oriented Gradients (HOG) feature extraction on a labeled training set of images and train a classifier Linear SVM classifier
* For the steps to extract featrues to train the classifier, I have used the scaler to normalize the the features from the training images and test images.
* Implement a sliding-window technique and use the trained classifier to search for vehicles in images.
* Run the image processing pipeline on a video stream and create a heat map of recurring detections frame by frame to reject outliers and follow detected vehicles.
* Estimate a bounding box for vehicles detected.

[//]: # (Image References)
[image1]: ./images/3_car_noncar.png
[image2]: ./images/4_hog_features.png
[image3]: ./images/6_sliding_windows.png
[image4]: ./images/11_img_with_windows.png
[image51]: ./images/121_heatmap.png
[image52]: ./images/122_heatmap.png
[image53]: ./images/123_heatmap.png
[image54]: ./images/124_heatmap.png
[image6]: ./images/13_labelled.png
[image7]: ./images/5_detection_image.png
[video1]: ./project_video.mp4

## Rubric Points
### Here I will consider the rubric points individually and describe how I addressed each point in my implementation.  

---
### Writeup / README

#### 1. Provide a Writeup / README that includes all the rubric points and how you addressed each one.  You can submit your writeup as markdown or pdf.  [Here](https://github.com/udacity/CarND-Vehicle-Detection/blob/master/writeup_template.md) is a template writeup for this project you can use as a guide and a starting point.  

You're reading it!

### Histogram of Oriented Gradients (HOG)

#### 1. Explain how (and identify where in your code) you extracted HOG features from the training images.

The code for this step is contained in the first and second code cell of the IPython notebook.  

I started by reading in all the `vehicle` and `non-vehicle` images.  Here is an example of one of each of the `vehicle` and `non-vehicle` classes:

![alt text][image1]

I then explored different color spaces and different `skimage.hog()` parameters (`orientations`, `pixels_per_cell`, and `cells_per_block`).  I grabbed random images from each of the two classes and displayed them to get a feel for what the `skimage.hog()` output looks like.

Different color spaces were tested including `RGB`, `HLS` and `YCrCb`. The highest accuracy of using `RGB` on the test samples is 0.995, while the top value of the accuracy by using `YCrCb` is 1. Theoretically, the color space `YCrCb` benifits from reducing the correlation.  As a result the color space of `YCrCb` is choosen for the color spaces.

Here is an example to show the HOG features of images with vehicle and without vehicle, using the `YCrCb` color space and HOG parameters of `orientations=9`, `pixels_per_cell=(8, 8)` and `cells_per_block=(2, 2)`:


![alt text][image2]

#### 2. Explain how you settled on your final choice of HOG parameters.

The main HOG parameters to adjust in the function of `skimage.feature.hog()` include `orientations`, `pixels_per_cell` and `cells_per_block`. I tried various combinations of parameters, i.e., `orientations={6, 9, 12}`, `pixels_per_cell={(4, 4), (8, 8)}` and `cells_per_block={(1, 1), (2, 2), (4, 4)}`. The final set of HOG parameters are settled as `orientations=9`, `pixels_per_cell=(8, 8)` and `cells_per_block=(2, 2)`, based on which the trained classifier performs the highest accuracy.

#### 3. Describe how (and identify where in your code) you trained a classifier using your selected HOG features (and color features if you used them).

I trained a classifier to detect vehicle in a image. The main steps used to train the classifier are given as follows,
* 1. Explore and import the training images, as shown in the 1st and 3rd cell of the Ipython notebook;
* 2. Apply feature engineering on these images, i.e., feature selection and feature pretreatment. The features selected in the project include spatial color features, color histogram features and hog features as shown in the 20th cell of the Ipython notebook. The main approach for feature pretreatment is normalization of the training and test features.
* 3. Split the features and labels into trainnig and testing samples as shown in the 21th cell of the Ipython notebook.
* 4. Fit and train the classfier with machine learning models, i.e., svm as used in this project. Moreover, the approach of grid search based on cross validation was applied in this section to tune some parameters of the model. This can be found in the 22th cell of the Ipython notebook.

Then the trained classifier can be applied to detect whether a vehicle exists in a sliced section of an image.



### Sliding Window Search

#### 1. Describe how (and identify where in your code) you implemented a sliding window search.  How did you decide what scales to search and how much to overlap windows?

For the sliding window search, there are three kinds of parameters tuned to optimize the detection.
* 1. The region in an image to slide. The upper portion is mainly taken up by the sky and other non-vehicle objects. The lower part mainly is manily taken by the hood of the vehicle where the camera is mounted. As a result, only the middle partion, i.e., pixels with y value locate in the range of about (350, 650).
* 2. Size of the sliding windows. As shown in the training images, the idea windows are the ones that perfectly match the size of vehicles in the images. In the project video, vehicles present different sizes due to their distances from the camera. Accordingly, several sizes of windows were applied to search vehicles, i.e., (32, 32), (48, 48), (64, 64) and (128, 128). 
* 3. Overlap of sliced windows. Increasing the overlap will provide better match on vehicles, while it will lead to increased number of windows to predict.

The scales and the overlap of windows are decided by applying the detection function on all the test images, i.e., using grid search to find the best combination of them. The setting of these parameters can be found in the 23rd and 24th cells of the Ipython notebook. In the image below, it illustrates what the sliding windows are like with blue lines on the original image.

![alt text][image3]

#### 2. Show some examples of test images to demonstrate how your pipeline is working.  What did you do to optimize the performance of your classifier?

 To optimize the performance of the classfier, the main approaches that are applied in the project include,
* The ratio of between the number of train/test samples: For cross validation, the ratio of test set was set as 0.33. And this ratio was changed to 0.1 to increase the size of the training set when the model is fixed. See the codes in the 21th cell of the Ipython notebook.
* Selection of the features: The main improvements from converting the images from the color space of `RGB` to the color space of `YCrCb`, and using all the hog features. The codes are provided in the 20th cell of the Ipython notebook.
* Normalization of the features with a scaler: The features from the training and the test images are normalized with the function `sklearn.preprocessing.StandardScaler()`. This is crucial to improve the performance of svc model.
* Selection of machine learning models for the task of classifier: Besides of svm, the model of random forest was tested as well, as it is usually an ideal model applied in AI competitions in Kaggle. However, the accuracy of the random forest model is much lower than svm in my project. The codes for the model of classifiers are provided in the 22th cell of the Ipython notebook. The models of svm and random forest are trained using the functions of `sklearn.ensemble.RandomForestClassifier()` and `sklearn.model_selection.GridSearchCV()`.
* Tuning of the parameters: The parameters of the classifer were tuned using the grid search approach, with the function `sklearn.model_selection.GridSearchCV`. The parameters for svc are tuned with values given by `parameters = {'kernel':('linear', 'rbf'), 'C':[1, 10, 50]}`. The codes were given in the 22nd cell of the Ipython notebook.

Ultimately I searched on three scales using YCrCb 3-channel HOG features plus spatially binned color and histograms of color in the feature vector, which provided a nice result.  Here are some example images:

![alt text][image4]
---

### Video Implementation

#### 1. Provide a link to your final video output.  Your pipeline should perform reasonably well on the entire project video (somewhat wobbly or unstable bounding boxes are ok as long as you are identifying the vehicles most of the time with minimal false positives.)
Here's a [link to my video result](output_video/final_video.mp4)


#### 2. Describe how (and identify where in your code) you implemented some kind of filter for false positives and some method for combining overlapping bounding boxes.

I recorded the positions of positive detections in each frame of the video.  From the positive detections I created a heatmap and then thresholded that map to identify vehicle positions.  I then used `scipy.ndimage.measurements.label()` to identify individual blobs in the heatmap.  I then assumed each blob corresponded to a vehicle.  I constructed bounding boxes to cover the area of each blob detected.  

Besides, I have also adopted a queue to store the labeled boxes from continuious frames, as shown in the 24th cell of the Ipython notebook. The box list from this queue also contributes to reduce the chance of false positive as well.

Here's an example result showing the heatmap from a series of frames of video, the result of `scipy.ndimage.measurements.label()` and the bounding boxes then overlaid on the last frame of video:

### Here are four frames and their corresponding heatmaps:

![alt text][image51]
![alt text][image52]
![alt text][image53]
![alt text][image54]

### Here is the output of `scipy.ndimage.measurements.label()` on the integrated heatmap from all six frames:
![alt text][image6]

### Here the resulting bounding boxes are drawn onto the last frame in the series:
![alt text][image7]



---

### Discussion

#### 1. Briefly discuss any problems / issues you faced in your implementation of this project.  Where will your pipeline likely fail?  What could you do to make it more robust?

The main prolbems faced in the implementation of the project

Here I'll talk about the approach I took, what techniques I used, what worked and why, where the pipeline might fail and how I might improve it if I were going to pursue this project further.

There were several problems I met in developing the project. 
* The first one is about the accuracy of the model. I tried RGB at the early stages, while the accuracy is low and caused many false positive detection. This was overcomed by comparing different kind of color spaces. 
* The second sort of challenge is the detecting different size of vehicles in the image. I simply tried the size of 32 by 32, while it fails to detect vehicles in most cases. Larger sizes, such as 64 by 64 and 128 by 128, fails to detect vehicle in the test3.jpg as provided by the class.
* The third kind of false positive comes up randomly. The essential solution to reduce them is generating a box queue, which stores positive detections from continious frames to smooth the detection.

There are several scenarios that this pipeline will fail, mainly due to the limitation of the training set. These potential scenarios include:
* There are other sorts of vehichles on the road, such as trucks, buses, pickups and so on.
* There are vehicles running in a different direction on the left lane, i.e., coming face to face.
* The illumination of the scenario is not well.

To overcome these limitation, the model can be improved by gathering more images that contains different sort of vehicles with various limitation conditions. Moreover, applying deep learning algorithms will greatly improve the performance on vehicle detection in various scenarios.
