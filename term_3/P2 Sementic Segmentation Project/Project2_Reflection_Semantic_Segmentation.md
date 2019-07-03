
# Project Reflection: Semantic Segmentation
------

[image1]: ./images/umm_000012.png "predicted image 12"
[image2]: ./images/umm_000013.png "predicted image 13"
[image3]: ./images/umm_000017.png "predicted image 17"

[image4]: ./images/umm_000020.png "predicted image 20"
[image5]: ./images/umm_000028.png "predicted image 28"
[image6]: ./images/umm_000029.png "predicted image 29"


**This reflection will be presented with the following key points regarding to the project.**

* The **objects** to achieve;
* The main **frame** applied to meet the objects;
* How the coding meet the requirements of the **rubics**;
* **Reflections** about how to improve;


## 1. The objectives achieved
------

This project is to build and train a fully convolution network to identify the drivable area (road) in each of the given sample images.

The code of the project provides following functions,

* Load the trained vgg16 network;
* Build up FCN 8 network, by transforming fully connected layer to fully convolution layer, adding outputs of layer 3 and layer 4 of the vgg16 model as skips;
* Set the optimizer of FCN8;
* Train the FCN8 network on the Kitti data set.Meanwhile, image augmentation has been applied to improve the performance;
* Inference with the trained FCN8 network.


## 2. The frame of the project coding
------

### 2.1 Input

The inputs of the project are two data sets, including,

* The dataset from Kitti dataset for training: It includes 289 raw images of street scenes and 289 images marking out the ground truth to sepeare road and non-road pixles in the image. These images weres used to train the FCN8 network.

* Another dataset from Kitti dataset for testing: It includes 290 raw images, which are the images to infer.


### 2.2 FCN8 Model

#### 2.2.1 load the VGG16

FCN8 model is developed from VGG16. It provides a reasonable precision, does not require much computation capability and memory space. 

For time saving, VGG16 model is loaded with pretrained weights that are trained from ImageNet. This model is then used for transferred learning with its outputs from the last convolution layer, pool 3 and pool 4.

#### 2.2.2 Convert fully connected layer to fully convolution layer

The final classifier layer of VGG16 is decapitated and the fully connected layers converted to convolution layer. To fine tune the convolution layer, a deconvolution layer is adapted to upsample the coarse outputs.

#### 2.2.3 Add skip connections

As the coarse output with strides of 32 pixel limits the scale of detail in the upsampling. Two skips  are added to combine the final prediction layer with the pool 3 and pool 4 of VGG 16 using different levels of upsampling.

#### 2.2.4 Optimization operation

Adam optimizer is adopted to reduce the cross entropy loss of logits.

### 2.3 Output

The outputs are inferered images with drivable areas marked by green color.



## 3. How does the code meets the requiments of the rubic
------

### 3.1 Build the Neural Network

**Does the project load the pretrained vgg model?**

Ans: The function 'load_vgg()' is implemented with the function 'tf.saved_model.loader.load(sess, [vgg_tag], vgg_path)' to load the pretrained VGG 16 model. Moreover, the tensors of image_input, keep_prob, layer 3, layer 4 and layer 7 are also loaded with the function 'get_tensor_by_name()', as shown in the codes through line 40 to line 50 in 'main.py'. 

**Does the project learn the correct features from the images?**

Ans: To learn the correct features, the 'layers()' function converted the fully connected layer into covolution layer. The information from layer 3 and layer 4 of VGG 16 are also added up in the skipping scheme to include early stage information for segmentation. The codes are implemented through line 69 to line 121 in 'main.py'. As a result, the function layers is implemented correctly. It is also shown in the predicted images.

**Does the project optimize the neural network?**

Ans: Semantic segmentation can be considered as a classification problem. Therefore, the metric of cross entropy loss is applied to optimize the neural network. The 'Adam' optimizer is selected to minimize the cross entropy loss. These can be found through line 139 to line 147 in 'main.py'.

**Does the project train the neural network?**

Ans: The project train the neural network for 20 epoches with batch size of 15. As shown in lines through line 170 to line 182 in 'main.py', the 289 training images are divided to around 20 batches with the generative function 'get_batches_fn()'. For each of the trained batch, the loss of the network would be printed while the network is training.


### 3.2 Neural Network Training

**Does the project train the model correctly?**

Ans: On average, the model decreases loss over time. The starting loss is about 1.7 and the ending loss is about 0.05.


**Does the project use reasonable hyperparameters?**

Ans: The number of epoch and batch size are set to a reasonable number. The number of epoch is set based on the performance of the training loss, while the batch size is set based on size of the image and the space of memory in my laptop. I have a GPU that can be used to train with tensorflow, its memory size is just 4GB. With the memory of GPU, I can just train with the batch size of 1. To increase the training speed, tensorflow without GPU was used.

During training, the loss does not decrease with much improvment, i.e., less than 0.01 on average, after 20 epoches. Moreover, it takes about 10 hours to train for 20 epoches. As a result, 20 epoches is selected. 

Increasing the batch size will contribute to increase the training speed, while it will take more epoches to converge. As a result, mini-batch training was applied, with batch size of 10.

Besides, I have also compared the effects of using learning rate of 0.01 and 0.001. Decreasing the learning rate, i.e., using 0.001 as learnin rate helped to increase the performance of the network.


**Does the project correctly label the road?**

Ans: The project labels most pixels of roads close to the best solution. The model predict correctly in most of the images.

A solution that is close to best would label at least 80% of the road and label no more than 20% of non-road pixels as road.

Some of the infered results are given as follows.

![segmentation][image1]
![segmentation][image2]
![segmentation][image3]
![segmentation][image4]
![segmentation][image5]
![segmentation][image6]


## 4. Further improvements
------

Although the trained FCN8 model can segment images with high precision in most of the cases, it still fails where the transportation scenarios are complicated. There are many approaches available to improve its performance, such as,

* Using more training data, such as using dataset Cityscapes, which has around 3000 images with precise ground truth. To apply them, some pretreatments are necessary as they marked several classes of instances in each of the image.
* Using other neural networks, such as ResNet, DenseNet, GoogleNet and so on. Based on more complicated architectures, the transferred network has much more potential to improve its precision, if the computation capabilities are enough.
