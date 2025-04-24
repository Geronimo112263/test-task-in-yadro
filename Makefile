.PHONY: all clean clang_format
CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -std=c++17 -O2
TESTFLAGS = -lgtest -lgtest_main
TEST = $(wildcard tests/club_tests.cc)
CLUB = $(wildcard src/*.cc)
MAIN = $(wildcard main.cc)
OBJTEST = $(TEST:.cc=.o)
OBJCLUB = $(CLUB:.cc=.o)
OBJMAIN = $(MAIN:.cc=.o)
CLUBEXE = club
TESTEXE = test
BUILD = build

all: $(CLUBEXE) $(TESTEXE)

$(CLUBEXE): $(OBJCLUB) $(OBJMAIN) | $(BUILD)
	$(CXX) $(CXXFLAGS) -o $@ $^
	mv $(CLUBEXE) $(BUILD)

$(TESTEXE): $(OBJTEST) $(OBJCLUB)
	$(CXX) $(CXXFLAGS) $(TESTFLAGS) -o $@ $^
	./$(TESTEXE)

$(BUILD):
	mkdir $(BUILD)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

clang_format:
	clang-format -n $(TEST) $(CLUB) $(MAIN)
	clang-format -i $(TEST) $(CLUB) $(MAIN)

clean:
	rm -f $(OBJCLUB) $(OBJTEST) $(OBJMAIN) $(TESTEXE)
	rm -rf $(BUILD)
