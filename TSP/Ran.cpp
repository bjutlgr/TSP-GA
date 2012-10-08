#include "Ran.h"

/*Uso de random:
crear una instancia de ran con alguna semila Ran myran(17)
para obtener un int de 64 bits ir llamando myrand.int64(), similar para 32 bits
para obtener un double entre 0.0 y 1.0 llamar myrand.doub()
para obtener un entero entre 1 y n (inclusivo) usar 1 + myran.int64() % (n-1)*/

//constructor: call with any integer seed (except value of v above)
//return 64-bit random integer 
unsigned long long int Ran::int64()
{
	u = u * 2862933555777941757LL + 7046029254386353087LL;
	v ^= v >> 17; v ^= v << 31; v ^= v >> 8;
	w = 4294957665U*(w & 0xffffffff) + (w >> 32);
	unsigned long long int x = u ^ (u << 21); x ^= x >> 35; x ^= x << 4;
	return (x + v) ^ w;
}
Ran::Ran(unsigned long long int j) : v(4101842887655102017LL), w(1)
{
	u = j^v; int64();
	v = u; int64();
	w = v; int64();
}

//return random double-precision floating value in the range 0 to 1.
double Ran::doub()
{
	return 5.42101086242752217E-20 * int64();
}

//return 32-bit random integer.
unsigned int Ran::int32()
{
	return (unsigned int)int64();
}
Ran::~Ran(void)
{
}
