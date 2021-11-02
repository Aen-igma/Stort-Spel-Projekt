#include "PCH.h"
#include "LevelExporter.h"

namespace Aen {

	LevelExporter::LevelExporter()
	{
		OpenFile();
	}

	LevelExporter::~LevelExporter()
	{
		CloseFile();
	}

	void LevelExporter::OpenFile()
	{
		m_outfile.open(m_filePath + m_fileName, std::ofstream::out | std::ofstream::binary);

	}

	void LevelExporter::CloseFile()
	{
		m_outfile.close();

	}

	template<class T>
	inline void LevelExporter::WriteToFile(T* whatToWrite, std::ofstream& outfile)
	{
		outfile.write((const char*)&*whatToWrite, sizeof(T));
	}

	void LevelExporter::WriteInto(vector<Aen::Entity*>& entityList, vector<string>& itemList, vector<string>& meshObjList, vector<string>& textureFileName, vector<string>& entityType)
	{
		cout << "writeInto" << endl;
		cout << "entityList " << entityList.size() << endl;
		cout << "itemList " << itemList.size() << endl;
		cout << "meshObjList " << meshObjList.size() << endl;
		cout << "textureFileName " << textureFileName.size() << endl;
		cout << "entityType " << entityType.size() << endl;


		RoomStruct roomStruct;
		ModelStruct* modelStruct = new ModelStruct();
		TextureStruct textureStruct;
		LightStruct lightStruct;
		MaterialStruct materialStruct;
		ParticleStruct particleStruct;

		m_ModelVector.reserve(meshObjList.size());
		m_TextureVector.reserve(textureFileName.size());

		int meshIndex = 0;


		for (size_t i = 0; i < entityList.size(); i++)
		{
			if (entityType[i] == "Model")
			{
				strcpy(modelStruct->name, itemList[i].c_str());
				strcpy(modelStruct->mesh, meshObjList[meshIndex].c_str());

				modelStruct->translation[0] = entityList[i]->GetPos().x;
				modelStruct->translation[1] = entityList[i]->GetPos().y;
				modelStruct->translation[2] = entityList[i]->GetPos().z;

				modelStruct->rotation[0] = entityList[i]->GetRot().x;
				modelStruct->rotation[1] = entityList[i]->GetRot().y;
				modelStruct->rotation[2] = entityList[i]->GetRot().z;

				modelStruct->scale[0] = entityList[i]->GetScale().x;
				modelStruct->scale[1] = entityList[i]->GetScale().y;
				modelStruct->scale[2] = entityList[i]->GetScale().z;

				strcpy(modelStruct->type, "type");
				strcpy(modelStruct->sound, "sound");

				m_ModelVector.push_back(*modelStruct);
				cout << m_ModelVector[meshIndex].name << endl;
				cout << m_ModelVector[meshIndex].mesh << endl;
				meshIndex++;
			}
			else if(entityType[i] == "Directional light")
			{
				strcpy(lightStruct.type, entityType[i].c_str());
				cout << lightStruct.type << endl;
			}
			else if (entityType[i] == "Spot light")
			{
				strcpy(lightStruct.type, entityType[i].c_str());
				cout << lightStruct.type << endl;
			}
			else if (entityType[i] == "Point light")
			{
				strcpy(lightStruct.type, entityType[i].c_str());
				cout << lightStruct.type << endl;
			}

			cout << endl;
		}

		ModelHeader *modelHeader = new ModelHeader();
		WriteToFile(modelHeader, m_outfile);

		for (size_t i = 0; i < m_ModelVector.size(); i++)
		{
			*modelStruct = m_ModelVector[i];
			WriteToFile(modelStruct, m_outfile);
		}

		m_ModelVector.clear();

		delete modelHeader;
	}
}
