#include <iostream>
#include <cmath>
#include <stdexcept>
#include <memory>
#include <boost/math/distributions/normal.hpp>

// Define the OptionPricingStrategy interface
class OptionPricingStrategy {
public:
    virtual double calculate_option_price(double S, double K, double T, double r, double sigma) const = 0;
    virtual ~OptionPricingStrategy() = default;  // Add a virtual destructor
};

// Implement the Black-Scholes pricing strategy
class BlackScholesPricing : public OptionPricingStrategy {
public:
    double calculate_option_price(double S, double K, double T, double r, double sigma) const override {
        using namespace boost::math;

        double d1 = (1 / (sigma * sqrt(T))) * (log(S / K) + (r + (sigma * sigma) / 2) * T);
        double d2 = d1 - sigma * sqrt(T);
        double C = S * cdf(normal(), d1) - K * exp(-r * T) * cdf(normal(), d2);
        return C;
    }
};

// Implement a factory for creating pricing strategies
class PricingStrategyFactory {
public:
    static std::unique_ptr<OptionPricingStrategy> create_strategy(const std::string& strategy_type) {
        if (strategy_type == "BlackScholes") {
            return std::make_unique<BlackScholesPricing>();
        }
        // Add more strategies here as needed
        throw std::invalid_argument("Invalid strategy type");
    }
};

// Context class that uses the selected pricing strategy
class OptionPricingContext {
private:
    std::unique_ptr<OptionPricingStrategy> pricing_strategy;

public:
    OptionPricingContext(std::unique_ptr<OptionPricingStrategy> strategy) : pricing_strategy(std::move(strategy)) {}

    double calculate_option_price(double S, double K, double T, double r, double sigma) const {
        return pricing_strategy->calculate_option_price(S, K, T, r, sigma);
    }
};

int main() {
    // Butterfly spread parameters
    double S = 100;       // Current stock price
    double K1 = 95;       // Lower strike price
    double K2 = 100;      // Middle strike price
    double K3 = 105;      // Higher strike price
    double T = 1;         // Time to expiration (in years)
    double r = 0.05;      // Risk-free interest rate
    double sigma = 0.2;   // Volatility

    // Create a context with the selected pricing strategy
    std::string strategy_type = "BlackScholes";  // You can change this to select a different strategy
    auto pricing_strategy = PricingStrategyFactory::create_strategy(strategy_type);
    OptionPricingContext pricing_context(std::move(pricing_strategy));

    // Calculate the individual call option prices
    double C1 = pricing_context.calculate_option_price(S, K1, T, r, sigma);
    double C2 = pricing_context.calculate_option_price(S, K2, T, r, sigma);
    double C3 = pricing_context.calculate_option_price(S, K3, T, r, sigma);

    // Calculate the net cost of the long call butterfly spread
    double cost = C1 - 2 * C2 + C3;

    std::cout << "Cost of Long Call Butterfly Spread using " << strategy_type << ": " << cost << std::endl;

    return 0;
}
