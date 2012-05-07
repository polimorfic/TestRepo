#include "stdafx.h"
#include "Base64.h"


//////////////////////////////////////////////
// see RFC 2045


namespace 
{

////////////////////////////////////////////////
// таблица дл€ кодировани€
const char enc_table[] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
	'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
	'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
	'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
	'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
	'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
	'w', 'x', 'y', 'z', '0', '1', '2', '3',
	'4', '5', '6', '7', '8', '9', '+', '/'
};

// таблица дл€ декодировани€
const char dec_table[] = {
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
	52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
	64,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
	15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
	64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
	41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64
};

const char padding_symbol = '=';

////////////////////////////////////////////////
// кодирует 3 вход€щих байта в 4 исход€щих
void encode_bucket(char* dst, char a, char b, char c) 
{
	//dst[0] = enc_table[(a >> 2) & 0x3F];
	//dst[1] = enc_table[((a << 4) | (b >> 4)) & 0x3F];
	//dst[2] = enc_table[((b << 2) | (c >> 6)) & 0x3F];
	//dst[3] = enc_table[c & 0x3F];

	dst[0] = enc_table[(a & 0xFC) >> 2];
	dst[1] = enc_table[(a & 0x03) << 4 | (b & 0xF0) >> 4];
	dst[2] = enc_table[(b & 0x0F) << 2 | (c & 0xC0) >> 6];
	dst[3] = enc_table[c & 0x3F];
}

// кодирует 3 вход€щих байта в 4 исход€щих
void encode_bucket(char* dst, const char* src) 
{
	encode_bucket(dst, src[0], src[1], src[2]);
}

// кодирует 1 вход€щий байт в 4 исход€щих
void encode_bucket_padding_1(char* dst, const char* src) 
{
	encode_bucket(dst, src[0], 0, 0);
	dst[2] = padding_symbol;
	dst[3] = padding_symbol;
}

// кодирует 2 вход€щих байта в 4 исход€щих
void encode_bucket_padding_2(char* dst, const char* src) 
{
	encode_bucket(dst, src[0], src[1], 0);
	dst[3] = padding_symbol;
}

////////////////////////////////////////////////
// декодирует 4 вход€щих байта в 3 исход€щих
bool decode_bucket(char* dst, const char* src) 
{
	char a = dec_table[src[0]];
	char b = dec_table[src[1]];
	char c = dec_table[src[2]];
	char d = dec_table[src[3]];
	if ((a | b | c | d) & 0x40) {
		return false;
	}
	dst[0] = (a << 2) | (b >> 4);
	dst[1] = (b << 4) | (c >> 2);
	dst[2] = (c << 6) | d;
	return true;
}

} // namespace


/////////////////////////////////////////////////
bool IsBase64Symbol(char ch)
{
	return 	(ch >= 'A' && ch <= 'Z') ||
			(ch >= 'a' && ch <= 'z') ||
			(ch >= '0' && ch <= '9') ||			
			ch == '+' ||
			ch == '/' ||
			ch == padding_symbol;
}

bool IsNotBase64Symbol(char ch)
{
	return !IsBase64Symbol(ch);
}


/////////////////////////////////////////////////
namespace Base64 
{

/////////////////////////////////////////////////
void Encode(std::vector<char>& dst, const char* src, size_t srcLen)
{
	if (srcLen == 0) {
		dst.clear();
		return;
	}
	const size_t tailLen = srcLen % 3;
	const size_t dstLenWOPadding = ((srcLen - tailLen) * 4) / 3;
	dst.resize(dstLenWOPadding);
	if(dstLenWOPadding) {
		EncodeWOPadding(&dst[0], src, srcLen - tailLen);
	}
	if (tailLen != 0) {
		dst.resize(dstLenWOPadding + 4);
		EncodeWithPadding(&dst[dstLenWOPadding], src + (srcLen - tailLen), tailLen);
	}
}

void EncodeWOPadding(char* dst, const char* src, size_t srcLen)
{
	_ASSERT(srcLen % 3 == 0);

	const char* const srcEnd = src + srcLen;
	while (src < srcEnd) {
		encode_bucket(dst, src);
		src += 3;
		dst += 4;
	}
}

void EncodeWOPadding(char* dst, char a, char b, char c, const char* src, size_t srcLen)
{
	encode_bucket(dst, a, b, c);
	dst += 4;
	EncodeWOPadding(dst, src, srcLen);
}

void EncodeWithPadding(char* dst, const char* src, size_t srcLen)
{
	_ASSERT(srcLen == 1 || srcLen == 2);

	if (srcLen == 1) {
		encode_bucket_padding_1(dst, src);
	} else {
		encode_bucket_padding_2(dst, src);
	}
}

////////////////////////////////////////////////////
bool Decode(std::vector<char>& dst, const char* src, size_t srcLen) 
{
	if (srcLen == 0) {
		return true;
	}

	_ASSERT(srcLen % 4 == 0);

	dst.resize((srcLen * 3)/ 4);

	size_t dstLen = 0;
	for (dstLen = 0; srcLen > 4; src += 4, srcLen -= 4, dstLen += 3) {
		if (!decode_bucket(&dst[dstLen], src)) {
			return false;
		}
	}
	if (srcLen != 4) {
		return false;
	}
	size_t dstPaddingLen = 0;
	if (src[3] == padding_symbol) {
		++dstPaddingLen;
		if (src[2] == padding_symbol) {
			++dstPaddingLen;
		}
	}	
	char buffer[4] = {'A', 'A', 'A', 'A'};
	for (size_t i = 0; i < 4 - dstPaddingLen; ++i) {
		buffer[i] = src[i];
	}
	if (decode_bucket(&dst[dstLen], &buffer[0])) {
		dst.resize(dst.size() - dstPaddingLen);
		return true;
	}
	return false;
}


} // namespace Base64
