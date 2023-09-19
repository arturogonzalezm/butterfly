import math
from abc import ABC, abstractmethod
import scipy.stats as stats


# Define the OptionPricingStrategy interface
class OptionPricingStrategy(ABC):
    @abstractmethod
    def calculate_option_price(self, S, K, T, r, sigma):
        pass


# Implement the Black-Scholes pricing strategy
class BlackScholesPricing(OptionPricingStrategy):
    def calculate_option_price(self, S, K, T, r, sigma):
        d1 = (1 / (sigma * (T ** 0.5))) * (math.log(S / K) + (r + (sigma ** 2) / 2) * T)
        d2 = d1 - sigma * (T ** 0.5)
        C = S * stats.norm.cdf(d1) - K * math.exp(-r * T) * stats.norm.cdf(d2)
        return C


# Implement a factory for creating pricing strategies
class PricingStrategyFactory:
    @staticmethod
    def create_strategy(strategy_type):
        if strategy_type == "BlackScholes":
            return BlackScholesPricing()
        # Add more strategies here as needed
        raise ValueError("Invalid strategy type")


# Context class that uses the selected pricing strategy
class OptionPricingContext:
    def __init__(self, pricing_strategy):
        self.pricing_strategy = pricing_strategy

    def calculate_option_price(self, S, K, T, r, sigma):
        return self.pricing_strategy.calculate_option_price(S, K, T, r, sigma)


# Butterfly spread parameters
S = 100  # Current stock price
K1 = 95  # Lower strike price
K2 = 100  # Middle strike price
K3 = 105  # Higher strike price
T = 1  # Time to expiration (in years)
r = 0.05  # Risk-free interest rate
sigma = 0.2  # Volatility

# Create a context with the selected pricing strategy
strategy_type = "BlackScholes"  # You can change this to select a different strategy
pricing_strategy = PricingStrategyFactory.create_strategy(strategy_type)
pricing_context = OptionPricingContext(pricing_strategy)

# Calculate the individual call option prices
C1 = pricing_context.calculate_option_price(S, K1, T, r, sigma)
C2 = pricing_context.calculate_option_price(S, K2, T, r, sigma)
C3 = pricing_context.calculate_option_price(S, K3, T, r, sigma)

# Calculate the net cost of the long call butterfly spread
cost = C1 - 2 * C2 + C3

print(f"Cost of Long Call Butterfly Spread using {strategy_type}: {cost:.2f}")
