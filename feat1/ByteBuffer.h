#ifndef BYTEBUFFER_INCLUDED__
#define BYTEBUFFER_INCLUDED__

#include "ESpec.h"
#include "Swap.h"
#include <memory.h> 

///////////////////////////////////////////////// Declaration

///////////////////////// ByteBuffer

class ByteBuffer {
	typedef ByteBuffer This;
public:
	typedef unsigned char Byte;
public:
	ByteBuffer() NOTHROW;
	explicit ByteBuffer(size_t size) THROWSANY;
	ByteBuffer(const This & other) THROWSANY;
	~ByteBuffer() NOTHROW;
	void Allocate(size_t size) THROWSANY;
	void ReAllocate(size_t size) THROWSANY;
	void ReAllocateEx(size_t size) THROWSANY;
	void Free(void) THROWSANY;
	void * Detach(void) NOTHROW;
	void Attach(void * buffer,size_t size) NOTHROW;
	size_t GetSize(void) const NOTHROW;
	const Byte * Get(void) const NOTHROW;
	Byte * Get(void) NOTHROW;
	void Swap(This & other) NOTHROW;
	This & operator=(const This & other) THROWSANY;
	operator Byte * (void) NOTHROW;
	operator const Byte * (void) const NOTHROW;
	static void Copy(This & dest,const This & src) NOTHROW;
	static void Copy(void * dest,const void * src,size_t size) NOTHROW;
	static void Move(void * dest,const void * src,size_t size) NOTHROW;
	static void Zero(This & dest) NOTHROW;
	static void Zero(void * dest,size_t size) NOTHROW;
private:
	This & operator==(const This & other) const NOTHROW;
	This & operator!=(const This & other) const NOTHROW;
private:
	Byte * m_buffer;
	size_t m_size;
};

///////////////////////// GetAs

template <class C_>
C_ GetAs(ByteBuffer::Byte * pointer,C_ * dummy=0) NOTHROW {
	return reinterpret_cast<C_>(pointer);
}

template <class C_>
C_ GetAs(const ByteBuffer::Byte * pointer,C_ * dummy=0) NOTHROW {
	return reinterpret_cast<C_>(pointer);
}

///////////////////////////////////////////////// Implementation

///////////////////////// ByteBuffer

inline ByteBuffer::ByteBuffer() NOTHROW :
	m_buffer(0),
	m_size(0)
{
}

inline ByteBuffer::ByteBuffer(size_t size) THROWSANY :
	m_buffer(0),
	m_size(size)
{
	if (m_size) {
		m_buffer=new Byte [size];
		if (!m_buffer) {
			m_size=0;
		}
	}
}

inline ByteBuffer::ByteBuffer(const This & other) THROWSANY :
	m_buffer(0),
	m_size(other.m_size)
{
	if (m_size) {
		This(m_size).Swap(*this);
		Copy(*this,other);
	}
}

inline ByteBuffer::~ByteBuffer() NOTHROW {
	delete [] m_buffer;
}

inline void ByteBuffer::Allocate(size_t size) THROWSANY {
	This(size).Swap(*this);
}

inline void ByteBuffer::ReAllocate(size_t size) THROWSANY {
	enum { MaxSpeedable=0x08000000 };
	enum { Speed=2 };
	if (GetSize()<size) {
		ReAllocateEx(
			(size<=MaxSpeedable)?
				(size*Speed):size);
	}
}

inline void ByteBuffer::ReAllocateEx(size_t size) THROWSANY {
	if (GetSize()<size) {
		This temp(size);
		Copy(temp,*this);
		temp.Swap(*this);
	}
}

inline void ByteBuffer::Free(void) NOTHROW {
	This().Swap(*this);
}

inline void * ByteBuffer::Detach(void) NOTHROW {
	void * buffer=m_buffer;
	m_buffer=0;
	m_size=0;
	return buffer;
}

inline void ByteBuffer::Attach(void * buffer,size_t size) NOTHROW {
	Free();
	if (buffer && size) {
		m_buffer=static_cast<Byte*>(buffer);
		m_size=size;
	}
}

inline const ByteBuffer::Byte * ByteBuffer::Get(void) const NOTHROW {
	return m_buffer;
}

inline ByteBuffer::Byte * ByteBuffer::Get(void) NOTHROW {
	return m_buffer;
}

inline size_t ByteBuffer::GetSize(void) const NOTHROW {
	return m_size;
}

inline void ByteBuffer::Swap(This & other) NOTHROW {
	::Swap(m_buffer,other.m_buffer);
	::Swap(m_size,other.m_size);
}

inline ByteBuffer & ByteBuffer::operator=(const This & other) THROWSANY {
	if (this!=&other) {
		This(other).Swap(*this);
	}
	return *this;
}

inline ByteBuffer::operator ByteBuffer::Byte * (void) NOTHROW {
	return Get();
}

inline ByteBuffer::operator const ByteBuffer::Byte * (void) const NOTHROW {
	return Get();
}

inline void ByteBuffer::Copy(This & dest,const This & src) NOTHROW {
	size_t copySize=
		(dest.GetSize()>src.GetSize())?
			src.GetSize():dest.GetSize();
	if (copySize) {
		Copy(dest.m_buffer,src.m_buffer,copySize);
	}
}

inline void ByteBuffer::Copy(void * dest,const void * src,size_t size) NOTHROW {
	memcpy(dest,src,size);
}

inline void ByteBuffer::Move(void * dest,const void * src,size_t size) NOTHROW {
	memmove(dest,src,size);
}

inline void ByteBuffer::Zero(This & dest) NOTHROW {
	Zero(dest,dest.GetSize());
}

inline void ByteBuffer::Zero(void * dest,size_t size) NOTHROW {
	memset(dest,0,size);
}

/////////////////////////////////////////////////

#endif // BYTEBUFFER_INCLUDED__