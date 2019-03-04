
# **Traffic Sign Recognition** 

## Writeup by Elffer

---

**Build a Traffic Sign Recognition Project**

The pipeline of this project is given as follows:
* Load the data set (see below for links to the project data set)
* Explore, summarize and visualize the data set
* Design, train and test a model architecture
* Use the model to make predictions on new images
* Analyze the softmax probabilities of the new images
* Summarize the results with a written report


[//]: # (Image References)

[image1]: ./output/static_fig/distribution.png "Visualization"
[image2]: ./examples/before_gray_scale.png "Grayscaling"
[image21]: ./examples/after_gray_scale.png "Grayscaling 2"
[image3]: ./examples/before_addblock.png "Add block"
[image31]: ./examples/after_addblock.png "Add block2"
[image4]: ./examples/downloaded/1_speed_limit_30.jpg "Traffic Sign 1"
[image5]: ./examples/downloaded/11_right_of_way.jpeg "Traffic Sign 2"
[image6]: ./examples/downloaded/16_vehicle_over_metric_prohibited.jpg "Traffic Sign 3"
[image7]: ./examples/downloaded/25_Road_work_2.jpg "Traffic Sign 4"
[image8]: ./examples/downloaded/40_round_about.jpg "Traffic Sign 5"

## Rubric Points
### Here I will consider the [rubric points](https://review.udacity.com/#!/rubrics/481/view) individually and describe how I addressed each point in my implementation.  

---
### Writeup / README

#### 1. Provide a Writeup / README that includes all the rubric points and how you addressed each one. You can submit your writeup as markdown or pdf. You can use this template as a guide for writing the report. The submission includes the project code.

You're reading it! and here is a link to my [project code](https://github.com/elffer/sdce/blob/master/term_1/P2/Traffic_Sign_Classifier_Elffer_v3.ipynb)

### Data Set Summary & Exploration

#### 1. Provide a basic summary of the data set. In the code, the analysis should be done using python, numpy and/or pandas methods rather than hardcoding results manually.

I used the numpy and pandas libraries to calculate summary statistics of the traffic
signs data sets:

* The size of training set is 34799
* The size of the validation set is 4410
* The size of test set is 12630
* The shape of a traffic sign image is (32, 32, 3)
* The number of unique classes/labels in the data set is 43

#### 2. Include an exploratory visualization of the dataset.

Here is an exploratory visualization of the data set. It is a bar chart showing the distribution of image sets, where the training set, valid set and test set are plot in red, green and blue, respectively. Some insights are captured from this image, i.e., 
* The training set, valid set and test set have similar distribution in considering the sign types;
* The ratio of different type of traffic signs are not well balanced. Some signs, from 0 to 10 have much greater ratios, while the proportion of signs from 25 to 30 are quite low.
* Compared with the sample size of training set and test set, the samepe size of validation set is less than enough.

![alt text][image1]

### Design and Test a Model Architecture

#### 1. Describe how you preprocessed the image data. What techniques were chosen and why did you choose these techniques? Consider including images showing the output of each preprocessing technique. Pre-processing refers to techniques such as converting to grayscale, normalization, etc. (OPTIONAL: As described in the "Stand Out Suggestions" part of the rubric, if you generated additional data for training, describe why you decided to generate additional data, how you generated the data, and provide example images of the additional data. Then describe the characteristics of the augmented training set like number of images in the set, number of images for each class, etc.)

Before setting up the model, I decided to convert the images to grayscale as it is recommended in the literature. However, it turned out to be not so effective in my model. 

Here is an example showing a traffic sign image before and after converted to grayscale.

![alt text][image2]

![alt text][image21]

Then, I normalized the images as this can help to speed up the training of neural networks, and reduce the impact of scale in the process of learning.

Moreover, I decided to generate additional data because the sample size is not quite large, considering that there are so many types of signs.

To add more images to the the data sets, I used four techniques for image augmentation:
* shift
* add noise
* blur
* add block

In fact, there are much more options for image operation available. In each of the batches, the images are augmented randomly by selecting one of the augmentation techniques. Image augmentation enhanced the validation accuracy of my architecture from about 0.92 to higher than 0.94.

Here is an example of an original image and an augmented image, where an random block was added to the original image,

![alt text][image3]

![alt text][image31]

To save the memory, the augmented images are not stored, but generated based on each batch of selected images, and applied for model training, directly. 


#### 2. Describe what your final model architecture looks like including model type, layers, layer sizes, connectivity, etc.) Consider including a diagram and/or table describing the final model.

In this project, the LeNet was applied to train and predict the traffic sign. The final model consisted of the following layers:

| Layer         		|     Description	        					| 
|:---------------------:|:---------------------------------------------:| 
| Input         		| 32x32x3 RGB image   							| 
| Convolution 5x5      	| 1x1 stride, valid padding, outputs 28x28x6 	|
| ELU					|												|
| Max pooling	      	| 2x2 stride,  outputs 14x14x6  				|
| Convolution 5x5	    | 1x1 stride, valid padding, outputs 10x10x16	|
| ELU					|												|
| Max pooling	      	| 2x2 stride, outputs 5x5x16    				|
| Flattern       		| outputs 400  									|
| Fully connected		| outputs 120  									|
| ELU					|												|
| Fully connected		| outputs 84  									|
| Dropout				| probability 0.5        						|
| ELU					|												|
| Fully connected		| outputs 43  									|


#### 3. Describe how you trained your model. The discussion can include the type of optimizer, the batch size, number of epochs and any hyperparameters such as learning rate.

To train the model, some extra essential setting of the model are given as follows,
* Type of optimizer: Adam optimization
* Batch size: 64
* Number of epochcs: 40
* learning rate: 0.001
* Dropout probability: 0.5

#### 4. Describe the approach taken for finding a solution and getting the validation set accuracy to be at least 0.93. Include in the discussion the results on the training, validation and test sets and where in the code these were calculated. Your approach may have been an iterative process, in which case, outline the steps you took to get to the final solution and why you chose those steps. Perhaps your solution involved an already well known implementation or architecture. In this case, discuss why you think the architecture is suitable for the current problem.

My final model results were:
* validation set accuracy of 0.957 
* test set accuracy of 0.932

If an iterative approach was chosen:
* What was the first architecture that was tried and why was it chosen?

Ans: The first architecture was the LeNet without dropout, elu and image augmentation. It was choosen as its structure is convinient to apply and easy to train.

* What were some problems with the initial architecture?

Ans: Both the validation accuracy and the test accuracy of the initial architecture are less than 0.9. Moreover, the initial architecture overfit the training set.

* How was the architecture adjusted and why was it adjusted? Typical adjustments could include choosing a different model architecture, adding or taking away layers (pooling, dropout, convolution, etc), using an activation function or changing the activation function. One common justification for adjusting an architecture would be due to overfitting or underfitting. A high accuracy on the training set but low accuracy on the validation set indicates over fitting; a low accuracy on both sets indicates under fitting.

Ans: To improve the performance of the LeNet from the course, I changed the activation function from ReLU to ELU, added a dropout layer and even tried the AlexNet. As suggested by the literatue, ELU outperformed all the ReLU variants in their experiment in both the accuracy and time saving. It also works for my architecture. The dropout applied in my model helped to reduce the drawback of overfitting. Besides, I have also tried AlexNet. However, the kernel of my jupyter simply flashed out that it did not run succesively. Some extra works, such as reducing the batch size and debuging might be needed.

* Which parameters were tuned? How were they adjusted and why?

Ans: I have adjusted the batch size of the architechture. Due to the application of image augmentation for each batch of images, it doubled the size of each batch. As a result, I have reduced the batch size from 128 to 64. The epochs has also been adjusted to achieve the effect of early stop, that also reduced the time for training.

* What are some of the important design choices and why were they chosen? For example, why might a convolution layer work well with this problem? How might a dropout layer help with creating a successful model?

Ans: Important design choices include dropout and ELU. As the architecture is based on LeNet, convolution layers are also adopted. The convolution layers contributed to extract low level features and assemble them to higher-level features. In another word, they are the essential tools to extract features of images. Dropout is the most popular regularization technique and helps to reduce the error caused by overfitting. During training, dropout helps to increase the adaption of the neurons to different inputs. Considering the activation function, ReLU suffers from a problem of dying.

If a well known architecture was chosen:
* What architecture was chosen?

Ans: The LeNet was chosen.

* Why did you believe it would be relevant to the traffic sign application?

Ans: This architectue is based on convolutional networks which are proven to be well suited for image classfication in both literature and competitions. Convolutional layers are effective in extracting features in images.

* How does the final model's accuracy on the training, validation and test set provide evidence that the model is working well?

Ans: Both the validation accuracy and test accuracy achieves higher than 0.93. It shows that the model is working well on the given sets.


### Test a Model on New Images

#### 1. Choose five German traffic signs found on the web and provide them in the report. For each image, discuss what quality or qualities might be difficult to classify.

Here are five German traffic signs that I downloaded from the web:

![alt text][image4] ![alt text][image5] ![alt text][image6] 
![alt text][image7] ![alt text][image8]

Compared with the training set, these images might be difficult to classify for specific constraints, i.e.,
* The first image might be difficult to classify because the background contains some extra objects, such as the pole and the hedge. Besides, the size of this image is much larger than 32 by 32. The compression would also affect the accuracy of prediction.
* The second image states the right of way is an idea sign to predict. A small part of another sign might affect the accuracy.
* The third image is difficult to recognize as the sign just takes up a small part of the image. The house and extra sign with words might cause much distraction for prediction.
* The fourth image also need to be compressed, with its bachground being noisy and covered by some words on the sign.
* The fifth image has the challenge of noisy background, distraction from extra sign and block by the soursage on the sign.


#### 2. Discuss the model's predictions on these new traffic signs and compare the results to predicting on the test set. At a minimum, discuss what the predictions were, the accuracy on these new predictions, and compare the accuracy to the accuracy on the test set (OPTIONAL: Discuss the results in more detail as described in the "Stand Out Suggestions" part of the rubric).

Here are the results of the prediction:

| Image			        |     Prediction	        					| 
|:---------------------:|:---------------------------------------------:| 
| 30 km/h      	    	| 30 km/h  	    								| 
| Right of Way			| Right of Way      							|
| vehicle over_metric	| 80 km/h 										|
| Road Work				| Road Work	    								|
| Round About			| Round About       							|


The model was able to correctly guess 4 of the 5 traffic signs, which gives an accuracy of 80%. Generally, the performance on the new images is worse than the accuracy on the test set. These results were based on the signs that have been specially segmentated from their background. The background objects and extra objects covering on the images can greatly affect the accuracy of prediction.

#### 3. Describe how certain the model is when predicting on each of the five new images by looking at the softmax probabilities for each prediction. Provide the top 5 softmax probabilities for each image along with the sign type of each probability. (OPTIONAL: as described in the "Stand Out Suggestions" part of the rubric, visualizations can also be provided such as bar charts)

The code for making predictions on my final model is located in the 10th cell of the Ipython notebook.

For the first image, the model is relatively sure that this is a 30 km/h limit sign (probability of 0.92), and the image does contain a 30 km/h limit sign. The top five soft max probabilities were

| Probability         	|     Prediction	        					| 
|:---------------------:|:---------------------------------------------:| 
| .92         			| 30 km/h   									| 
| .06     				| 60 km/h										|
| .01					| 80 km/h									 	|
| .00	      			| End of all speed and passing limits			|
| .00				    | 50 km/h    					        		|

For the second image, the model is sure that this is a right of Way sign (probability of 1), and the image does contain a right of way sign. The top five soft max probabilities were

| Probability         	|     Prediction	        					| 
|:---------------------:|:---------------------------------------------:| 
| 1.00         			| Right of Way  								| 
| .00     				| Beware of ice/snow							|
| .00					| End of no passing							 	|
| .00	      			| Slippery road					 				|
| .00				    | Double curve     	    						|

For the third image, the model predicted that this is a 80 km/h sign, while, in fact, it is a vehicle over metric sign. The top five soft max probabilities were

| Probability         	|     Prediction	        					| 
|:---------------------:|:---------------------------------------------:| 
| .99         			| Stop 					        				| 
| .00     				| Road work										|
| .00					| No passing for vehicles over 3.5 metric tons	|
| .00	      			| Traffic signals				 				|
| .00				    | No entry    			        				|

For the fourth image, the model predicted that this is bicycles crossing sign (probability of 0.97), while it is infact a road work sign. The top five soft max probabilities were

| Probability         	|     Prediction	        					| 
|:---------------------:|:---------------------------------------------:| 
| .97         			| bicycles crossing								| 
| .03     				| road work										|
| .00					| Bumpy road								 	|
| .00	      			| Stop					        				|
| .00				    | Wild animals crossing     					|

For the fifth image, the model is sure that this is a round about sign (probability of 0.98), and the image does contain a round about sign. The top five soft max probabilities were

| Probability         	|     Prediction	        					| 
|:---------------------:|:---------------------------------------------:| 
| .98         			| round about  									| 
| .01     				| Beware of ice/snow							|
| .00					| End of no passing by vehicles over 3.5 tons 	|
| .00	      			| Slippery road				 	    			|
| .00				    | Double curve     			    				|

Note that, the process of predicting the signs and the process of testing the top 5 probabilities are operated seperatly. As a result, the predictiong accuracies are different.

### (Optional) Visualizing the Neural Network (See Step 4 of the Ipython notebook for more details)
#### 1. Discuss the visual output of your trained network's feature maps. What characteristics did the neural network use to make classifications?



