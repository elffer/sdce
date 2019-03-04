/**
 * particle_filter.cpp
 *
 * Created on: Dec 12, 2016
 * Author: Tiffany Huang
 */

#include "particle_filter.h"

#include <math.h>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <string>
#include <vector>

#include "helper_functions.h"

using std::string;
using std::vector;
using std::normal_distribution;

void ParticleFilter::init(double x, double y, double theta, double std[]) {
  /**
   *   Set the number of particles. Initialize all particles to 
   *   first position (based on estimates of x, y, theta and their uncertainties
   *   from GPS) and all weights to 1. 
   */
  num_particles = 100;  // Set the number of particles

  // standard deviations for x, y and theta
  double std_x = std[0];
  double std_y = std[1];
  double std_theta = std[2];

  // create engine for generating random distribution
  std::default_random_engine gen;

  // create functions to generate normal (Gaussian) distribution
  normal_distribution<double> distf_x(x, std_x);
  normal_distribution<double> distf_y(y, std_y);
  normal_distribution<double> distf_theta(theta, std_theta);

  // initialize particles
  for(int i=0; i<num_particles; i++){
     Particle particle_i;
     particle_i.id = i;
     particle_i.x = distf_x(gen);
     particle_i.y = distf_y(gen);
     particle_i.theta = distf_theta(gen);
     particle_i.weight = 1;
  
     // add a particle to the particle vector
     particles.push_back(particle_i);
     }

  is_initialized = true;

}

void ParticleFilter::prediction(double delta_t, double std_pos[], 
                                double velocity, double yaw_rate) {
  /**
   * Add measurements to each particle and add random Gaussian noise.
   */

  // create engine for generating random distribution
  std::default_random_engine gen;

  normal_distribution<double> distf_x(0.0, std_pos[0]);
  normal_distribution<double> distf_y(0.0, std_pos[1]);
  normal_distribution<double> distf_theta(0.0, std_pos[2]);

  const double velocity_dt = velocity * delta_t;
  const double yaw_dt = yaw_rate * delta_t;
  const double vel_yaw = velocity / yaw_rate;

  for (int i = 0; i < num_particles; i++)
     {
                // predict the new state of particles, based on the velocity, yaw_rate and delta_t
		if (fabs(yaw_rate) < 0.001)
		{
			particles[i].x += velocity_dt * cos(particles[i].theta);
			particles[i].y += velocity_dt * sin(particles[i].theta);
		}
		else
		{
			const double theta_new = particles[i].theta + yaw_dt;
			particles[i].x += vel_yaw * (sin(theta_new) - sin(particles[i].theta));
			particles[i].y += vel_yaw * (cos(particles[i].theta) - cos(theta_new));
			particles[i].theta = theta_new;
		}
		// adding random Gaussian noise
		particles[i].x += distf_x(gen);
		particles[i].y += distf_y(gen);
		particles[i].theta += distf_theta(gen);
	}
}

void ParticleFilter::dataAssociation(vector<LandmarkObs> predicted, 
                                     vector<LandmarkObs>& observations) {
  /**
   *   Find the predicted measurement that is closest to each 
   *   observed measurement and assign the observed measurement to this 
   *   particular landmark.
   */

  // iterate through the observations to associate
  for (unsigned int i = 0; i < observations.size(); i++)
	{
		// set current observation
		LandmarkObs current_obs = observations[i];
                LandmarkObs current_pred;

		// initialize the  minimum distance to maximum possiblity
		double min_dist = 999999;
                double current_dist;
		int closest_landmark = 0;

		for (unsigned int j = 0; j < predicted.size(); j++)
		{
			current_pred = predicted[j];
			current_dist = dist(current_obs.x, current_obs.y, current_pred.x, current_pred.y);

			// find the predicted landmark nearest the current observed landmark
			if (current_dist < min_dist)
			{
				min_dist = current_dist;
				closest_landmark = current_pred.id;
			}
		}
		// set the observation's ID to the nearest predicted landmark's ID
		observations[i].id = closest_landmark;
	}


}

void ParticleFilter::updateWeights(double sensor_range, double std_landmark[], 
                                   const vector<LandmarkObs> &observations, 
                                   const Map &map_landmarks) {
  /**
   *   Update the weights of each particle using a mult-variate Gaussian 
   *   distribution. 
   */

  // iterate each of the particles to calculate its weight
  for (int i = 0; i < num_particles; i++)
	{
		double p_x = particles[i].x;
		double p_y = particles[i].y;
		double p_theta = particles[i].theta;

                // update to attach detectable ladmarks of each particle
		vector<LandmarkObs> predictions;

		for (unsigned int j = 0; j < map_landmarks.landmark_list.size(); j++)
		{
			// Get ID and x,y coordinates
			float l_x = map_landmarks.landmark_list[j].x_f;
			float l_y = map_landmarks.landmark_list[j].y_f;
			int l_id = map_landmarks.landmark_list[j].id_i;

			if (fabs(l_x - p_x) <= sensor_range && fabs(l_y - p_y) <= sensor_range)
			{
				predictions.push_back(LandmarkObs{l_id, l_x, l_y});
			}
		}

                // iterate through to transform observations to the map coordinate
		vector<LandmarkObs> transformed_obs;
		for (unsigned int j = 0; j < observations.size(); j++)
		{
			double transf_x = cos(p_theta) * observations[j].x - sin(p_theta) * observations[j].y + p_x;
			double transf_y = sin(p_theta) * observations[j].x + cos(p_theta) * observations[j].y + p_y;
			transformed_obs.push_back(LandmarkObs{observations[j].id, transf_x, transf_y});
		}

                // attach the nearest landmark to each of the transformed observations
		dataAssociation(predictions, transformed_obs);

                // for each of the transformed observation, calculate its multivariate-gaussian probability
                // then, update the total weight of the particle
		particles[i].weight = 1.0;
		for (unsigned int j = 0; j < transformed_obs.size(); j++)
		{
			double obs_x, obs_y, pred_x, pred_y;
			obs_x = transformed_obs[j].x;
			obs_y = transformed_obs[j].y;
			int associated_prediction = transformed_obs[j].id;

			for (unsigned int k = 0; k < predictions.size(); k++)
			{
				if (predictions[k].id == associated_prediction)
				{
					pred_x = predictions[k].x;
					pred_y = predictions[k].y;
				}
			}

			double std_x = std_landmark[0];
			double std_y = std_landmark[1];
			double obs_weight = (1 / (2 * M_PI * std_x * std_y)) * exp(-(pow(pred_x - obs_x, 2) / (2 * pow(std_x, 2)) + (pow(pred_y - obs_y, 2) / (2 * pow(std_y, 2)))));

			particles[i].weight *= obs_weight;
		}
	}


}

void ParticleFilter::resample() {
  /**
   *   Resample particles with replacement with probability proportional 
   *   to their weight. 
   */

  vector<double> weights;
  for (int x = 0; x < num_particles; x++){
     weights.push_back(particles[x].weight);
     }


  // vector to store new particles
  vector<Particle> new_particles(num_particles);

  // use discrete distribution to fetch particles by weight
  std::random_device rand_dev;
  
  // create engine for generating random distribution
  std::default_random_engine gen;
  for(int i = 0; i < num_particles; i++){
     std::discrete_distribution<int> id_pdf(weights.begin(), weights.end());
     new_particles[i] = particles[id_pdf(gen)];
     }

  particles = new_particles;

}

void ParticleFilter::SetAssociations(Particle& particle, 
                                     const vector<int>& associations, 
                                     const vector<double>& sense_x, 
                                     const vector<double>& sense_y) {
  // particle: the particle to which assign each listed association, 
  //   and association's (x,y) world coordinates mapping
  // associations: The landmark id that goes along with each listed association
  // sense_x: the associations x mapping already converted to world coordinates
  // sense_y: the associations y mapping already converted to world coordinates
  particle.associations= associations;
  particle.sense_x = sense_x;
  particle.sense_y = sense_y;
}

string ParticleFilter::getAssociations(Particle best) {
  vector<int> v = best.associations;
  std::stringstream ss;
  copy(v.begin(), v.end(), std::ostream_iterator<int>(ss, " "));
  string s = ss.str();
  s = s.substr(0, s.length()-1);  // get rid of the trailing space
  return s;
}

string ParticleFilter::getSenseCoord(Particle best, string coord) {
  vector<double> v;

  if (coord == "X") {
    v = best.sense_x;
  } else {
    v = best.sense_y;
  }

  std::stringstream ss;
  copy(v.begin(), v.end(), std::ostream_iterator<float>(ss, " "));
  string s = ss.str();
  s = s.substr(0, s.length()-1);  // get rid of the trailing space
  return s;
}
