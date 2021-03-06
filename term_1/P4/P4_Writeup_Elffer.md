
## Writeup Template


---

**Advanced Lane Finding Project**

The goals / steps of this project are the following:

* Compute the camera calibration matrix and distortion coefficients given a set of chessboard images.
* Apply a distortion correction to raw images.
* Use color transforms, gradients, etc., to create a thresholded binary image.
* Apply a perspective transform to rectify binary image ("birds-eye view").
* Detect lane pixels and fit to find the lane boundary.
* Determine the curvature of the lane and vehicle position with respect to center.
* Warp the detected lane boundaries back onto the original image.
* Output visual display of the lane boundaries and numerical estimation of lane curvature and vehicle position.

[//]: # (Image References)

[image01]: ./output_images/original_chessbord.png "Original Chessboard Image"
[image02]: ./output_images/undistorted_chessbord.png "Undistorted Chessboard Image"
[image03]: ./output_images/test6.jpg "Original Image"
[image031]: ./output_images/undistorted_test6.png "Undistorted Image"
[image04]: ./output_images/combined_binary_image.png "Combined Binary Image"
[image05]: ./output_images/combined_binary_image_2.png "Combined Binary Image"
[image06]: ./output_images/warped_image.png "Warped Image"
[image07]: ./output_images/warped_image_1.png "Warped Image"
[image08]: ./output_images/lane_pixels.png "Warped Image"

[image09]: ./output_images/test1.jpg "Test 1"
[image10]: ./output_images/test1_processed.png "Test 1"
[image11]: ./output_images/test6.jpg "Test 1"
[image12]: ./output_images/test6_processed.png "Test 6"

[image13]: ./output_images/bug_1.png "Bug 1"
[image14]: ./output_images/bug_2.png "Bug 2"
[image15]: ./output_images/bug_2_pixels.png "Pixels of Bug 2"
[image16]: ./output_images/bug_2_pixels_enhanced.png "Refined Pixels of Bug 2"

[video1]: ./output_video/submission.mp4 "Video"

[image2]: ./test_images/test1.jpg "Road Transformed"
[image3]: ./examples/binary_combo_example.jpg "Binary Example"
[image4]: ./examples/warped_straight_lines.jpg "Warp Example"
[image5]: ./examples/color_fit_lines.jpg "Fit Visual"
[image6]: ./examples/example_output.jpg "Output"


## [Rubric](https://review.udacity.com/#!/rubrics/571/view) Points

### Here I will consider the rubric points individually and describe how I addressed each point in my implementation.  

---

### Writeup / README

#### 1. Provide a Writeup / README that includes all the rubric points and how you addressed each one.  You can submit your writeup as markdown or pdf.  [Here](https://github.com/udacity/CarND-Advanced-Lane-Lines/blob/master/writeup_template.md) is a template writeup for this project you can use as a guide and a starting point.  

You're reading it!

### Camera Calibration

#### 1. Briefly state how you computed the camera matrix and distortion coefficients. Provide an example of a distortion corrected calibration image.

The code for this step is contained in the code cell of the IPython notebook located in lines 15 through 41 of the file called `P4_Advanced_Lane_Finding_Elffer.ipynb`.  

I start by preparing "object points", which will be the (x, y, z) coordinates of the chessboard corners in the world. Here I am assuming the chessboard is fixed on the (x, y) plane at z=0, such that the object points are the same for each calibration image.  Thus, `objp` is just a replicated array of coordinates, and `objpoints` will be appended with a copy of it every time I successfully detect all chessboard corners in a test image.  `imgpoints` will be appended with the (x, y) pixel position of each of the corners in the image plane with each successful chessboard detection.  

I then used the output `objpoints` and `imgpoints` to compute the camera calibration and distortion coefficients using the `cv2.calibrateCamera()` function.  I applied this distortion correction to the test image using the `cv2.undistort()` function. The original and undistorted images are presented as follows.

Original Chessboard Image:
![Original Chessboard Image][image01]

Undistorted Chessboard Image:
![Undistorted Chessboard Image][image02]

### Pipeline (single images)

#### 1. Provide an example of a distortion-corrected image.

To demonstrate this step, I will present an example of the distortion correction to one of the test images, i.e., test6.jpg:

Original test6.jpg
![Original Image][image03]

Undistorted test6.jpg
![Original Image][image031]


#### 2. Describe how (and identify where in your code) you used color transforms, gradients or other methods to create a thresholded binary image.  Provide an example of a binary image result.

I use a combination of color and gradient thresholds to generate a binary image (thresholding steps at lines 45 through 85 in `P4_Advanced_Lane_Finding_Elffer.ipynb`). 

Firstly, I fetch the R channel directly from the array of the image as shown in lines 49 through 52, with `R = image[:, :, 0]`. None zero pixels in the R channel are reserved if they meet thresholds.

Then sobel operaters are applied to detect edges. Considering the requirements of this project, the gradients along x direction provide much information to detect the lane-lines. Sobel operater outperforms canny detector in this project as it is more efficient. The gradients along the x direction were extracted in lines 55 through 62 with the function of `cv2.Sobel(gray, cv2.CV_64F, 1, 0)` in the file of `P4_Advanced_Lane_Finding_Elffer.ipynb`.  

The third approach appield to filter the pixels is decomposing the image in the space of HLS with the function of `cv2.cvtColor(img, cv2.COLOR_RGB2HLS)`, as shown in lines 65 through 73 in `P4_Advanced_Lane_Finding_Elffer.ipynb`.

All these thresholds were later combined to fetch the effective none zero pixels in an image. Here's an example of my output from this step, with a comparison of the original image and thresholded binary image given as follows,

![Combined Binary Image][image04]

And here is another image for comparison given by,

![Another Combined Binary Image][image05]

#### 3. Describe how (and identify where in your code) you performed a perspective transform and provide an example of a transformed image.

The code for my perspective transform includes a function called `warper()`, which appears in lines 89 through 113 in the file `P4_Advanced_Lane_Finding_Elffer.ipynb`. The `warper()` function takes in the inputs including an image (`img`), as well as source (`src`) and destination (`dst`) points. The binary image is then transformed with the function of `cv2.getPerspectiveTransform(src, dst)`. I chose the hardcode the source and destination points in the following manner:

```python
    src = np.float32(
        [[(img_size[0] / 2) - 63, img_size[1] / 2 + 100],
         [((img_size[0] / 6) - 10), img_size[1]],
         [(img_size[0] * 5 / 6) + 60, img_size[1]],
         [(img_size[0] / 2 + 63), img_size[1] / 2 + 100]])
    dst = np.float32(
        [[(img_size[0] / 4 - 200), 0],
         [(img_size[0] / 4 - 200), img_size[1]],
         [(img_size[0] * 3 / 4 + 10), img_size[1]],
         [(img_size[0] * 3 / 4 + 10), 0]])
```

This resulted in the following source and destination points:

| Source        | Destination   | 
|:-------------:|:-------------:| 
| 577, 460      | 120, 0        | 
| 203, 720      | 120, 720      |
| 1127, 720     | 970, 720      |
| 703, 460      | 970, 0        |

I verified that my perspective transform was working as expected by drawing the `src` and `dst` points onto a test image and its warped counterpart to verify that the lines appear parallel in the warped image.
* A comparison of the original image and the warped image is given as follows,
![Warped Image][image07]

* A comparison of the combined binary image and the warped image is given as follows,

![Warped Image][image06]

#### 4. Describe how (and identify where in your code) you identified lane-line pixels and fit their positions with a polynomial?

Then I did some other stuff and fit my lane lines with a 2nd order polynomial like this:

![alt text][image08]

The lane-line pixels are identified with two approaches, i.e., searching with histogram of pixels and searching pixels around the polynome found in the prior frame. The general strategy for the use of these two methods are given as follows.

* If the polynomial of the last frame does not exist or the number of pixels nearby the polynomial does not meet the lower bound requirement, such as at the start of the video, the process will search peaks in histogram with sliding windows to find lane lines.

* Otherwise, the previous polynomials can be used to search for none zero pixels nearby, which will be used to fit and update the polynomials.

Then the none zero pixels identified as pixels of the lane-lines will be applied with the function of `np.polyfit()` to get the polynomials.

The codes are implemented in lines 117 through 260 in the file of `P4_Advanced_Lane_Finding_Elffer.ipynb`.

#### 5. Describe how (and identify where in your code) you calculated the radius of curvature of the lane and the position of the vehicle with respect to center.

I did this in lines 265 through 295 in my code in `P4_Advanced_Lane_Finding_Elffer.ipynb`. The values for the radius of curvature of the lane and the distance of the vehicle with respect to lane center, are calculated and converted from pixels space to meters. The radius of curvature is calculated with the followring equation, as introduced in the class,
$$ R_{curve} = \frac{(1+(2Ay+B)^2)^{\frac{3}{2}}}{|2A|}. $$

The distance from the center of the vehicle to the center of the road, is equal to the distance from the center of the image to the center of the road, assuming that the camera was mounted in the center of the vehicle.

The radius of curvature of the lane and the position of the vehicle are ploted on each frame of the video.

#### 6. Provide an example image of your result plotted back down onto the road such that the lane area is identified clearly.

I implemented this step in lines 299 through 321 in my code in `P4_Advanced_Lane_Finding_Elffer.ipynb` in the function `draw_area()`.  Here are the examples of my results on two test images.

Original photo of test1.jpg:
![alt text][image09]

Result of test1.jpg:
![alt text][image10]

Original photo of test6.jpg:
![alt text][image11]

Result of test6.jpg:
![alt text][image12]

---

### Pipeline (video)

#### 1. Provide a link to your final video output.  Your pipeline should perform reasonably well on the entire project video (wobbly lines are ok but no catastrophic failures that would cause the car to drive off the road!).

Here's a [link to my video result](./output_video/submission.mp4).

The submitted pipeline performs quite well on the entire project video. Some minor wobble exist while the pipeline can track the lanes precisely, although in several frames the left or right polynomial deviate from the true lines for a little bit(less than 0.3 meters).

---

### Discussion

#### 1. Briefly discuss any problems / issues you faced in your implementation of this project.  Where will your pipeline likely fail?  What could you do to make it more robust?

Here I'll talk about the approach I took, what techniques I used, what worked and why, where the pipeline might fail and how I might improve it if I were going to pursue this project further.

The pipeline were challeged by three sorts of issues.
* In many cases, the detected polynomial deviate from the lane-line, some times larger than 0.4 meters.
* The function sometimes take other lines on the road as lane-lines, shown as follows,
![alt text][image13]
* When the vehicle passby the shadow of a tree, the detected polynomial wobbled with a very large curvature. The pictures below show where and how the issue was identified.
![alt text][image14]
![alt text][image15]

All these troubles took place in the steps of threshold treatment and lane pixel finding.
* For the first problem, I tuned the margine and threshold of the color channels, and fixed most of the scenarios.
* To deal with the second problem, I combined color channel threshold with finding pixles nearby prior polynomial, in the lines 49 through 83 in `P4_Advanced_Lane_Finding_Elffer.ipynb`. By applying this strategy, the combined binary image was enhanced and presented as follows. 
![Enhanced with extra color channel][image16]
* In dealing with the third trouble, I found that the shadow of the tree does not exist in the red channel. As a result, the red channel was combined to filter the shadows.

Up to now, the pipeline works well with the `project_video.mp4`. However, there needs more improvement to apply this pipeline to the videos of `challenge_video.mp4` and `harder_challenge_video.mp4`. Some possible approaches include tuning the thresholds, selection of interest area with mask and selection of idea channels such as using YUV or HSV. This pipeline would be likely to fail if the curvature is too large, i.e. when the raidus of curvature is small.
