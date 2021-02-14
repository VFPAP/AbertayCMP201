#pragma once

#include <string>

class rollingHash {
private:

	std::string text;
	unsigned int start = 0;
	unsigned short pattLength = 0;
	unsigned short hash = 0;

	char* charStart = nullptr;

	static constexpr unsigned short base = 257; //Should be a prime number close to the size of the alphabet (256)
	static constexpr unsigned short primeMod = 65521; //Should be a big prime number to limit the amount of spurious hits (hash collisions). spurious hits ≈ 1/Prime

	unsigned int multiplier = 1; //Used to precompute the multiplier that is used to implement the Horner's method to solve the polynomial of degree n, with n = pattern length

public:

	rollingHash(const std::string&, const unsigned short&);
	~rollingHash();

	void update();
	unsigned short hashValue();
	std::string textValue();
	unsigned short getStart();
};