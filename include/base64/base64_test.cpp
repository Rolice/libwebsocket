#include "base64.h"
#include <iostream>

#include "../sha1/sha1.h"

int main() {
	SHA1 sha;
	unsigned char digest[20];

	sha.Input("dGhlIHNhbXBsZSBub25jZQ==258EAFA5-E914-47DA-95CA-C5AB0DC85B11", 60);
	sha.Result((unsigned *) digest);

	std::string encoded = base64_encode(digest, sizeof(digest));
	std::string decoded = base64_decode(encoded);

	std::cout << "encoded: " << encoded << std::endl;
	std::cout << "decoded: " << decoded << std::endl;

	return 0;
}