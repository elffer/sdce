
# **Finding Lane Lines on the Road** 

## Writeup by Elffer


---

**Finding Lane Lines on the Road**

The goals / steps of this project are the following:
* Make a pipeline that finds lane lines on the road
* Reflect on your work in a written report


---

### Reflection

### 1. Describe your pipeline. As part of the description, explain how you modified the draw_lines() function.

From the highest level, I split the video into clips and images, and edit each of the image with an function, named process_image, which mask lane lines with red on each of the image.

The process_image function is a pipline consisted of 4 steps:
* get the gray version
* make it Gaussian blur, get the canny version to mark edges
* use mask to focus on the interested area, get the lines use hough transformation 
* get the lined image by combining the lines and the initial image.

In order to draw a single line on the left and right lanes, I modified the draw_lines() function by:
* calculate the slope by averating the slopes of all the shot segments;
* get the mean position of all the start and end positions of segments, from left lanes and right lanes, seperately.


### 2. Identify potential shortcomings with your current pipeline


One potential shortcoming would be what would happen when the car is not in the center of the road, as then the slope will be quite different the exsistent video.

Another shortcoming could be that some extra work are needed if apply these codes on the challenge video.


### 3. Suggest possible improvements to your pipeline

A possible improvement would be to sort the segments according to their x or y value to get better estimation of slopes for left lane and right lane, rather then using a tight constraint region to mask the lines.

Another potential improvement could be to use deep learning approaches to identify the road and mark out left and right lanes.

