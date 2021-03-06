#pragma once

#include <stdint.h>

template<typename K, typename D>
class ITreeMap
{
public:

	virtual bool Search(D* outData, const K& key) const = 0;
	virtual bool Insert(const K& key, const D& data) = 0;
	virtual bool Delete(const K& key) = 0;

	virtual uint32_t GetSize() const = 0;
	virtual uint32_t GetHeight() const = 0;
	virtual void PrintTree() const = 0;
	
private:
};
