#include <gtest/gtest.h>
#include <wayward/support/datetime.hpp>
#include <wayward/testing/time_machine.hpp>

namespace wayward {
  void PrintTo(const wayward::DateTime& dt, std::ostream* os) {
    *os << dt.iso8601();
  }
}

namespace {
  using wayward::DateTime;
  using wayward::DateTimeDuration;
  using wayward::Nanoseconds;
  using wayward::Microseconds;
  using wayward::Milliseconds;
  using wayward::Seconds;
  using wayward::Minutes;
  using wayward::Hours;
  using wayward::Days;
  using wayward::Weeks;
  using wayward::Months;
  using wayward::Years;

  TEST(DateTime, instantiates_now) {
    DateTime now = DateTime::now();
  }

  TEST(DateTime, at_creates_a_date_at_nanosecond_precision) {
    auto date = DateTime::at(2012, 3, 21, 12, 21, 43, 100, 200, 300);
    EXPECT_EQ(2012, date.year());
    EXPECT_EQ(3, date.month());
    EXPECT_EQ(21, date.day());
    EXPECT_EQ(12, date.hour());
    EXPECT_EQ(21, date.minute());
    EXPECT_EQ(43, date.second());
    EXPECT_EQ(100, date.millisecond());
    EXPECT_EQ(200, date.microsecond());
    EXPECT_EQ(300, date.nanosecond());
  }

  TEST(DateTime, strftime_formats_iso8601) {
    auto date = DateTime::at(2012, 3, 21, 12, 21, 43);
    auto formatted = date.strftime("%Y-%m-%d %H:%M:%S %Z");
    EXPECT_EQ("2012-03-21 12:21:43 UTC", formatted);
  }

  TEST(DateTime, adds_nanoseconds) {
    auto date = DateTime::at(2012, 3, 21, 12, 21, 43, 100, 200, 300);
    auto x = date + Nanoseconds{10};
    auto expected = DateTime::at(2012, 3, 21, 12, 21, 43, 100, 200, 310);
    EXPECT_EQ(x, expected);
  }

  TEST(DateTime, overflowing_nanoseconds_become_microseconds) {
    auto date =     DateTime::at(2012, 3, 21, 12, 21, 43, 100, 200, 1300);
    auto expected = DateTime::at(2012, 3, 21, 12, 21, 43, 100, 201,  300);
    EXPECT_EQ(date, expected);
  }

  TEST(DateTime, adds_microseconds) {
    auto date = DateTime::at(2012, 3, 21, 12, 21, 43, 100, 200, 300);
    auto x = date + Microseconds{10};
    auto expected = DateTime::at(2012, 3, 21, 12, 21, 43, 100, 210, 300);
    EXPECT_EQ(x, expected);
  }

  TEST(DateTime, overflowing_microseconds_become_milliseconds) {
    auto date =     DateTime::at(2012, 3, 21, 12, 21, 43, 100, 1200, 300);
    auto expected = DateTime::at(2012, 3, 21, 12, 21, 43, 101,  200, 300);
    EXPECT_EQ(date, expected);
  }

  TEST(DateTime, adds_milliseconds) {
    auto date = DateTime::at(2012, 3, 21, 12, 21, 43, 100, 200, 300);
    auto x = date + Milliseconds{10};
    auto expected = DateTime::at(2012, 3, 21, 12, 21, 43, 110, 200, 300);
    EXPECT_EQ(x, expected);
  }

  TEST(DateTime, overflowing_milliseconds_become_seconds) {
    auto date =     DateTime::at(2012, 3, 21, 12, 21, 43, 1100, 200, 300);
    auto expected = DateTime::at(2012, 3, 21, 12, 21, 44,  100, 200, 300);
    EXPECT_EQ(date, expected);
  }

  TEST(DateTime, overflowing_seconds_become_minutes) {
    auto date =     DateTime::at(2012, 3, 21, 12, 21, 61, 100, 200, 300);
    auto expected = DateTime::at(2012, 3, 21, 12, 22,  1, 100, 200, 300);
    EXPECT_EQ(date, expected);
  }

  TEST(DateTime, overflowing_minutes_become_hours) {
    auto date =     DateTime::at(2012, 3, 21, 12, 61, 43, 100, 200, 300);
    auto expected = DateTime::at(2012, 3, 21, 13, 1,  43, 100, 200, 300);
    EXPECT_EQ(date, expected);
  }

  TEST(DateTime, overflowing_hours_become_days) {
    auto date =     DateTime::at(2012, 3, 21, 25, 21, 43, 100, 200, 300);
    auto expected = DateTime::at(2012, 3, 22,  1, 21, 43, 100, 200, 300);
    EXPECT_EQ(date, expected);
  }

  TEST(DateTime, overflowing_days_become_months) {
    auto date =     DateTime::at(2012, 3, 32, 12, 21, 43, 100, 200, 300);
    auto expected = DateTime::at(2012, 4, 1,  12, 21, 43, 100, 200, 300);
    EXPECT_EQ(date, expected);
  }

  TEST(DateTime, overflowing_days_become_months_in_regular_february) {
    auto date =     DateTime::at(2011, 2, 29, 12, 21, 43, 100, 200, 300);
    auto expected = DateTime::at(2011, 3, 1,  12, 21, 43, 100, 200, 300);
    EXPECT_EQ(date, expected);
  }

  TEST(DateTime, overflowing_days_become_months_in_leap_year_february) {
    auto date =     DateTime::at(2012, 2, 30, 12, 21, 43, 100, 200, 300);
    auto expected = DateTime::at(2012, 3, 1,  12, 21, 43, 100, 200, 300);
    EXPECT_EQ(date, expected);
  }

  TEST(DateTime, add_integer_month_to_get_same_day_of_month) {
    auto date = DateTime::at(2012, 3, 21, 12, 21, 43, 100, 200, 300);
    auto x = date + Months{1};
    auto expected = DateTime::at(2012, 4, 21, 12, 21, 43, 100, 200, 300);
    EXPECT_EQ(x, expected);
  }

  TEST(DateTime, adding_integer_month_gets_ultimo_in_leap_year) {
    auto date = DateTime::at(2012, 1, 31, 12, 21, 43, 100, 200, 300);
    auto x = date + Months{1};
    auto expected = DateTime::at(2012, 2, 29, 12, 21, 43, 100, 200, 300);
    EXPECT_EQ(x, expected);
  }

  TEST(DateTime, adding_one_year_to_leap_date_give_ultimo_february) {
    auto date = DateTime::at(2012, 2, 29, 0, 0, 0);
    auto x = date + Years{1};
    auto expected = DateTime::at(2013, 2, 28, 0, 0, 0);
    EXPECT_EQ(x, expected);
  }

  TEST(DateTime, treats_1900_as_common_year) {
    auto date = DateTime::at(1900, 2, 29, 0, 0, 0);
    EXPECT_EQ(date.month(), 3);
  }

  TEST(DateTime, treats_2000_as_leap_year) {
    auto date = DateTime::at(2000, 2, 29, 0, 0, 0);
    EXPECT_EQ(date.month(), 2);
  }

  TEST(DateTime, treats_1980_as_leap_year) {
    auto date = DateTime::at(1980, 2, 29, 0, 0, 0);
    EXPECT_EQ(date.month(), 2);
  }
}
