#include <cassert>
#include <memory>
#include <string>
#include <stdexcept>
#include <iostream>
#include "factory.h"


double get_mean(const std::unique_ptr<TRandomNumberGenerator> &distr, int num_points=1000000) {
    double sum = 0;
    for(int i = 1; i <= num_points; i++) {
        sum += (distr->Generate() - sum) / i;
    }
    return sum;
}


bool compare_mean(const std::unique_ptr<TRandomNumberGenerator> &distr, double mean) {

	double diff = std::abs(get_mean(distr) - mean);
	return (diff < 0.01);
}


int main(int argc, char **argv) {

	TFactory factory;


	assert(compare_mean(factory.Create("poisson", {0.01}), 0.01));
	assert(compare_mean(factory.Create("poisson", {0.2}), 0.2));
	assert(compare_mean(factory.Create("poisson", {0.5}), 0.5));
	assert(compare_mean(factory.Create("poisson", {1.0}), 1.0));

	std::cout << "Poisson test passed\n";

	assert(compare_mean(factory.Create("bernoulli", {0.01}), 0.01));
	assert(compare_mean(factory.Create("bernoulli", {0.2}), 0.2));
	assert(compare_mean(factory.Create("bernoulli", {0.2}), 0.2));
	assert(compare_mean(factory.Create("bernoulli", {1.0}), 1.0));

	std::cout << "Bernoulli test passed\n";		

	assert(compare_mean(factory.Create("geometric", {0.2}), 4));
	assert(compare_mean(factory.Create("geometric", {0.3}), 0.7/0.3));
	assert(compare_mean(factory.Create("geometric", {0.6}), 0.2/0.3));
	assert(compare_mean(factory.Create("geometric", {0.9}), 1.0/9.0));

	std::cout << "Geometric test passed\n";
	
	assert(compare_mean(factory.Create("discrete", {std::vector<double>{1}, std::vector<double>{1}}), 1));
	assert(compare_mean(factory.Create("discrete", {std::vector<double>{1, 2}, std::vector<double>{0.2, 0.8}}), 1.8));
	assert(compare_mean(factory.Create("discrete", {std::vector<double>{1, 2, 3}, std::vector<double>{0.25, 0.25, 0.5}}), 2.25));
	assert(compare_mean(factory.Create("discrete", {std::vector<double>{1, 2, 3, 4}, std::vector<double>{0.5, 0, 0.4, 0.1}}), 2.1));

	std::cout << "Discrete test passed\n";

}
