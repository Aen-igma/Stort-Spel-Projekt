#include "PCH.h"
#include "DBuffer.h"

namespace Aen {

	ElRef DBuffer::operator[](const std::string& name) {
		for(auto& i : m_layout->m_dataMap)
			if(i.first == name)
				return {m_data.data(), i.second.m_offset, i.second.m_type};

		throw;
	}
	
	
	Data::~Data() {
		delete[] m_data;
	}

	Data::Data()
		:m_type(DBType::None), m_offset(0), m_size(0), m_data(nullptr) {}

	Data::Data(const DBType& type, const uint32_t& byteSize)
		:m_type(type), m_offset(0), m_size(GetDataSize(type)), m_data(AEN_NEW Byte[byteSize]) {}

	Data::Data(const Data& rhs)
		:m_type(rhs.m_type), m_offset(rhs.m_offset), m_size(rhs.m_size) {
		m_data = AEN_NEW Byte[m_size];
		memcpy(m_data, rhs.m_data, m_size);
	}

	const uint32_t Data::GetDataSize(const DBType& type) {
		switch(type) {
			#define X(el) case DBType::el : return DBMap<DBType::el>::size; break;
			Def_DBType
				#undef X
		}

		return 0u;
	}
	

	DBLayout::DBLayout() {}

	ElRef DBLayout::operator[](const std::string& name) {
		for(auto& i : m_dataMap)
			if(i.first == name)
				return {i.second.m_data, i.second.m_offset, i.second.m_type};

		throw;
	}
	

	DBuffer::~DBuffer() {
		m_buffer.Reset();
	}

	DBuffer::DBuffer()
		: m_data(16), m_layout(nullptr), m_byteSize(0), m_buffer(NULL) {}

	DBuffer::DBuffer(DBLayout& layout)
		: m_data(16), m_layout(&layout), m_byteSize(0), m_buffer(NULL) {

		for(auto& i : m_layout->m_dataMap) {
			i.second.m_offset = m_byteSize;
			m_byteSize += i.second.m_size;
		}

		m_data.resize(m_byteSize);

		for(auto& i : m_layout->m_dataMap)
			memcpy(m_data.data() + i.second.m_offset, i.second.m_data, i.second.m_size);

		D3D11_BUFFER_DESC bDesc;
		ZeroMemory(&bDesc, sizeof(D3D11_BUFFER_DESC));

		int mod = m_byteSize % 16;
		UINT size = (mod == 0) ? static_cast<UINT>(m_byteSize) : static_cast<UINT>(m_byteSize + (16 - mod));

		bDesc.Usage = D3D11_USAGE_DYNAMIC;
		bDesc.ByteWidth = size;
		bDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bDesc.MiscFlags = 0;
		bDesc.StructureByteStride = 0;

		if(FAILED(m_device->CreateBuffer(&bDesc, nullptr, &m_buffer)))
			throw;
	}

	//DBuffer::DBuffer(const DBuffer& rhs) {
	//}

	void DBuffer::Create(DBLayout& layout) {
		m_layout = &layout;
		
		for(auto& i : m_layout->m_dataMap) {
			i.second.m_offset = m_byteSize;
			m_byteSize += i.second.m_size;
		}

		m_data.resize(m_byteSize);

		for(auto& i : m_layout->m_dataMap)
			memcpy(m_data.data() + i.second.m_offset, i.second.m_data, i.second.m_size);

		UINT mod = m_byteSize % 16u;
		UINT bytes = (mod == 0) ? static_cast<UINT>(m_byteSize) : static_cast<UINT>(m_byteSize + (16u - mod));

		D3D11_BUFFER_DESC bDesc;
		ZeroMemory(&bDesc, sizeof(D3D11_BUFFER_DESC));

		bDesc.Usage = D3D11_USAGE_DYNAMIC;
		bDesc.ByteWidth = (bytes < 16) ? 16 : bytes;
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
}