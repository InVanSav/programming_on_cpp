class PrimeNumberGenerator {
  int l;

  static bool Prime(int n) {
    for (int i = 2; i < static_cast < int > (sqrt(n)) + 1; i++) {
      if (n % i == 0)
        return false;
    }
    return true;
  }

 public:
    explicit PrimeNumberGenerator(int start): l(start) {
      if (start == 1)
        l = 2;
    }

  int GetNextPrime() {
    while (!PrimeNumberGenerator::Prime(l)) {
      l+=1;
    }
    l+=1;
    return l - 1;
  }
};
