#include "PCH.h"
#include "DBuffer.h"

namespace Aen {

	ElRef DBuffer::operator[](const std::string& name) {
		for(auto& i : m_layout.get()->m_dataMap)
			if(i.first == name)
				return {m_data.data(), i.second.m_offset, i.second.m_type};

		throw;
	}
	
	
	Data::Data() 
		:m_type(None), m_offset(0), m_size(0), m_data(nullptr) {}

	Data::Data(const DBType& type, const uint32_t& byteSize)
		:m_type(type), m_offset(0), m_size(0), m_data(AEN_NEW Byte[byteSize]) {}
	

	DBLayout::DBLayout() {}

	ElRef DBLayout::operator[](const std::string& name) {
		for(auto& i : m_dataMap)
			if(i.first == name)
				return {i.second.m_data, i.second.m_offset, i.second.m_type};

		throw;
	}
	

	DBuffer::DBuffer()
		: m_data(16), m_layout(nullptr), m_byteSize(0), m_buffer(NULL) {}

	DBuffer::DBuffer(DBLayout& layout)
		: m_data(16), m_layout(std::make_unique<DBLayout>(layout)), m_byteSize(0), m_buffer(NULL) {

		for(auto& i : m_layout.get()->m_dataMap) {
			i.second.m_size = GetDataSize(i.second.m_type);
			i.second.m_offset = m_byteSize;
			m_byteSize += i.second.m_size;
		}

		m_data.resize(m_byteSize);

		for(auto& i : m_layout.get()->m_dataMap)
			memcpy(m_data.data() + i.second.m_offset, i.second.m_data, i.second.m_size);

		D3D11_BUFFER_DESC bDesc;
		ZeroMemory(&bDesc, sizeof(D3D11_BUFFER_DESC));

		bDesc.Usage = D3D11_USAGE_DYNAMIC;
		bDesc.ByteWidth = (m_byteSize < 16) ? 16 : static_cast<UINT>(m_byteSize);
		bDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bDesc.MiscFlags = 0;
		bDesc.StructureByteStride = 0;

		if(FAILED(m_device->CreateBuffer(&bDesc, nullptr, &m_buffer)))
			throw;
	}

	void DBuffer::Create(DBLayout& layout) {
		m_layout = std::make_unique<DBLayout>(layout);
		
		for(auto& i : m_layout.get()->m_dataMap) {
			i.second.m_size = GetDataSize(i.second.m_type);
			i.second.m_offset = m_byteSize;
			m_byteSize += i.second.m_size;
		}

		m_data.resize(m_byteSize);

		for(auto& i : m_layout.get()->m_dataMap)
			memcpy(m_data.data() + i.second.m_offset, i.second.m_data, i.second.m_size);

		D3D11_BUFFER_DESC bDesc;
		ZeroMemory(&bDesc, sizeof(D3D11_BUFFER_DESC));

		bDesc.Usage = D3D11_USAGE_DYNAMIC;
		bDesc.ByteWidth = (m_byteSize < 16) ? 16 : static_cast<UINT>(m_byteSize);
		bDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bDesc.MiscFlags = 0;
		bDesc.StructureByteStride = 0;

		if(FAILED(m_device->CreateBuffer(&bDesc, nullptr, &m_buffer)))
			throw;
	}

	void DBuffer::UpdateBuffer() {
		D3D11_MAPPED_SUBRESOURCE mResource;
		m_dContext->Map(m_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mResource);

		CopyMemory(mResource.pData, m_data.data(), m_byteSize);
		m_dContext->Unmap(m_buffer.Get(), 0);
	}

	const uint32_t DBuffer::GetDataSize(const DBType& type) {
		switch(type) {
			#define X(el) case el: return DBMap<el>::size; break;
			Def_DBType
				#undef X
		}
	}
}