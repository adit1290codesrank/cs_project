#pragma once
#include <bitset>
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

//Functions to read data from environment file
std::map<std::string, std::string> readEnv(std::string path);
std::pair<std::string, std::string> splitKeyValue(std::string text);

std::map<std::string, std::string> readEnv(std::string path)
{
	std::map<std::string, std::string> env;
	std::ifstream read_file(path);
	std::string text;
	while (std::getline(read_file, text))
	{
		std::cout << text;
		std::pair<std::string, std::string> key_value = splitKeyValue(text);
		env.insert(key_value);
	}
	return env;
}

std::pair<std::string, std::string> splitKeyValue(std::string text)
{
	std::string key = "", value = "";
	int pos = text.find("=");
	key = text.substr(0, pos);
	value = text.substr(pos + 1);
	return { key, value };
}

//Taken from NIST spec http://csrc.nist.gov/publications/fips/fips180-4/fips-180-4.pdf
#define ROTRIGHT(word,bits) (((word) >> (bits)) | ((word) << (32-(bits))))
#define SSIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))
#define SSIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))
#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define BSIG0(x) (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22))
#define BSIG1(x) (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25))

class Encryption {
	private:
		std::string message;
		//Converts to binary
		std::vector<unsigned long> to_binary(std::string string);
		//Adds padding(1000....) to make length 512-64 bits
		std::vector<unsigned long> padding(std::vector<unsigned long> vector);
		//Resize from 64 8 bits sections to 16 32 bits sections
		std::vector<unsigned long> resize(std::vector<unsigned long> vector);
		//Computees sha256 hashing following rules from NIST
		std::string compute_sha256_hash(std::vector<unsigned long> vector);
		//Convertys from binary to hex as a string
		std::string to_hexadecimal(unsigned long input);
	public:
		Encryption(std::string message_to_store);
		//Return sha256 encryption
		std::string encrypt_sha256();
};

std::vector<unsigned long> Encryption::to_binary(std::string string)
{
	std::vector<unsigned long> binary;
	for (int i = 0; i < string.size(); i++)
	{
		//Converts every character in message to 8 bits
		std::bitset<8> bits(string.c_str()[i]);
		binary.push_back(bits.to_ulong());
	}
	return binary;
}
std::vector<unsigned long> Encryption::padding(std::vector<unsigned long> vector)
{
	int length = vector.size() * 8; //Every character is represented as 8 bits so total length * 8

	//We want to calculate how many 0's to add to make it 64 bits short of 512
	//length + 1 + k = 512-64
	//k = 447 - length
	int k = 447 - length;

	//Add a 8 bit block with first bit as 1
	unsigned long t1 = 0x80;//Equivalent to 128 in decimal and 1000000 in binary
	vector.push_back(t1);
	k = k - 7;//Remove 7 0's as we added 7 already

	for (int i = 0; i < k / 8; i++)
	{
		vector.push_back(0x00000000);//Adding 8 bit blocks which just contain 0's
	}

	//Now we have 488 bits out of 512. Add 1 in the form of 8 bit blocks

	std::bitset <64> bits_64(length);
	std::string blob_string = bits_64.to_string();//Splits the 64 bit message into 8 bit sections

	std::bitset<8> temp(blob_string.substr(0, 8));//Push first block into 56th position
	vector.push_back(temp.to_ulong());

	for (int i = 0; i < 63; i=i+8)
	{
		std::bitset<8> temp2(blob_string.substr(i, 8));//Keep pushing in form a 8 bit blocks
		vector.push_back(temp2.to_ulong());
	}

	return vector;
}
std::vector<unsigned long> Encryption::resize(std::vector<unsigned long> vector)
{
	std::vector<unsigned long> to_return(16);

	for (int i = 0; i < 64; i=i+4)
	{
		std::bitset<32> temp(0);

		//Shift blocks to assigned spots or merge them
		temp = (unsigned long) vector[i] << 24;
		temp |= (unsigned long) vector[i + 1] << 16;
		temp |= (unsigned long) vector[i + 2] << 8;
		temp |= (unsigned long) vector[i + 3];

		to_return[i / 4] = temp.to_ulong();
	}

	return to_return;

}

std::string Encryption::to_hexadecimal(unsigned long input)
{
	std::bitset<32> bits_32(input);
	unsigned long n = bits_32.to_ulong();
	std::stringstream ss;
	ss << std::hex << std::setw(8) << std::setfill('8') << n;
	return ss.str();
}

Encryption::Encryption(std::string message_to_store)
{
	message = message_to_store;
}

std::string Encryption::compute_sha256_hash(std::vector<unsigned long> vector)
{
	// Taken from the spec
	//   SHA-224 and SHA-256 use the same sequence of sixty-four constant
	//   32-bit words, K0, K1, ..., K63.  These words represent the first 32
	//   bits of the fractional parts of the cube roots of the first sixty-
	//   four prime numbers.
	unsigned long k[64] = {
		0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,
		0x923f82a4,0xab1c5ed5,0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,
		0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,0xe49b69c1,0xefbe4786,
		0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
		0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,
		0x06ca6351,0x14292967,0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,
		0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,0xa2bfe8a1,0xa81a664b,
		0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
		0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,
		0x5b9cca4f,0x682e6ff3,0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,
		0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
	};

	// Initial Hash Values, first thirty-two bits of the fractional parts of
	// the square roots of the first eight prime numbers. 

	unsigned long static H0 = 0x6a09e667;
	unsigned long static H1 = 0xbb67ae85;
	unsigned long static H2 = 0x3c6ef372;
	unsigned long static H3 = 0xa54ff53a;
	unsigned long static H4 = 0x510e527f;
	unsigned long static H5 = 0x9b05688c;
	unsigned long static H6 = 0x1f83d9ab;
	unsigned long static H7 = 0x5be0cd19;

	unsigned long W[64];

	for (int i = 0; i < 16; i++)
	{
		W[i] = vector[i] & 0xFFFFFFFF;
	}

	for (int i = 16; i < 64; i++)
	{
		W[i] = SSIG1(W[i - 2]) + W[i - 7] + SSIG0(W[i - 15]) + W[i - 16];
		W[i] = W[i] & 0xFFFFFFFF;
	}

	unsigned long temp1;
	unsigned long temp2;
	unsigned long a = H0;
	unsigned long b = H1;
	unsigned long c = H2;
	unsigned long d = H3;
	unsigned long e = H4;
	unsigned long f = H5;
	unsigned long g = H6;
	unsigned long h = H7;

	for (int i = 0; i < 64; i++)
	{
		temp1 = h + BSIG1(e) + CH(e, f, g) + k[i] + W[i];
		
		temp2 = BSIG0(a) + MAJ(a, b, c);

		// Do the working variables operations as per NIST.

		h = g;
		g = f;
		f = e;
		e = (d + temp1) & 0xFFFFFFFF; // Makes sure that we are still using 32 bits.
		d = c;
		c = b;
		b = a;
		a = (temp1 + temp2) & 0xFFFFFFFF; // Makes sure that we are still using 32 bits.
	}

	// Add up all the working variables to each hash and make sure we are still
	// working with solely 32 bit variables.
	H0 = (H0 + a) & 0xFFFFFFFF;
	H1 = (H1 + b) & 0xFFFFFFFF;
	H2 = (H2 + c) & 0xFFFFFFFF;
	H3 = (H3 + d) & 0xFFFFFFFF;
	H4 = (H4 + e) & 0xFFFFFFFF;
	H5 = (H5 + f) & 0xFFFFFFFF;
	H6 = (H6 + g) & 0xFFFFFFFF;
	H7 = (H7 + h) & 0xFFFFFFFF;
	return to_hexadecimal(H0) + to_hexadecimal(H1) + to_hexadecimal(H2) + to_hexadecimal(H3) + to_hexadecimal(H4) + to_hexadecimal(H5) + to_hexadecimal(H6) + to_hexadecimal(H7);
}

std::string Encryption::encrypt_sha256()
{
	std::vector<unsigned long> vector;
	vector = to_binary(message);

	vector = padding(vector);

	vector = resize(vector);

	return compute_sha256_hash(vector);
}

