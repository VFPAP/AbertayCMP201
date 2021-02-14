#include "rollingHash.h"

/**
 * @brief Constructor of the rollingHash Class.
 *
 * Every time an object is created, this function will calculate the hash of the first N characters of the string passed as a parameter. N=pattern length
 *
 * @param str The text of the object (either a line of the text file or the pattern).
 * @param size The length of the pattern.
 */
rollingHash::rollingHash(const std::string& str, const unsigned short& size) { //O(M), M=pattern length
	text = str;
	pattLength = size;
	charStart = &text.front();

	//base^(pattLength-1) % primeMod
	for (unsigned short i = 1; i < pattLength; i++)
		multiplier = (multiplier * base) % primeMod;

	for (unsigned short i = 0; i < pattLength; i++) {
		hash = (hash * base + int(*charStart)) % primeMod; //Using pointers instead of indexing the string "text" (text[i]) 
		charStart++; //Increment the address the pointer is pointing to
	}
	charStart -= pattLength; //Revert the address being pointed to the initial value (charStart = &text.front())
}

/**
 * @brief Destructor of the rollingHash Class.
 */
rollingHash::~rollingHash() {}

/**
 * @brief Calculate the hash of the next portion of text being tested.
 *
 * This function updates the hash of a N-length portion of text (N=pattern length) by removing the first char of the current rolling window and adding the char that follows the end of the current window.
 *
 * rolling window = a [b c d] e f
 * <Remove first char of the current rolling window>
 * rolling window = a b [c d] e f
 * <Add the char that follows the current rolling window>
 * rolling window = a b [c d e] f
 */
void rollingHash::update() { //O(1)

	//If the start of the next "window" is not the end of the string ('\0'): window = a b c [d e f] \0  ==>  a b c d [e f \0]
	if (charStart != nullptr && *(charStart + pattLength) != '\0') {

		hash = (hash + primeMod - multiplier * int(*charStart) % primeMod) % primeMod; //Remove first char of the current rolling window
		hash = (hash * base + int(*(charStart + pattLength))) % primeMod; //Add the char that follows the current rolling window

		charStart++; //Increment the address the pointer is pointing to

		start++; //To keep track of the index of the first char of the substring we're testing
	}
}

/**
 * @brief Get the current hash value of the object.
 *
 * @return unsigned short with the hash.
 */
unsigned short rollingHash::hashValue() {
	return hash;
}

/**
 * @brief Get the current text value (the portion of text being analysed) of the object.
 *
 * @return string with the portion of the text.
 */
std::string rollingHash::textValue() {
	return text.substr(start, pattLength);
}

/**
 * @brief Get the index of the first char of the portion of text being analysed.
 *
 * @return unsigned short with the index.
 */
unsigned short rollingHash::getStart() {
	return start;
}
