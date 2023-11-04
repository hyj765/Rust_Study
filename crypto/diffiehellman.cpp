#include<iostream>
#include<vector>
#include <cstdlib> 
#include <ctime> 
using namespace std;

/*
* 
	기본 원리
	mod 특징
	(g^a mod p)^b mod p = g^ab mod p

	p = big prime number
	g = p primitive root modulo

	alice			=>			bob
1.				g^a mod p 
2.					<=			bob (b)
				g^b mod p
3.  (g^b mod p)^a mod p           (g^a mod p)^b mod p
	g^ba mod p					g^ab mod p





*/


class client {
public:
	double g;
	double p;
	double mynumber;
	double publickey;
	double secretkey;



	void InitializeMyNumber() {
		srand(time(nullptr));
		long randomnumber = rand() %100;
	}

	bool isPrime(int n) {
		if (n <= 1) return false;
		if (n <= 3) return true;
		if (n % 2 == 0 || n % 3 == 0) return false;
		for (int i = 5; i * i <= n; i += 6) {
			if (n % i == 0 || n % (i + 2) == 0) return false;
		}
		return true;
	}

	int getRandomPrime() {
		int minPrime = 10;  
		int maxPrime = 1000; 

		int randomPrime;
		do {
			randomPrime = minPrime + (std::rand() % (maxPrime - minPrime + 1));
		} while (!isPrime(randomPrime));

		return randomPrime;
	}
	
	int findAnyPrimitiveRoot(int p) {
		for (int g = 2; g < p; ++g) {
			std::vector<bool> visited(p, false);
			bool isPrimitiveRoot = true;

			for (int i = 1; i < p; ++i) {
				int power = static_cast<int>(pow(g, i)) % p;
				if (visited[power]) {
					isPrimitiveRoot = false;
					break;
				}
				visited[power] = true;
			}

			if (isPrimitiveRoot) {
				return g;
			}
		}

		return -1;
	}


	double Getpublickey(double p, double g, double myNumber) {
		return fmod(pow(g, myNumber), p);
	}
	double GetSecretKey() {
		return fmod(pow(publickey, mynumber),p);
	}

	void KeyExchange(client& other) {
		other.g = g;
		other.p = p;
		other.publickey = Getpublickey(p, g, mynumber);
		publickey = Getpublickey(other.p, other.g, other.mynumber);
		other.secretkey = other.GetSecretKey();
		secretkey = GetSecretKey();
	}

};



int main() {

	
	client a, b;
	
	//a.InitializeMyNumber();
	//b.InitializeMyNumber();

	a.p = 23;
	a.g = 5;
	a.mynumber = 6;
	b.mynumber = 3;
	a.KeyExchange(b);

	cout << a.secretkey << " " << b.secretkey;
}