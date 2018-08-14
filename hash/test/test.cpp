
#include "eteran/cpp-utilities/SHA1.h"
#include "eteran/cpp-utilities/MD5.h"
#include <iostream>
#include <cassert>

int main() {

	using hash::SHA1;
	using hash::MD5;

	{
		// --------------------- Test SHA1 ---------------------
		const std::string s = "Hello World";
		SHA1 sha1(s.begin(), s.end());

		auto d1 = sha1.finalize();
		std::cout << d1.to_string() << std::endl;
		assert(d1.to_string() == "0a4d55a8d778e5022fab701977c5d840bbc486d0");

		sha1 << '!';

		auto d2 = sha1.finalize();
		std::cout << d2.to_string() << std::endl;
		assert(d2.to_string() == "2ef7bde608ce5404e97d5f042f95f89f1c232871");

		auto d3 = SHA1().finalize();
		std::cout << d3.to_string() << std::endl;
		assert(d3.to_string() == "da39a3ee5e6b4b0d3255bfef95601890afd80709");

		auto d4 = SHA1("The quick brown fox jumps over the lazy dog.").finalize();
		std::cout << d4.to_string() << std::endl;
		assert(d4.to_string() == "408d94384216f890ff7a0c3528e8bed1e0b01621");

		auto d5 = SHA1("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA").finalize();
		std::cout << d5.to_string() << std::endl;
		assert(d5.to_string() == "5021b3d42aa093bffc34eedd7a1455f3624bc552");

		auto d6 = SHA1("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA").finalize();
		std::cout << d6.to_string() << std::endl;
		assert(d6.to_string() == "6b45e3cf1eb3324b9fd4df3b83d89c4c2c4ca896");

		auto d7 = SHA1("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA").finalize();
		std::cout << d7.to_string() << std::endl;
		assert(d7.to_string() == "e8d6ea5c627fc8676fa662677b028640844dc35c");

		auto d8 = SHA1({1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }).finalize();
		std::cout << d8.to_string() << std::endl;
		assert(d8.to_string() == "c5391e308af25b42d5934d6a201a34e898d255c6");
	}

	// --------------------- Test MD5 ---------------------
	{
		const std::string s = "Hello World";
		MD5 md5(s.begin(), s.end());

		auto d1 = md5.finalize();
		std::cout << d1.to_string() << std::endl;
		assert(d1.to_string() == "b10a8db164e0754105b7a99be72e3fe5");

		md5 << '!';

		auto d2 = md5.finalize();
		std::cout << d2.to_string() << std::endl;
		assert(d2.to_string() == "ed076287532e86365e841e92bfc50d8c");

		auto d3 = MD5().finalize();
		std::cout << d3.to_string() << std::endl;
		assert(d3.to_string() == "d41d8cd98f00b204e9800998ecf8427e");

		auto d4 = MD5("The quick brown fox jumps over the lazy dog.").finalize();
		std::cout << d4.to_string() << std::endl;
		assert(d4.to_string() == "e4d909c290d0fb1ca068ffaddf22cbd0");

		auto d5 = MD5("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA").finalize();
		std::cout << d5.to_string() << std::endl;
		assert(d5.to_string() == "e38a93ffe074a99b3fed47dfbe37db21");

		auto d6 = MD5("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA").finalize();
		std::cout << d6.to_string() << std::endl;
		assert(d6.to_string() == "a2f3e2024931bd470555002aa5ccc010");

		auto d7 = MD5("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA").finalize();
		std::cout << d7.to_string() << std::endl;
		assert(d7.to_string() == "9a7c38569e5a96e3cfbad45fb9ce5209");

		auto d8 = MD5({1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }).finalize();
		std::cout << d8.to_string() << std::endl;
		assert(d8.to_string() == "70903e79b7575e3f4e7ffa15c2608ac7");	
	}
}
