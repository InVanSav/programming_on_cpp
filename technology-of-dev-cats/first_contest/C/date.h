#include <string>
class Date {
  int year, month, day;

  bool IsLeapYear(int years) const {
    return (year % 400 == 0 || year % 4 == 0 && year % 100 != 0);
  }

  static bool IsFirstBigger(const Date *d1, const Date d2) {
    if (d1->year > d2.year) return true;
    else if (d1->year < d2.year) return false;

    if (d1->month > d2.month) return true;
    else if (d1->month < d2.month) return false;

    if (d1->day > d2.day) return true;

    return false;
  }

 public:
  explicit Date(int year = 0, int month = 0, int day = 0)
      : year(year), month(month), day(day) {}

  std::string ToString() const {
    std::string d = (Date::day < 10 ? "0" : "") + std::to_string(Date::day);
    std::string m = (Date::month < 10 ? "0" : "") + std::to_string(Date::month);
    std::string y = (
        Date::year < 10 ? "000" :
        Date::year < 100 ? "00" :
        Date::year < 1000 ? "0" : "") + std::to_string(Date::year);
    return d + '.' + m + '.' + y;
  }

  bool IsLeap() const {
    return IsLeapYear(Date::year);
  }

  Date DaysLater(int days) const {
    int days_l = days;
    int ma[12]={31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int d = Date::day, m = Date::month, y = Date::year;
    int f;

    days_l += d;
    d = 0;

    for (int i = m - 2; i >= 0; i--) {
      if (i == 1 && IsLeapYear(y)) {
        days_l += 29;
        continue;
      } else {
        days_l += ma[i];
      }
    }
    m = 1;

    do {
      f = days_l;
      if (IsLeapYear(y)) {
        if (days_l >= 366) {days_l -= 366; y++;}
      } else {
        if (days_l >= 365) {days_l -= 365; y++;}
      }
      if (f == days_l) break;
    } while (true);

    for (int i = 0; i < 12; i++) {
      f = days_l;
      if (i == 1 && IsLeapYear(y)) {
        if (days_l >= 29) {
          m++;
          days_l -= 29;
        }
      } else {
        if (days_l >= ma[i]) {
          m++;
          days_l -= ma[i];
        }
      }
      if (f == days_l) break;
    }

    d = days_l;

    if (d == 0) {
      d = ma[m];
      m--;
    }
    return Date(y, m, d);
  }

  int DaysLeft(const Date& date) const {
    Date big, small;
    int days_res = 0;
    if (IsFirstBigger(this, date)) {
      big = *this;
      small = date;
    } else {
      big = date;
      small = *this;
    }

    int ma[12]={31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int f;

    int y = small.year, m = small.month, d = small.day;

    days_res -= d;
    d = 0;

    for (int i = m - 2; i >= 0; i--) {
      if (i == 1 && IsLeapYear(y)) {
        days_res -= 29;
        continue;
      } else {
        days_res -= ma[i];
      }
    }

    m = 1;
    do {
      f = days_res;
      if (IsLeapYear(y)) {
        if (y < big.year) {days_res += 366; y++;}
      } else {
        if (y < big.year) {days_res += 365; y++;}
      }
      if (f == days_res) break;
    } while (true);

    for (int i = 0; i < big.month - 1; i++) {
      if (i == 1 && IsLeapYear(y)) {
        m++;
        days_res += 29;
      } else {
        m++;
        days_res += ma[i];
      }
    }

    days_res += big.day;

    return days_res;
  }
};
