import csv
import cv2
import numpy as np
import tensorflow as tf
from sklearn.model_selection import train_test_split
from sklearn.utils import shuffle

# 1. Import the data got from training mode (only build up a table for generators)
lines = []
drive_log_1 = 'data/elffer_cycling_data/group1/driving_log.csv'
with open(drive_log_1) as csvfile:
    reader = csv.reader(csvfile)
    for line in reader:
        lines.append(line)


# 2. Split the recorded data into training and validation sets
train_samples, validation_samples = train_test_split(lines, test_size=0.2)


# 3. Create a generator to fectch a small fixed number of figures for training
#    Notes: All the figures from left, center and right cameras are imported for training.
#           Steering from left and right cameras are tuned and corrected.      
def generator(samples, img_path, batch_size=32):
    num_samples = len(samples)
    while 1: # Loop forever so the generator never terminates
        samples = shuffle(samples)
        for offset in range(0, num_samples, batch_size):
            batch_samples = samples[offset:(offset+batch_size)]
            
            images = []
            angles = []
            for batch_sample in batch_samples:
                
                image_name_list = [img_path + 'IMG/' + batch_sample[i].split('/')[-1] for i in range(3)]
                img_center = cv2.imread(image_name_list[0])
                img_left = cv2.imread(image_name_list[1])
                img_right = cv2.imread(image_name_list[2])
                images.extend([img_center, img_left, img_right])
                
                steering_center = float(batch_sample[3])
                correction = 0.1
                steering_left = steering_center + correction
                steering_right = steering_center - correction
                angles.extend([steering_center, steering_left, steering_right])

            X_train = np.reshape(np.array(images), (-1, 160, 320, 3))
            y_train = np.reshape(np.array(angles), (-1, 1))
            yield shuffle(X_train, y_train)    
    
    
# 4. Set train and validation sets based on the generator
current_path = 'data/elffer_cycling_data/group1/'
train_generator = generator(train_samples, img_path=current_path, batch_size=4)
validation_generator = generator(validation_samples, img_path=current_path, batch_size=4)


from keras.models import Sequential
from keras.layers import Flatten, Dense, Lambda, Convolution2D, MaxPooling2D, Cropping2D, Dropout
import matplotlib.pyplot as plt
%matplotlib inline

# 5. Build the model (enhanced version of LeNet)
# Notes: This model follows the design of Nvidia team as suggested by the class
#        The tricks applied in this model includes:
#        a. Normalization of the figures
#        b. Cropping unrelated parts of the images
#        c. Added dropout layers to overcome the problem of overfitting
def LeNet():
    model = Sequential()
    model.add(Lambda(lambda x: x / 255.0 - 0.5, input_shape=(160, 320, 3)))
    model.add(Cropping2D(cropping=((70, 25), (0, 0))))
    model.add(Convolution2D(24, (5, 5), strides=(2, 2), activation='relu'))
    model.add(Convolution2D(36, (5, 5), strides=(2, 2), activation='relu'))
    model.add(Convolution2D(48, (5, 5), strides=(2, 2), activation='relu'))
    model.add(Dropout(0.5))
    model.add(Convolution2D(64, (3, 3), activation='relu'))
    model.add(Convolution2D(64, (3, 3), activation='relu'))
    model.add(Flatten())
    model.add(Dense(100))
    model.add(Dropout(0.5))
    model.add(Dense(50))
    model.add(Dense(10))
    model.add(Dense(1))
    return model


lenet_model = LeNet()
print(lenet_model.summary())


# 6. Fit and train the model
lenet_model.compile(loss='mse', optimizer='adam')
history_object = lenet_model.fit_generator(train_generator, steps_per_epoch=
                                          len(train_samples), validation_data=validation_generator,
                                          validation_steps = len(validation_samples), epochs=3)


# 7. Save the model
lenet_model.save('model.h5')


# 8. plot the training and validation loss for each epoch
### print the keys contained in the history object
print(history_object.history.keys())
plt.plot(history_object.history['loss'])
plt.plot(history_object.history['val_loss'])
plt.title('model mean squared error loss')
plt.ylabel('mean squared error loss')
plt.xlabel('epoch')
plt.legend(['training set', 'validation set'], loc='upper right')
plt.show()

