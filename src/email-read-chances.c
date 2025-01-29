#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int simulate_recruiter_behavior(char *subject, int sender_reputation,
                                int recruiter_mood) {
  int score = 0;

  if (strstr(subject, "Urgent") || strstr(subject, "Important") || strstr(subject, "Quality and Performance Automation Engineer")) {
    score += 20; // Higher score for urgent/important subjects
  } else if (strstr(subject, "Job Application") ||
             strstr(subject, "Position")) {
    score += 10;
  }

  score += sender_reputation * 5; // gmail reputation should be good enough
  score += recruiter_mood * 10; 
  score += rand() % 20; // randomness

// Threshold
  if (score >= 50) {
    return 1; // read
  } else {
    return 0; // not read
  }
}

int main() {
  srand(time(NULL));

  int num_simulations = 1000000; // one-in-a-million simulation
  int num_emails = 123;         // total emails
  int position_sum = 0;

// Your email details
  char subject = "Quality and Performance Automation Engineer (GB)"; // Change as needed
  int sender_reputation = 5; // middleground

// Simulate reading order many times
  for (int i = 0; i < num_simulations; i++) {
    int position = 0;
    int emails_read = 0;
    int recruiter_mood = rand() % 11;

    for (int j = 0; j < num_emails; j++) {
// recruiter behavior for EACH email
      int is_your_email = j == 0; // Check if it's your email
      int email_read = simulate_recruiter_behavior(
          is_your_email? subject: "Other Subject",
          is_your_email? sender_reputation: 5,
          recruiter_mood
      );
      if (email_read) {
        emails_read++;
        if (is_your_email) {
          position = emails_read;
        }
      }
    }

// not read, assign it a position beyond the read emails
    if (position == 0) {
      position = emails_read + 1;
    }
    position_sum += position;
  }

// average position
  double average_position = (double)position_sum / num_simulations;
  printf("Simulation Results:\n");
  printf("Average position: %.2f\n", average_position);
  return 0;
}
