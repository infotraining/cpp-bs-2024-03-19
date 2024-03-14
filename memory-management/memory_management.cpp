#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;

struct Gadget
{
	std::string name;

	Gadget(const std::string& n = "not-set") // constructor
		: name(n)
	{
		std::cout << "Gadget(" << name << ": @" << this << ")\n";
	}

	Gadget(const Gadget&) = delete;
    Gadget& operator=(const Gadget&) = delete;

	~Gadget() // destructor
	{
		std::cout << "~Gadget(" << name << ": @" << this << ")\n";
	}

	void use()
	{
		std::cout << "Using " << name << ": @" << this << "\n";
	}
};

TEST_CASE("stack")
{
	Gadget g1_on_stack("ipad");
	g1_on_stack.use();
}