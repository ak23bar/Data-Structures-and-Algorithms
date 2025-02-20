#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "include/search.h"

using namespace std;
using namespace testing;

// CleanToken Tests
TEST(CleanToken, AllToLower) {
  string text = "TESTING";
  string expected = "testing";

  EXPECT_THAT(cleanToken(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}
TEST(CleanToken, SomeToLower) {
  string text = "TeStiNG";
  string expected = "testing";

  EXPECT_THAT(cleanToken(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}
TEST(CleanToken, BeginPunct) {
  string text = ".TESTING";
  string expected = "testing";

  EXPECT_THAT(cleanToken(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}
TEST(CleanToken, EndPunct) {
  string text = "TESTING!?";
  string expected = "testing";

  EXPECT_THAT(cleanToken(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}
TEST(CleanToken, SandWitchPunct) {
  string text = ".TESTING.";
  string expected = "testing";

  EXPECT_THAT(cleanToken(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}
TEST(CleanToken, IgnorePunct) {
  string text = "!TES??TING...";
  string expected = "tes??ting";

  EXPECT_THAT(cleanToken(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}
TEST(CleanToken, OnlyNum) {
  string text = "1237475679827378";
  string expected = "";
  EXPECT_THAT(cleanToken(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}
TEST(CleanToken, EmptyString) {
  string text = "";
  string expected = "";
  EXPECT_THAT(cleanToken(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}
TEST(CleanToken, OnlyPunct) {
  string text = "...&*!&#&*$!@....";
  string expected = "";
  EXPECT_THAT(cleanToken(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}
TEST(CleanToken, PunctInMiddleNotAtEnds) {
  string text = "test.ing";
  string expected = "test.ing";
  EXPECT_THAT(cleanToken(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}
TEST(CleanToken, PunctInMiddleAndStart) {
  string text = ".test.ing";
  string expected = "test.ing";
  EXPECT_THAT(cleanToken(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}

TEST(CleanToken, PunctInMiddleAndEnd) {
  string text = "test.ing.";
  string expected = "test.ing";
  EXPECT_THAT(cleanToken(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}

// GatherTokens Tests
TEST(GatherTokens, NoPunct) {
  string text = "hi please pass this test i like trains";
  set<string> expected = {"hi",   "please", "pass", "this",
                          "test", "i",      "like", "trains"};
  EXPECT_THAT(gatherTokens(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}
TEST(GatherTokens, FrontSpaces) {
  string text = "  .hi pl+=ease  pa;!ss this test oka-=y";
  set<string> expected = {"hi", "pl+=ease", "pa;!ss", "this", "test", "oka-=y"};
  EXPECT_THAT(gatherTokens(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}
TEST(GatherTokens, EndSpaces) {
  string text = "  .hi pl+=ease  pa;!ss this test oka-=y      ";
  set<string> expected = {"hi", "pl+=ease", "pa;!ss", "this", "test", "oka-=y"};
  EXPECT_THAT(gatherTokens(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}
TEST(GatherTokens, SpacesBetweenTokens) {
  string text =
      "     .hi     pl+=ease    pa;!ss      this     test      oka-=y          "
      "  ";
  set<string> expected = {"hi", "pl+=ease", "pa;!ss", "this", "test", "oka-=y"};
  EXPECT_THAT(gatherTokens(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}
TEST(GatherTokens, SpacesBetweenTokenModified) {
  string text = "PLEase     pass    THiS   OR   ElsE";
  set<string> expected = {"please", "pass", "this", "or", "else"};
  EXPECT_THAT(gatherTokens(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}

// BuildIndex Tests
TEST(BuildIndex, TinyTest) {
  string filename = "data/tiny.txt";
  map<string, set<string>> expectedIndex = {
      {"eggs", {"www.shoppinglist.com"}},
      {"milk", {"www.shoppinglist.com"}},
      {"fish", {"www.shoppinglist.com", "www.dr.seuss.net"}},
      {"bread", {"www.shoppinglist.com"}},
      {"cheese", {"www.shoppinglist.com"}},
      {"red", {"www.rainbow.org", "www.dr.seuss.net"}},
      {"gre-en", {"www.rainbow.org"}},
      {"orange", {"www.rainbow.org"}},
      {"yellow", {"www.rainbow.org"}},
      {"blue", {"www.rainbow.org", "www.dr.seuss.net"}},
      {"indigo", {"www.rainbow.org"}},
      {"violet", {"www.rainbow.org"}},
      {"one", {"www.dr.seuss.net"}},
      {"fish", {"www.dr.seuss.net"}},
      {"two", {"www.dr.seuss.net"}},
      {"fish", {"www.dr.seuss.net"}},
      {"red", {"www.dr.seuss.net"}},
      {"fish", {"www.dr.seuss.net"}},
      {"blue", {"www.dr.seuss.net"}},
      {"fish", {"www.dr.seuss.net"}},
      {"i'm", {"www.bigbadwolf.com"}},
      {"not", {"www.bigbadwolf.com"}},
      {"trying", {"www.bigbadwolf.com"}},
      {"to", {"www.bigbadwolf.com"}},
      {"eat", {"www.bigbadwolf.com"}},
      {"you", {"www.bigbadwolf.com"}}};

  map<string, set<string>> myIndex;
  int MyNumProcesed = buildIndex(filename, myIndex);

  string indexFeedback =
      "buildIndex(\"" + filename + "\", ...) index incorrect\n";
  EXPECT_THAT(myIndex, ContainerEq(expectedIndex)) << indexFeedback;

  string retTestFeedback =
      "buildIndex(\"" + filename + "\", ...) return value incorrect\n";
  EXPECT_THAT(MyNumProcesed, Eq(4)) << retTestFeedback;
}
TEST(BuildIndex, InvalidFile) {
  string filename = "NotValidFile";
  map<string, set<string>> myIndex;
  int MyNumProcesed = buildIndex(filename, myIndex);
  string indexTestFeedback = "Invalid File, Expected to return 0";
  EXPECT_EQ(MyNumProcesed, 0) << indexTestFeedback;
}

// FindQueryMatches Tests
TEST(FindQueryMatches, FirstTermNotInMap) {
  map<string, set<string>> index = {
      {"Lebron", {"Lebron.com"}},
      {"Eduardo", {"Eduardo.com"}},
      {"James", {"James.com"}},
  };
  string sentence = "Lebronita Eduardo James";

  set<string> expected = {};

  EXPECT_THAT(findQueryMatches(index, sentence), ContainerEq(expected))
      << "Sentence=\"" << sentence << "\"";
}
TEST(FindQueryMatches, PositiveTermNotInMap) {
  map<string, set<string>> index = {
      {"Akbar", {"Akbar.com"}},
      {"Aman", {"Aman.com"}},
      {"Ahmed", {"Ahmed.com"}},
  };
  string sentence = "Akbar +Ethin";

  set<string> expected = {};

  EXPECT_THAT(findQueryMatches(index, sentence), ContainerEq(expected))
      << "Sentence=\"" << sentence << "\"";
}
TEST(FindQueryMatches, NegativeTermNotInMap) {
  map<string, set<string>> index = {
      {"Three", {"Three.com"}},
      {"Dunk", {"Two.com"}},
      {"Layup", {"Two.com"}},
      {"Halfcourt", {"Halfcourt.com"}},
  };
  string sentence = "Three -Touchdown";

  set<string> expected = {};

  EXPECT_THAT(findQueryMatches(index, sentence), ContainerEq(expected))
      << "Sentence=\"" << sentence << "\"";
}
TEST(FindQueryMatches, LaterTermNotInMap) {
  map<string, set<string>> index = {
      {"milk", {"milk.com"}},
      {"sprite", {"unhealthy.com"}},
      {"water", {"healthy.com"}},
  };
  string sentence = "milk horchata";

  set<string> expected = {"milk.com"};

  EXPECT_THAT(findQueryMatches(index, sentence), ContainerEq(expected))
      << "Sentence=\"" << sentence << "\"";
}

// Search Engine Tests are built in, no need to write any on my end.