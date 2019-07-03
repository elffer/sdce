#!/usr/bin/env python3
import os.path
import tensorflow as tf
import helper
import warnings
from distutils.version import LooseVersion
import project_tests as tests


# Check TensorFlow Version
assert LooseVersion(tf.__version__) >= LooseVersion('1.0'), 'Please use TensorFlow version 1.0 or newer.  You are using {}'.format(tf.__version__)
print('TensorFlow Version: {}'.format(tf.__version__))

# Check for a GPU
if not tf.test.gpu_device_name():
    warnings.warn('No GPU found. Please use a GPU to train your neural network.')
else:
    print('Default GPU Device: {}'.format(tf.test.gpu_device_name()))


def load_vgg(sess, vgg_path):
    """
    Load Pretrained VGG Model into TensorFlow.
    :param sess: TensorFlow Session
    :param vgg_path: Path to vgg folder, containing "variables/" and "saved_model.pb"
    :return: Tuple of Tensors from VGG model (image_input, keep_prob, layer3_out, layer4_out, layer7_out)
    """

    #   Use tf.saved_model.loader.load to load the model and weights

    # tensor names in the stored weights
    vgg_tag = 'vgg16'
    vgg_input_tensor_name = 'image_input:0'
    vgg_keep_prob_tensor_name = 'keep_prob:0'
    vgg_layer3_out_tensor_name = 'layer3_out:0'
    vgg_layer4_out_tensor_name = 'layer4_out:0'
    vgg_layer7_out_tensor_name = 'layer7_out:0'

    # 1. load model
    tf.saved_model.loader.load(sess, [vgg_tag], vgg_path)

    # 2. get the tensors by names
    graph = tf.get_default_graph()
    image_input = graph.get_tensor_by_name(vgg_input_tensor_name)
    keep_prob = graph.get_tensor_by_name(vgg_keep_prob_tensor_name)
    vgg_layer3_out = graph.get_tensor_by_name(vgg_layer3_out_tensor_name)
    vgg_layer4_out = graph.get_tensor_by_name(vgg_layer4_out_tensor_name)
    vgg_layer7_out = graph.get_tensor_by_name(vgg_layer7_out_tensor_name)
    
    return image_input, keep_prob, vgg_layer3_out, vgg_layer4_out, vgg_layer7_out


tests.test_load_vgg(load_vgg, tf)


def layers(vgg_layer3_out, vgg_layer4_out, vgg_layer7_out, num_classes):
    """
    Create the layers for a fully convolutional network.  Build skip-layers using the vgg layers.
    :param vgg_layer3_out: TF Tensor for VGG Layer 3 output
    :param vgg_layer4_out: TF Tensor for VGG Layer 4 output
    :param vgg_layer7_out: TF Tensor for VGG Layer 7 output
    :param num_classes: Number of classes to classify
    :return: The Tensor for the last layer of output
    """
    # 1. skip 1: conv2d_transpose(conv2d(layer7)) + conv2d(layer4)

    # 1.1 1x1 convolution of vgg layer 7: convert fully connected layer to convolution layer
    #     size: 7x7x6
    layer7_conv = tf.layers.conv2d(vgg_layer7_out, filters=num_classes,
                                   kernel_size=1, padding='same',
                                   kernel_initializer=tf.random_normal_initializer(stddev=0.01),
                                   kernel_regularizer=tf.contrib.layers.l2_regularizer(1e-3), name="layer7_conv")
    
    # 1.2 upsample the convolution layer layer7_conv with size depth=4096 to match size of layer 4
    #     so that we can add skip connection with 4th pooling layer
    #     size: 14x14x6
    deconv1 = tf.layers.conv2d_transpose(layer7_conv, filters=num_classes,
                                         kernel_size=4, strides=(2, 2), padding='same',
                                         kernel_initializer=tf.random_normal_initializer(stddev=0.01),
                                         kernel_regularizer=tf.contrib.layers.l2_regularizer(1e-3), name="deconv1")

    # 1.3 1x1 convolution of vgg layer 4
    #     size: 14x14
    layer4_conv = tf.layers.conv2d(vgg_layer4_out, filters=num_classes,
                                   kernel_size=1, padding='same',
                                   kernel_initializer=tf.random_normal_initializer(stddev=0.01),
                                   kernel_regularizer=tf.contrib.layers.l2_regularizer(1e-3), name="layer4_conv")

    # 1.4 generate the 1st skip layer by add the transformed layer 7 and transformed layer 4
    #     size: 14x14
    pool4_skip = tf.add(deconv1, layer4_conv, name="pool4_skip")

    # 2. skip 2: conv2d_transpose(pool4_skip) + conv2d(layer3)

    # 2.1 Upsample to match size of pool3
    #     size: 28x28x6
    deconv2 = tf.layers.conv2d_transpose(pool4_skip, filters=num_classes, kernel_size=4, strides=(2, 2), padding='same',
                                         kernel_initializer=tf.random_normal_initializer(stddev=0.01),
                                         kernel_regularizer=tf.contrib.layers.l2_regularizer(1e-3),
                                         name="deconv2")

    # 2.2 1x1 convolution of vgg layer 3
    #     size: 28x28
    layer3_conv = tf.layers.conv2d(vgg_layer3_out, num_classes, 1, padding='same',
                                   kernel_initializer=tf.random_normal_initializer(stddev=0.01),
                                   kernel_regularizer=tf.contrib.layers.l2_regularizer(1e-3),
                                   name="layer3_conv")

    # 2.3 generate the 2nd skip layer by add the 1st skip layer and the transformed layer 3
    #     size: 28x28
    pool3_skip = tf.add(deconv2, layer3_conv, name="fcn10_plus_pool3")

    # 3. Upsample to match the size of the input image
    #    size: 224x224x6
    deconv3 = tf.layers.conv2d_transpose(pool3_skip, num_classes,
                                         kernel_size=16, strides=(8, 8), padding='same',
                                         kernel_initializer=tf.random_normal_initializer(stddev=0.01),
                                         kernel_regularizer=tf.contrib.layers.l2_regularizer(1e-3),
                                         name="fcn11")

    return deconv3


tests.test_layers(layers)


def optimize(nn_last_layer, correct_label, learning_rate, num_classes):
    """
    Build the TensorFLow loss and optimizer operations.
    :param nn_last_layer: TF Tensor of the last layer in the neural network
    :param correct_label: TF Placeholder for the correct label image
    :param learning_rate: TF Placeholder for the learning rate
    :param num_classes: Number of classes to classify
    :return: Tuple of (logits, train_op, cross_entropy_loss)
    """

    # 1. convert the 4D tensor to 2D: the value on each position of the 2D matrix stands for the class of that pixel
    #    logits is the output of nn_last_layer that output the log(i/Ni)
    logits = tf.reshape(nn_last_layer, (-1, num_classes), name="logits")
    correct_label_reshaped = tf.reshape(correct_label, (-1, num_classes))

    # 2. Calculate metric using cross entropy
    #    use the logits of nn_layer_output and ground truth to calculate the cross_entropy_loss
    cross_entropy_loss = tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits(logits=logits, labels=correct_label_reshaped), name="loss")
    # regularization_losses = tf.losses.get_regularization_loss()
    # loss = cross_entropy_loss + regularization_losses
    
    # 3. set the adam optimizer
    train_op = tf.train.AdamOptimizer(learning_rate=learning_rate).minimize(cross_entropy_loss, name="train_op")
    return logits, train_op, cross_entropy_loss


tests.test_optimize(optimize)


def train_nn(sess, epochs, batch_size, get_batches_fn, train_op, cross_entropy_loss, input_image, correct_label,
             keep_prob, learning_rate):
    """
    Train neural network and print out the loss during training.
    :param sess: TF Session
    :param epochs: Number of epochs
    :param batch_size: Batch size
    :param get_batches_fn: Function to get batches of training data.  Call using get_batches_fn(batch_size)
    :param train_op: TF Operation to train the neural network
    :param cross_entropy_loss: TF Tensor for the amount of loss
    :param input_image: TF Placeholder for input images
    :param correct_label: TF Placeholder for label images
    :param keep_prob: TF Placeholder for dropout keep probability
    :param learning_rate: TF Placeholder for learning rate
    """
    
    keep_prob_value = 0.5
    learning_rate_value = 0.001

    for epoch in range(epochs):
        print("EPOCH {} ...".format(epoch + 1))
        total_loss = 0
        for image_batch, label_batch in get_batches_fn(batch_size):
            # Training
            loss, _ = sess.run([cross_entropy_loss, train_op],
                               feed_dict={input_image: image_batch, correct_label: label_batch,
                                          keep_prob: keep_prob_value, learning_rate: learning_rate_value})
            total_loss += loss
            print("Loss = {:.3f}".format(loss))


tests.test_train_nn(train_nn)


def run():
    num_classes = 2
    image_shape = (160, 576)  # KITTI dataset uses 160x576 images
    data_dir = './data'
    runs_dir = './runs'       # saves the output image
    EPOCHS = 20
    BATCH_SIZE = 10
    tests.test_for_kitti_dataset(data_dir)

    # 1. Download pre-trained vgg model
    helper.maybe_download_pretrained_vgg(data_dir)

    # OPTIONAL: Train and Inference on the Cityscapes data set instead of the Kitti dataset.
    # You'll need a GPU with at least 10 teraFLOPS to train on.
    #  https://www.cityscapes-dataset.com/

    # 2. setup placeholder tensors
    correct_label = tf.placeholder(tf.float32, [None, image_shape[0], image_shape[1], num_classes])
    learning_rate = tf.placeholder(tf.float32)

    with tf.Session() as sess:
        # 3. Create function to get batches of images
        # Path to vgg model
        vgg_path = os.path.join(data_dir, 'vgg')
        get_batches_fn = helper.gen_batch_function(os.path.join(data_dir, 'data_road/training'), image_shape)

        # OPTIONAL: Augment Images for better results
        #  https://datascience.stackexchange.com/questions/5224/how-to-prepare-augment-images-for-neural-network

        # 4. Build NN using load_vgg, layers, and optimize function
        input_image, keep_prob, layer3_out, layer4_out, layer7_out = load_vgg(sess, vgg_path)
        final_layer_output = layers(layer3_out, layer4_out, layer7_out, num_classes)
        
        # 5. Set the optimizer
        # correct_label, learning_rate are place holders
        logits, train_op, cross_entropy_loss = optimize(final_layer_output, correct_label, learning_rate, num_classes)

        # 6. Initialize all variables
        sess.run(tf.global_variables_initializer())
        sess.run(tf.local_variables_initializer())

        print("Model build successful, starting training...")

        # 7. Train the neural network
        train_nn(sess, EPOCHS, BATCH_SIZE, get_batches_fn, train_op, cross_entropy_loss, input_image, correct_label, keep_prob, learning_rate)

        # # 8. Save the trained session
        # saver = tf.train.Saver()
        # saver.save(sess, 'save_model/fcn8')
        # print("Model has been saved.")

        # 9. Save inference data using helper.save_inference_samples
        helper.save_inference_samples(runs_dir, data_dir, sess, image_shape, logits, keep_prob, input_image)

        # OPTIONAL: 10. Apply the trained model to a video


if __name__ == '__main__':
    helper.maybe_download_pretrained_vgg('data/')
    run()

    # 11.
    # with tf.Session() as sess:
    #     saver = tf.train.Saver()
    #     saver.restore(sess, tf.train.latest_checkpoint('save_model/'))
    #     print('Model has been restored. ')
