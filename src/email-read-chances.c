#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function to simulate recruiter behavior based on subject, sender, and mood
int simulate_recruiter_behavior(char *subject, int sender_reputation,
                                int recruiter_mood) {
  // This is a simplified model, adjust weights and logic as needed
  int score = 0;

  // Subject line score (adjust weights as needed)
  if (strstr(subject, "Urgent") || strstr(subject, "Important")) {
    score += 20; // Higher score for urgent/important subjects
  } else if (strstr(subject, "Job Application") ||
             strstr(subject, "Position")) {
    score += 10; // Moderate score for standard application subjects
  }

  // Sender reputation score (adjust weights as needed)
  score += sender_reputation * 5; // Higher reputation increases score

  // Recruiter mood score (adjust weights as needed)
  score += recruiter_mood * 10; // Better mood increases score

  // Random factor to simulate unpredictable behavior
  score += rand() % 20; // Adds some randomness

  // Threshold for reading the email (adjust as needed)
  if (score >= 50) {
    return 1; // Email is read
  } else {
    return 0; // Email is not read
  }
}

int main() {
  // Seed the random number generator
  srand(time(NULL));

  int num_simulations = 1000000; // One million simulations
  int num_emails = 101;         // Total number of emails
  int position_sum = 0;

  // Your email details
  char subject = "Job Application for Software Engineer"; // Change as needed
  int sender_reputation = 7; // Scale of 1 to 10, adjust as needed

  // Simulate reading order many times
  for (int i = 0; i < num_simulations; i++) {
    int position = 0;
    int emails_read = 0;
    int recruiter_mood = rand() % 11; // Recruiter mood from 0 to 10

    // Simulate going through the emails
    for (int j = 0; j < num_emails; j++) {
      // Simulate recruiter behavior for EACH email
      int is_your_email = j == 0; // Check if it's your email
      int email_read = simulate_recruiter_behavior(
          is_your_email? subject: "Other Subject", // Use your subject or a
                                                    // generic one
          is_your_email? sender_reputation: 5,    // Use your reputation or an
                                                  // average one
          recruiter_mood); // Pass the recruiter mood

      if (email_read) {
        emails_read++;
        if (is_your_email) {
          position = emails_read;
        }
      }
    }

    // If your email was not read, assign it a position beyond the read emails
    if (position == 0) {
      position = emails_read + 1;
    }

    position_sum += position;
  }

  // Calculate average position
  double average_position = (double)position_sum / num_simulations;

  printf("Simulation Results:\n");
  printf("Average position: %.2f\n", average_position);

  return 0;
}
