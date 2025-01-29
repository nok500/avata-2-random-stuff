#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

double randn(double mu, double sigma) {
  double U1, U2, W, mult;
  static double X1, X2;
  static int call = 0;

  if (call == 1) {
    call =!call;
    return (mu + sigma * (double) X2);
  }

  do {
    U1 = -1 + ((double) rand() / RAND_MAX) * 2;
    U2 = -1 + ((double) rand() / RAND_MAX) * 2;
    W = pow(U1, 2) + pow(U2, 2);
  } while (W >= 1 || W == 0);

  mult = sqrt((-2 * log(W)) / W);
  X1 = U1 * mult;
  X2 = U2 * mult;

  call =!call;

  return (mu + sigma * (double) X1);
}

int main() {
  srand(time(NULL));

// avata 2 specifications - didn't check the website for specifics, just assumed
  double battery_capacity = 4000;  // mAh
  double drone_mass = 0.5;  // kg - ish, with guards on and an ND filter
  double motor_efficiency = 0.8; // crashed the drone way too many times, propelers are kind worn out, so .7 - .8 seems about right
  double max_flight_time_no_wind = 33;  // according to DJI - that is you fly this thing in a lab maybe, in reality it's half that

// wind
  int num_wind_speeds = 50;
  double wind_speed[num_wind_speeds];
  for (int i = 0; i < num_wind_speeds; i++) {
    wind_speed[i] = i * 6.4 / (num_wind_speeds - 1); // usually 14-15mph winds in my area
  }
  double wind_direction = 0;  // worst case, headwind :/
 
// diesel tractors joined the chat
  double exhaust_density = 1.2;
  double exhaust_concentration = 0.1;
  double exhaust_dispersion_rate = 0.05;
  
  double air_density_with_exhaust[num_wind_speeds];
  for (int i = 0; i < num_wind_speeds; i++) {
    double current_concentration = exhaust_concentration * exp(-exhaust_dispersion_rate * wind_speed[i]);
    air_density_with_exhaust[i] = (1 - current_concentration) * 1.225 + current_concentration * exhaust_density;
  }

// PWR consumption w/ headwind
  double air_density = 1.293;  // assume pure air density, some country-side farming field might get close to this
  double drone_frontal_area = 0.1;
  double drag_coefficient = 0.5;
  double power_wind[num_wind_speeds];
  for (int i = 0; i < num_wind_speeds; i++) {
    power_wind[i] = 0.5 * air_density * drone_frontal_area * drag_coefficient * pow(wind_speed[i], 3);
  }

// ND filter
  double filter_protrusion = 0.001;
  double original_frontal_area = 0.1;
  double new_frontal_area = original_frontal_area + (4 * filter_protrusion * 0.1);
  double power_hover = (drone_mass * 9.81) / motor_efficiency;  // pwr to hover in no wind
  double power_total[num_wind_speeds];
  for (int i = 0; i < num_wind_speeds; i++) {
    power_total[i] = power_hover + power_wind[i];
  }


// flight time
  double flight_time_min[num_wind_speeds];
  for (int i = 0; i < num_wind_speeds; i++) {
    flight_time_min[i] = (battery_capacity * 60) / (power_total[i] * 1000 / 60);
  }

// Uncertainty quantification
// Assume uncertainty in wind speed measurement
  double wind_speed_uncertainty = 1;  // m/s standard deviation
  int num_samples = 1000;
  double flight_time_samples[num_samples][num_wind_speeds];
  double flight_time_mean[num_wind_speeds]; // No initialization here
  double flight_time_std[num_wind_speeds];  // No initialization here

  // Initialize the arrays to 0
  for (int i = 0; i < num_wind_speeds; i++) {
    flight_time_mean[i] = 0;
    flight_time_std[i] = 0;
  }

  for (int i = 0; i < num_wind_speeds; i++) {
    for (int j = 0; j < num_samples; j++) {
      double wind_speed_sample = randn(wind_speed[i], wind_speed_uncertainty);
      flight_time_samples[j][i] = (battery_capacity * 60) / (
          (power_hover + 0.5 * air_density_with_exhaust[i] * new_frontal_area * 0.5 * pow(wind_speed_sample, 3)) * 1000 / 60
      );
      flight_time_mean[i] += flight_time_samples[j][i];
    }
    flight_time_mean[i] /= num_samples;

    for (int j = 0; j < num_samples; j++) {
      flight_time_std[i] += pow(flight_time_samples[j][i] - flight_time_mean[i], 2);
    }
    flight_time_std[i] = sqrt(flight_time_std[i] / num_samples);
  }

  // Print some results
  printf("Maximum flight time in no wind: %.2f minutes\n", max_flight_time_no_wind);
  for (int i = 0; i < num_wind_speeds; i += 5) {
    printf("Flight time at %.1f m/s wind speed: %.2f +/- %.2f minutes\n", wind_speed[i], flight_time_mean[i], flight_time_std[i]);
  }

  // Plotting - You'll need a library like gnuplot to do this in C
  // This code generates data files that can be plotted with gnuplot
  FILE *gnuplot_data = fopen("flight_time_data.txt", "w");
  for (int i = 0; i < num_wind_speeds; i++) {
    fprintf(gnuplot_data, "%f %f %f\n", wind_speed[i], flight_time_mean[i], flight_time_std[i]);
  }
  fclose(gnuplot_data);

  // System call to gnuplot to generate the plot
  system("gnuplot -persist -e \"set title 'DJI Drone Flight Time vs. Wind Speed with Uncertainty'; set xlabel 'Wind Speed (m/s)'; set ylabel 'Flight Time (minutes)'; plot 'flight_time_data.txt' using 1:2 with lines title 'Mean Flight Time', 'flight_time_data.txt' using 1:2:3 with yerrorbars title 'Uncertainty'\"");

  return 0;
}
