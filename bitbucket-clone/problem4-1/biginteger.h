#include <vector>
#include <string>

class BigInteger;

const BigInteger operator* (const BigInteger &first, const BigInteger &second);

const BigInteger operator+ (const BigInteger &first, const BigInteger &second);

const BigInteger operator- (const BigInteger &first, const BigInteger &second);

const BigInteger operator/ (const BigInteger &first, const BigInteger &second);

const BigInteger operator% (const BigInteger &first, const BigInteger &second);

bool operator>= (const BigInteger &first, const BigInteger &second);

bool operator<= (const BigInteger &first, const BigInteger &second);

class BigInteger {
public:
    friend std::istream &operator >> (std::istream &istr, BigInteger &tmp);

    friend bool operator== (const BigInteger &first, const BigInteger &second);

    friend bool operator> (const BigInteger &first, const BigInteger &second);

    friend bool operator< (const BigInteger &first, const BigInteger &second);

    BigInteger() : sign_(1) {}

    BigInteger(const int& n_) {
        int n = n_;

        if (n >= 0) {
            sign_ = 1;
        }
        else {
            sign_ = -1;
            n *= -1;
        }
        if (n == 0) buffer_.push_back(0);
        while (n > 0) {
            buffer_.push_back(n % 10);
            n /= 10;
        }
        Normalaze(*this);
    }
    
    BigInteger(const BigInteger &tmp) {
        sign_ = tmp.sign_;
        buffer_ = tmp.buffer_;
        Normalaze(*this);
    }

    explicit operator bool() const {
        return *this == 0 ? false : true;
    }

    std::string toString() const {
        std::string tmp;
        if (sign_ == -1)
            tmp.push_back('-');
        for (int i = size() - 1; i >= 0; --i) {
            tmp.push_back(buffer_[i] + '0');
        }
        return tmp;
    }

    operator std::string() const {
        return toString();
    }

    BigInteger operator= (const BigInteger &other) {
        sign_ = other.sign_;
        buffer_ = other.buffer_;
        Normalaze(*this);
        return *this;
    }
    
    BigInteger operator-() const {
        BigInteger tmp;
        tmp.sign_ = (-1) * sign_;
        tmp.buffer_ = buffer_;
        Normalaze(tmp);
        return tmp;
    }

    BigInteger& operator++ () {
        return *this += 1;
    }

    BigInteger& operator-- () {
        return *this -= 1;
    }

    const BigInteger operator++ (int) {
        BigInteger old = *this;
        ++(*this);
        return old;
    }
    
    const BigInteger operator-- (int) {
        BigInteger old = *this;
        --(*this);
        return old;
    }
    
    BigInteger& operator-=(const BigInteger &other) {
        if (sign_ * other.sign_ == 1)
            Minus(other);
        else
            Plus(-other);
        Normalaze(*this);
        return *this;
    }

    BigInteger& operator+=(const BigInteger &other) {
        if (sign_ * other.sign_ == 1)
            Plus(other);
        else
            Minus(-other);
        Normalaze(*this);
        return *this;
    }

    BigInteger& operator*=(const BigInteger &other_) {
        BigInteger other = other_;
        int sig = other.sign_ * sign_;
        if (AbsGraterEq(other))
            Multiply(other.buffer_, buffer_, buffer_);
        else
            Multiply(buffer_, other.buffer_, buffer_);
        sign_ = sig;
        Normalaze(*this);
        return *this;
    }

    BigInteger& operator%=(const BigInteger &other1) {
        int sig = sign_;
        BigInteger other = other1;
        other.sign_ = 1;
        BigInteger r;
        Divide(*this, other, r,*this);
        *this = r;
        sign_ = sig;
        Normalaze(*this);
        return *this;
    }

    BigInteger& operator/=(const BigInteger &other1) {
        BigInteger other = other1;
        int sig = other.sign_ * sign_;
        other.sign_ = 1;
        BigInteger r;
        Divide(*this, other, r,*this);
        sign_ = sig;
        Normalaze(*this);
        return *this;
    }

private:
    std::vector<int> buffer_;
    int sign_;
    
    void swap(int &a, int &b) {
        int c = a;
        a = b;
        b = c;
    }

    int size() const {
        return (int)buffer_.size();
    }

    void Divide(const BigInteger &first, const BigInteger &second, BigInteger &rest, BigInteger & q) {
        BigInteger div;
        int k = 0;
        while (first.size() > k) {
            ++k;
            rest.buffer_.push_back(first.buffer_[first.size() - k]);
            for (int i = rest.size() - 2; i >= 0; --i)
                swap(rest.buffer_[i], rest.buffer_[i + 1]);

            int a = 5;
            while (rest.AbsGreater(second * a))
                ++a;
            while (rest.AbsLess(second * a))
                --a;
            div.buffer_.push_back(a);
            rest -= second * a;
            if (rest.buffer_.back() == 0)
                rest.buffer_.pop_back();
        }
        div.Reverse();
        if (rest.size() == 0)
            rest.buffer_.push_back(0);
        q = div;
    }


    void CheckIfZero(BigInteger &tmp) const {
        if (tmp.size() == 1 && tmp.buffer_.back() == 0)
            tmp.sign_ = 1;
    }

    void Normalaze(BigInteger &tmp) const {
        Cut(tmp);
        CheckIfZero(tmp);
    }

    void Plus(const BigInteger &other) {
        if (AbsGraterEq(other)) {
            int tmp = 0;
            for (int i = 0; i < other.size(); ++i) {
                buffer_[i] = (buffer_[i] + other.buffer_[i] + tmp);
                tmp = buffer_[i] / 10;
                buffer_[i] %= 10;
            }
            for (int i = other.size(); i < size(); ++i) {
                buffer_[i] = (buffer_[i] + tmp);
                tmp = buffer_[i] / 10;
                buffer_[i] %= 10;
            }
            if (tmp)
                buffer_.push_back(tmp);
        }
        else {
            int tmp = 0;
            for (int i = 0; i < size(); ++i) {
                buffer_[i] = (buffer_[i] + other.buffer_[i] + tmp);
                tmp = buffer_[i] / 10;
                buffer_[i] %= 10;;
            }
            for (int i = size(); i < other.size(); ++i) {
                buffer_.push_back((other.buffer_[i] + tmp));
                tmp = buffer_[i] / 10;
                buffer_[i] %= 10;
            }
            if (tmp)
                buffer_.push_back(tmp);
            sign_ = other.sign_;

        }
    }

    void Minus(const BigInteger &other) {
        if (AbsGraterEq(other)) {
            int d = 0;
            for (int i = 0; i < other.size(); ++i) {
                buffer_[i] = (buffer_[i] - other.buffer_[i] + d);
                if (buffer_[i] < 0) {
                    d = -1;
                    buffer_[i] += 10;
                }
                else
                    d = 0;
            }
            for (int i = other.size(); i < size(); ++i) {
                buffer_[i] = (buffer_[i] + d);
                if (buffer_[i] < 0) {
                    d = -1;
                    buffer_[i] += 10;
                }
                else
                    d = 0;
            }
        }
        else {
            int d = 0;
            for (int i = 0; i < size(); ++i) {
                buffer_[i] = -buffer_[i] + other.buffer_[i] + d;
                if (buffer_[i] < 0) {
                    d = -1;
                    buffer_[i] += 10;
                }
                else
                    d = 0;
            }
            for (int i = size(); i < other.size(); ++i) {
                buffer_.push_back(other.buffer_[i] + d);
                if (buffer_[i] < 0) {
                    d = -1;
                    buffer_[i] += 10;
                }
                else
                    d = 0;
            }
            sign_ = other.sign_ * (-1);
        }
    }

    void Cut(BigInteger &tmp) const {
        while (tmp.buffer_[tmp.size() - 1] == 0 && tmp.size() != 1)
            tmp.buffer_.pop_back();
    }
    
    bool AbsLess(const BigInteger &other) const {
        if (size() > other.size())
            return false;
        else if (size() < other.size())
            return true;
        else {
            for (int i = size() - 1; i >= 0; --i) {
                if (buffer_[i] < other.buffer_[i])
                    return true;
                else if (buffer_[i] > other.buffer_[i])
                    return false;
            }
            return false;
        }
    }

    bool AbsGraterEq(const BigInteger &other) const {
        return !AbsLess(other);
    }

    bool AbsGreater(const BigInteger &other) const {
        if (size() > other.size())
            return true;
        else if (size() < other.size())
            return false;
        else {
            for (int i = size() - 1; i >= 0; --i) {
                if (buffer_[i] < other.buffer_[i])
                    return false;
                else if (buffer_[i] > other.buffer_[i])
                    return true;

            }
            return false;
        }
    }

    bool AbsLessEq(const BigInteger &other) const {
        return !AbsGreater(other);
    }

    bool AbsEqual(const BigInteger &other) const {
        if (size() != other.size())
            return false;
        for (int i = 0; i < size(); ++i)
            if (buffer_[i] != other.buffer_[i])
                return false;
        return true;
    }


    void Multiply(const std::vector<int> &x, const std::vector<int> &y, std::vector <int> &answer) {
        int len = x.size() + y.size();
        std::vector<int> res(len);

        for (int i = 0; i < (int)x.size(); ++i) {
            for (int j = 0; j < (int)y.size(); ++j) {
                res[i + j] += x[i] * y[j];
                res[i + j + 1] += res[i + j] / 10;
                res[i + j] %= 10;
            }
        }
        answer = res;
    }


    void Reverse() {
        for (int i = 0; i < size() / 2; ++i)
            swap(buffer_[i], buffer_[size() - 1 - i]);
    }
};
std::ostream& operator<<(std::ostream& ostr, const BigInteger& tmp)
{
    ostr << (tmp).toString();
    return ostr;
}

std::istream &operator >> (std::istream &istr, BigInteger& tmp)
{
    tmp.buffer_.clear();
    char symbol = '0';
    istr.get(symbol);
    while ((symbol < '0' || symbol > '9') && symbol != '-' && !istr.eof())
        istr.get(symbol);
    if (symbol == '-') {
        tmp.sign_ = -1;
        istr.get(symbol);
    }
    else
        tmp.sign_ = 1;

    while (!((symbol < '0' || symbol > '9') && symbol != '-')&& !istr.eof())
    {
        tmp.buffer_.push_back(symbol - '0');
        istr.get(symbol);
    }
    tmp.Reverse();
    tmp.Normalaze(tmp);
    return istr;
}

const BigInteger operator+ (const BigInteger & a, const BigInteger& b) {
    BigInteger tmp = a;
    return tmp += b;
}
const BigInteger operator- (const BigInteger & a, const BigInteger& b) {
    BigInteger tmp = a;
    return tmp -= b;
}
const BigInteger operator* (const BigInteger& first, const BigInteger& second) {
    BigInteger tmp = first;
    return tmp *= second;
}

const BigInteger operator/ (const BigInteger& first, const BigInteger& second) {
    BigInteger tmp = first;
    return tmp /= second;
}
const BigInteger operator% (const BigInteger& first, const BigInteger& second) {
    BigInteger tmp = first;
    return tmp %= second;
}

bool operator< (const BigInteger& first, const BigInteger& second) {
    if (first.sign_ > second.sign_)
        return false;
    if (first.sign_ < second.sign_)
        return true;
    return first.sign_ == 1 ? first.AbsLess(second) : first.AbsGreater(second);
}

bool operator >= (const BigInteger& first, const BigInteger& second) {
    return !(first<second);
}

bool operator > (const BigInteger& first, const BigInteger& second) {
    if (first.sign_ > second.sign_)
        return true;
    if (first.sign_ < second.sign_)
        return false;
    return first.sign_ == 1 ? first.AbsGreater(second) : first.AbsLess(second);
}

bool operator <= (const BigInteger& first, const BigInteger& second) {
    return !(first > second);
}

bool operator == (const BigInteger& first, const BigInteger& second) {
    if (first.sign_ != second.sign_)
        return false;
    return first.AbsEqual(second);
}

bool operator != (const BigInteger& first, const BigInteger& second) {
    return !(first == second);
}