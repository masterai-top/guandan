#ifndef _ENDIAN_H_
#define _ENDIAN_H_

static bool _isLittleEndian()
{
	const short _v_ = 1;
	return ((*((char*)&_v_)) == 1);
}

#define _EXCHANGE16(n) ((((n) & 0xff) << 8) | (((n) >> 8) & 0xff))  
#define _EXCHANGE32(n) (((n) >> 24) | (((n) & 0x00ff0000) >> 8) | (((n) & 0x0000ff00) << 8) | ((n) << 24))  
#define _EXCHANGE64(n) (((n) >> 56) | (((n) & 0x00ff000000000000LL) >> 40) | (((n) & 0x0000ff0000000000LL) >> 24) | (((n) & 0x000000ff00000000LL) >> 8) | (((n) & 0x00000000ff000000LL) << 8) | (((n) & 0x0000000000ff0000LL) << 24) | (((n) & 0x000000000000ff00LL) << 40) | (((n) << 56))) 

#define _NTOH16(n) _isLittleEndian() ? _EXCHANGE16(((unsigned short)n)) : n
#define _HTON16(n) _isLittleEndian() ? _EXCHANGE16(((unsigned short)n)) : n

#define _NTOH32(n) _isLittleEndian() ? _EXCHANGE32(((unsigned int)n)) : n
#define _HTON32(n) _isLittleEndian() ? _EXCHANGE32(((unsigned int)n)) : n

//#define _NTOH64(n) _isLittleEndian() ? _EXCHANGE64(((unsigned long long)n)) : n
static unsigned long long _NTOH64(unsigned long long n)
{
	return _isLittleEndian() ? _EXCHANGE64(((unsigned long long)n)) : n;	
}
//#define _HTON64(n) _isLittleEndian() ? _EXCHANGE64(((unsigned long long)n)) : n
static unsigned long long _HTON64(unsigned long long n)
{
	return _isLittleEndian() ? _EXCHANGE64(((unsigned long long)n)) : n;
}

#endif
