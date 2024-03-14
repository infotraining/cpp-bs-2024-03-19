#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;

namespace LegacyCode
{
	double avg(const int* data, const int size)
	{
		double sum = 0.0;

		for (const int* ptr_item = data; ptr_item != data + size; ++ptr_item)
			sum += *ptr_item;

		return sum / size;
	}
}

namespace ModernCpp
{
	// TODO: Modernize avg function
}

TEST_CASE("avg")
{
    int data[] = { 1, 2, 3, 4, 5 };

    CHECK(LegacyCode::avg(data, 5) == 3.0);   
    //CHECK(ModernCpp::avg(data, 5) == 3.0);
}