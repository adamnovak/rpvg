
#include "catch2/catch.hpp"

#include "vg/io/basic_stream.hpp"
#include "../fragment_length_dist.hpp"
#include "../utils.hpp"


TEST_CASE("FragmentLengthDist is valid normal distribution") {
    
	FragmentLengthDist fragment_length_dist(10, 2);

    REQUIRE(fragment_length_dist.isValid());	
    REQUIRE(fragment_length_dist.maxLength() == 30);

    REQUIRE(doubleCompare(fragment_length_dist.logProb(9), -1.737085713764618));
    REQUIRE(doubleCompare(fragment_length_dist.logProb(15), -4.737085713764618));
    REQUIRE(doubleCompare(fragment_length_dist.logProb(9), fragment_length_dist.logProb(11)));
}

TEST_CASE("Fragment length distribution parameters can be parsed from vg::Alignment") {
    
    FragmentLengthDist fragment_length_dist;

    SECTION("Does not parse missing fragment length distribution") {

	    string alignment_str = R"({"sequence":"ACGT"})";

	    vg::Alignment alignment;
	    json2pb(alignment, alignment_str);

	    REQUIRE(!fragment_length_dist.parseAlignment(alignment));	
	}

    SECTION("Does not parse empty fragment length distribution") {

	    string alignment_str = R"({"fragment_length_distribution":"0:0:0:0:1"})";

	    vg::Alignment alignment;
	    json2pb(alignment, alignment_str);

	    REQUIRE(!fragment_length_dist.parseAlignment(alignment));	
	}

    SECTION("Parses fragment length distribution parameters") {

	    string alignment_str = R"({"fragment_length_distribution":"100:10:2:0:1"})";

	    vg::Alignment alignment;
	    json2pb(alignment, alignment_str);

	    REQUIRE(fragment_length_dist.parseAlignment(alignment));
	    REQUIRE(doubleCompare(fragment_length_dist.mean(), 10));
	    REQUIRE(doubleCompare(fragment_length_dist.sd(), 2));
	}
}

TEST_CASE("Fragment length distribution parameters can be parsed from vg::MultipathAlignment") {
    
    FragmentLengthDist fragment_length_dist;

    SECTION("Does not parse missing fragment length distribution") {

	    string alignment_str = R"({"sequence":"ACGT"})";

	    vg::MultipathAlignment alignment;
	    json2pb(alignment, alignment_str);

	    REQUIRE(!fragment_length_dist.parseMultipathAlignment(alignment));	
	}

    SECTION("Parses fragment length distribution parameters") {

	    string alignment_str = R"({"annotation":{"fragment_length_distribution":"-I 10 -D 2"}})";

	    vg::MultipathAlignment alignment;
	    json2pb(alignment, alignment_str);

	    REQUIRE(fragment_length_dist.parseMultipathAlignment(alignment));
	    REQUIRE(doubleCompare(fragment_length_dist.mean(), 10));
	    REQUIRE(doubleCompare(fragment_length_dist.sd(), 2));
	}
}

