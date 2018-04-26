#include "stdafx.h"
#include "C6SPacket.h"
#include "C6SNetwork.h"
#include "CEncryption.h"

const void* C6SPacket::onSend(std::size_t &size) {
	// Add packet ID 
	*this << sf::Int8(m_iPacketID);

	size = getDataSize();

	// Encrypt data
	CEncrypt::XOREncode((char*)getData(), size, "6SGAME");

	return getData();
}
void C6SPacket::onReceive(const void *data, std::size_t size) {
	// Decode data
	//CEncrypt::XORDecode((char*)data, size);

	// Extract packet ID
	std::size_t s_size = sizeof(sf::Int8);
	std::memcpy(&m_iPacketID, (char*)(data)+size - s_size, s_size);

	// 'overloaded function must fill packet with recieved bytes'
	append(data, size - s_size);
} 