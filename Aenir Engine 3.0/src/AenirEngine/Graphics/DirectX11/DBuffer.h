#pragma once
#include"BuritoMath.h"
#include"Shader.h"

namespace Aen {
	
	#define Def_DBType\
			X(Float)\
			X(Float2)\
			X(Float3)\
			X(Float4)\
			X(Float4x4)\
			X(Int)\
			X(Int2)\
			X(Int3)\
			X(Int4)\
			X(Bool)

	enum class DBType {
		None,
		#define X(el) el,
		Def_DBType
		#undef X
	};

	template<DBType T> struct DBMap {
		static constexpr bool valid = false;
	};
	template<> struct DBMap<DBType::Float> {
		using DType = float;
		static constexpr uint32_t size = sizeof(DType);
	};
	template<> struct DBMap<DBType::Float2> {
		using DType = Vec2f;
		static constexpr size_t size = sizeof(DType);
	};
	template<> struct DBMap<DBType::Float3> {
		using DType = Vec3f;
		static constexpr size_t size = sizeof(DType);
	};
	template<> struct DBMap<DBType::Float4> {
		using DType = Vec4f;
		static constexpr size_t size = sizeof(DType);
	};
	template<> struct DBMap<DBType::Float4x4> {
		using DType = Mat4f;
		static constexpr size_t size = sizeof(DType);
	};
	template<> struct DBMap<DBType::Int> {
		using DType = int;
		static constexpr size_t size = sizeof(DType);
	};
	template<> struct DBMap<DBType::Int2> {
		using DType = Vec2i;
		static constexpr size_t size = sizeof(DType);
	};
	template<> struct DBMap<DBType::Int3> {
		using DType = Vec3i;
		static constexpr size_t size = sizeof(DType);
	};
	template<> struct DBMap<DBType::Int4> {
		using DType = Vec4i;
		static constexpr size_t size = sizeof(DType);
	};
	template<> struct DBMap<DBType::Bool> {
		using DType = bool;
		static constexpr size_t size = 4U;
	};

	template<class T> struct DBRMap {
	};
	template<> struct DBRMap<float> {
		static constexpr DBType type = DBType::Float;
	};
	template<> struct DBRMap<Vec2f> {
		static constexpr DBType type = DBType::Float2;
	};
	template<> struct DBRMap<Vec3f> {
		static constexpr DBType type = DBType::Float3;
	};
	template<> struct DBRMap<Vec4f> {
		static constexpr DBType type = DBType::Float4;
	};
	template<> struct DBRMap<Color> {
		static constexpr DBType type = DBType::Float4;
	};
	template<> struct DBRMap<Mat4f> {
		static constexpr DBType type = DBType::Float4x4;
	};
	template<> struct DBRMap<int> {
		static constexpr DBType type = DBType::Int;
	};
	template<> struct DBRMap<Vec2i> {
		static constexpr DBType type = DBType::Int2;
	};
	template<> struct DBRMap<Vec3i> {
		static constexpr DBType type = DBType::Int3;
	};
	template<> struct DBRMap<Vec4i> {
		static constexpr DBType type = DBType::Int4;
	};
	template<> struct DBRMap<bool> {
		static constexpr DBType type = DBType::Bool;
	};

	class AEN_DECLSPEC ElRef {
		public:

		template<class T>
		operator T& () {
			if(DBRMap<T>::type == m_type)
				return *reinterpret_cast<T*>(m_byte + m_offset);

			throw;
		}

		template<class T>
		T& operator= (const T& rhs) {
			return static_cast<T&>(*this) = rhs;
		}

		private:
		using Byte = unsigned char;

		ElRef(Byte* byte, const uint32_t& offset, const DBType& type)
			:m_byte(byte), m_offset(offset), m_type(type) {}

		Byte* m_byte;
		uint32_t m_offset;
		const DBType m_type;

		friend class DBuffer;
		friend class DBLayout;
	};

	struct Data {
		using Byte = unsigned char;
		~Data();
		Data();
		Data(const DBType& type, const uint32_t& byteSize); // causing memory leak
		Data(const Data& rhs);

		DBType m_type;
		uint32_t m_offset;
		uint32_t m_size;
		Byte* m_data;

		private:

		const uint32_t GetDataSize(const DBType& type);
	};

	class DBLayout {
		public:
		DBLayout();

		template<DBType type>
		void Add(const std::string& name);

		ElRef operator[] (const std::string& name);

		private:
		std::vector<std::pair<std::string, Data>> m_dataMap;

		friend class DBuffer;
	};

	template<DBType type>
	inline void DBLayout::Add(const std::string& name) {
		using Byte = unsigned char;
		m_dataMap.emplace_back(name, Data(type, DBMap<type>::size));
	}

	class AEN_DECLSPEC DBuffer : public GCore {
		public:
		~DBuffer();
		DBuffer();
		DBuffer(DBLayout& layout);
		
		ElRef operator[] (const std::string& name);

		void Create(DBLayout& layout);
		void UpdateBuffer();

		template<class T>
		void BindBuffer(const UINT& slot);

		private:
		using Byte = unsigned char;

		std::vector<Byte> m_data;
		DBLayout* m_layout;
		uint32_t m_byteSize;
		ComBuffer m_buffer;
	};

	template<>
	inline void DBuffer::BindBuffer<VShader>(const UINT& slot) {
		m_dContext->VSSetConstantBuffers(slot, 1, m_buffer.GetAddressOf());
	}

	template<>
	inline void DBuffer::BindBuffer<HShader>(const UINT& slot) {
		m_dContext->HSSetConstantBuffers(slot, 1, m_buffer.GetAddressOf());
	}

	template<>
	inline void DBuffer::BindBuffer<CShader>(const UINT& slot) {
		m_dContext->CSSetConstantBuffers(slot, 1, m_buffer.GetAddressOf());
	}

	template<>
	inline void DBuffer::BindBuffer<DShader>(const UINT& slot) {
		m_dContext->DSSetConstantBuffers(slot, 1, m_buffer.GetAddressOf());
	}

	template<>
	inline void DBuffer::BindBuffer<GShader>(const UINT& slot) {
		m_dContext->GSSetConstantBuffers(slot, 1, m_buffer.GetAddressOf());
	}

	template<>
	inline void DBuffer::BindBuffer<PShader>(const UINT& slot) {
		m_dContext->PSSetConstantBuffers(slot, 1, m_buffer.GetAddressOf());
	}
}
