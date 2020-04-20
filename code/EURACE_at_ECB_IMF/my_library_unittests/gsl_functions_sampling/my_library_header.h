/**
 * \file  my_library_header.h
 * \brief Header for user created library functions.
 */

#define EPSILON 1e-16 //Accuracy for doubles close to zero, eg to prevent division by zero
 

int random_int(int min,int max);
double random_unif();
double random_unif_interval(double a, double b);
double max(double a, double b);
double min(double a, double b);
int round_double_to_int(double x);
double abs_double(double x);
double normal_distributed_double();
