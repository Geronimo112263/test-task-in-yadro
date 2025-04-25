#include <gtest/gtest.h>

#include "../include/parser_file.h"

TEST(ComputerClubTest, ClientComeSuccessfully) {
  ComputerClub club(5, Time("09:00"), Time("21:00"), 100);
  Event e(Time("09:01"), 1, {"Alice"});
  club.handleEvent(e);

  Event expectedEvent(Time("09:01"), 1, {"Alice"});
  ASSERT_EQ(club.getAllEvents().back().args[0], expectedEvent.args[0]);
}

TEST(ComputerClubTest, ClientComesBeforeOpen) {
  ComputerClub club(5, Time("10:00"), Time("22:00"), 100);
  Event e(Time("09:59"), 1, {"Bob"});
  club.handleEvent(e);

  ASSERT_EQ(club.getAllEvents().back().args[0], "NotOpenYet");
}

TEST(ComputerClubTest, SitClientToTable) {
  ComputerClub club(3, Time("08:00"), Time("20:00"), 100);
  club.handleEvent(Event(Time("08:30"), 1, {"Charlie"}));
  club.handleEvent(Event(Time("08:40"), 2, {"Charlie", "1"}));

  ASSERT_TRUE(club.getTable(1).occupy);
}

TEST(ComputerClubTest, DoubleSitSameTable) {
  ComputerClub club(2, Time("09:00"), Time("22:00"), 100);
  club.handleEvent(Event(Time("09:10"), 1, {"Dave"}));
  club.handleEvent(Event(Time("09:15"), 2, {"Dave", "1"}));
  club.handleEvent(Event(Time("09:20"), 2, {"Dave", "1"}));

  ASSERT_EQ(club.getAllEvents().back().args[0], "PlaceIsBusy");
}

TEST(ComputerClubTest, ClientWaitsWhenNoFreeTables) {
  ComputerClub club(1, Time("10:00"), Time("22:00"), 100);
  club.handleEvent(Event(Time("10:05"), 1, {"Eva"}));
  club.handleEvent(Event(Time("10:06"), 2, {"Eva", "1"}));
  club.handleEvent(Event(Time("10:07"), 1, {"Frank"}));
  club.handleEvent(Event(Time("10:08"), 3, {"Frank"}));

  ASSERT_EQ(club.getWaitClientsCount(), 1);
}

TEST(TimeTest, FromStringAndToMinutes) {
  Time t("02:45");
  EXPECT_EQ(t.minutes, 165);
}

TEST(TimeTest, ConstructToMinutes) {
  Time t(88);
  EXPECT_EQ(t.minutes, 88);
}

TEST(TimeTest, Subtraction) {
  Time t1("10:30");
  Time t2("08:15");
  int result = t1 - t2;
  EXPECT_EQ(result, 135);
}

TEST(TimeTest, ComparisonOperators) {
  Time t1("09:00");
  Time t2("10:00");
  EXPECT_TRUE(t2 > t1);
  EXPECT_EQ(t2 + t1, 1140);
}

TEST(TimeTest, TimeInvalidEventThrows) {
  EXPECT_THROW(Time(""), std::runtime_error);
}

TEST(EventTest, ValidEvent) {
  Event event("08:00 1 client1");
  Time time("08:00");
  EXPECT_EQ(event.timeEvent.toString(), time.toString());
  EXPECT_EQ(event.idEvent, 1);
  EXPECT_EQ(event.args[0], "client1");
}

TEST(EventTest, EventWithTable) {
  Time time("09:10");
  Event event(time, 2, {"client2"});
  EXPECT_EQ(event.timeEvent.toString(), time.toString());
  EXPECT_EQ(event.idEvent, 2);
  EXPECT_EQ(event.args[0], "client2");
}

TEST(EventTest, InvalidEventThrows) {
  EXPECT_THROW(Event(""), std::runtime_error);
}