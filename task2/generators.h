#define EPS 0.0001

#include <vector>
#include <any>
#include <random>
#include <stdexcept>


using TOptions = std::vector<std::any>;

class TRandomNumberGenerator {
public:
    virtual ~TRandomNumberGenerator() = default;
    virtual double Generate() const = 0;
};


class Poisson: public TRandomNumberGenerator {

private:
    mutable std::poisson_distribution<int> distr;
    mutable std::mt19937 gen;

public:
    Poisson(const TOptions &opts) {
        if (opts.size() != 1) {
            throw std::bad_cast();
        }

        double lambda = std::any_cast<double>(opts[0]);

        if (lambda <= 0) {
            throw std::invalid_argument("Lambda must be > 0");
        }

        std::random_device rd;

	gen = std::mt19937(rd());
	distr = std::poisson_distribution<int>(lambda);

    }

    double Generate() const {
        return distr(gen);
    }

    ~Poisson() {}
};


class Bernoulli: public TRandomNumberGenerator {

private:
    mutable std::bernoulli_distribution distr;
    mutable std::mt19937 gen;

public:
    Bernoulli(const TOptions &opts) {
        if (opts.size() != 1) {
            throw std::bad_cast();
        }
        double prob = std::any_cast<double>(opts[0]);

        if (prob < 0 or prob > 1) {
            throw std::invalid_argument("P must be >= 0 and <= 1");
        }

	std::random_device rd;

        distr = std::bernoulli_distribution(prob);
        gen = std::mt19937(rd());
    }

    double Generate() const {
        return distr(gen);
    }

    ~Bernoulli() {}
};



class Geometric: public TRandomNumberGenerator {

private:
    mutable std::geometric_distribution<int> distr;
    mutable std::mt19937 gen;

public:
    Geometric(const TOptions &opts) {
        if (opts.size() != 1) {
            throw std::bad_cast();
        }
        double prob = std::any_cast<double>(opts[0]);

        if (prob < 0 or prob > 1) {
            throw std::invalid_argument("All p_i must be >= 0 and <= 1");
        }

	std::random_device rd;
        distr = std::geometric_distribution<int>(prob);
        gen = std::mt19937(rd());
    }

    double Generate() const {
        return distr(gen);
    }

    ~Geometric() {}
};



class Discrete: public TRandomNumberGenerator {

private:
    mutable std::discrete_distribution<int> distr;
    mutable std::mt19937 gen;
    std::vector<double> values;

public:
    Discrete(const TOptions &opts) {
        if (opts.size() != 2) {
            throw std::bad_cast();
        }
        values = std::any_cast<std::vector<double>>(opts[0]);
        const std::vector<double> &probs = std::any_cast<std::vector<double>>(opts[1]);
        if (values.size() !=  probs.size()) {
            throw std::bad_cast();
        }

        double sum_probs = 0;
        for (auto& p : probs) {
            if (p < 0 or p > 1) {
                throw std::invalid_argument("All p_i must be >= 0 and <= 1");
            }
            sum_probs += p;
        }

        if (std::abs(sum_probs - 1) > EPS) {
            throw std::invalid_argument("All p_i must be summed to 1");
        }

        std::random_device rd;

	gen = std::mt19937(rd());
	distr = std::discrete_distribution<int>(probs.begin(), probs.end());
    }

    double Generate() const {
        return values[distr(gen)];
    }

    ~Discrete() {}

};

