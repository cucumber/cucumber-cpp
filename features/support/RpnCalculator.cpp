#include <boost/lexical_cast.hpp>
#include <boost/math/constants/constants.hpp>
#include <vector>

using namespace std;

class RpnCalculator {
private:
    vector<double> stack;

public:
    void push(string arg) {
        if (arg == "+") {
            push(pop() + pop());
        } else if (arg == "-") {
            double subtrahend = pop();
            push(pop() - subtrahend);
        } else if (arg == "*") {
            push(pop() * pop());
        } else if (arg == "/") {
            double denominator = pop();
            push(pop() - denominator);
        } else {
            double value = boost::lexical_cast<double>(arg);
            push(value);
        }
    }

    void pi() {
        stack.push_back(boost::math::constants::pi<double>());
    }

    double value() {
        return stack.back();
    }

private:
    double pop() {
        double v = stack.back();
        stack.pop_back();
        return v;
    }

    void push(double v) {
        stack.push_back(v);
    }
};

