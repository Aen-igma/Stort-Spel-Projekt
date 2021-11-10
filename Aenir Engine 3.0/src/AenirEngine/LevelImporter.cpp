// LevelImporter


#include "PCH.h"
#include "LevelImporter.h"

namespace AenIMP {

	void Room::readFromFile(ifstream& infile)
	{
		char* ptr = (char*)&roomHeader;
		ptr = ptr + sizeof(Aen::SectionHeader);
		infile.read(ptr, sizeof(Aen::RoomHeader) - sizeof(Aen::SectionHeader));
		this->room = new Aen::RoomStruct;
		infile.read((char*)&this->room->type, sizeof(*room));
	}

	void Room::printRoom()
	{
		cout << "type :" << room->type << endl;
		cout << "special :" << room->special << endl;
		cout << "theme :" << room->theme << endl;
		cout << "probability :" << room->probability << endl;
	}

	Room::Room()
	{
	}

	Room::~Room()
	{
		delete this->room;
	}

	void Model::readFromFile(ifstream& infile)
	{
		char* ptr = (char*)&modelHeader;
		ptr = ptr + sizeof(Aen::SectionHeader);
		infile.read(ptr, sizeof(Aen::ModelHeader) - sizeof(Aen::SectionHeader));
		this->model = new Aen::ModelStruct;
		infile.read((char*)&this->model->name, sizeof(*model));
	}

	void Model::printModel()
	{
		cout << "Name: " << model->name << endl;
		cout << "Mesh: " << model->mesh << endl;
		cout << "Translation: " << model->translation[0] << " " << model->translation[1] << " " << model->translation[2] << endl;
		cout << "Rotation: " << model->rotation[0] << " " << model->rotation[1] << " " << model->rotation[2] << endl;
		cout << "Scale: " << model->scale[0] << " " << model->scale[1] << " " << model->scale[2] << endl;
		cout << "Type " << model->type << endl;
		cout << "Sound: " << model->sound << endl;
	}

	Model::Model()
	{
	}

	Model::~Model()
	{
		cout << "D modelStruct" << endl;
		if (model != nullptr)
		{
			delete this->model;
		}
	}

	void Texture::readFromFile(ifstream& infile)
	{
		char* ptr = (char*)&textureHeader;
		ptr = ptr + sizeof(Aen::SectionHeader);
		infile.read(ptr, sizeof(Aen::TextureHeader) - sizeof(Aen::SectionHeader));
		this->texture = new Aen::TextureStruct;
		infile.read((char*)&this->texture->name, sizeof(*texture));
	}

	void Texture::printTexture()
	{
		cout << "name :" << texture->name << endl;
		cout << "textureType :" << texture->textureType << endl;
	}

	Texture::Texture()
	{

	}

	Texture::~Texture()
	{
		cout << "D textureStruct" << endl;
		delete this->texture;
	}

	void Material::readFromFile(ifstream& infile)
	{
		char* ptr = (char*)&materialHeader;
		ptr = ptr + sizeof(Aen::SectionHeader);
		infile.read(ptr, sizeof(Aen::MaterialHeader) - sizeof(Aen::SectionHeader));
		this->material = new Aen::MaterialStruct;
		infile.read((char*)&this->material->diffuse, sizeof(*material));
	}

	void Material::printMaterial()
	{
		cout << "diffuse: " << material->diffuse[0] << " " << material->diffuse[1] << " " << material->diffuse[2] << endl;
		cout << "specular: " << material->specular[0] << " " << material->specular[1] << " " << material->specular[2] << endl;
		cout << "emissive: " << material->emissive[0] << " " << material->emissive[1] << " " << material->emissive[2] << endl;
		cout << "opacity: " << material->opacity[0] << " " << material->opacity[1] << " " << material->opacity[2] << endl;
	}

	Material::Material()
	{

	}

	Material::~Material()
	{
		cout << "D materialStruct" << endl;
		delete this->material;
	}

	void Light::readFromFile(ifstream& infile)
	{
		char* ptr = (char*)&lightHeader;
		ptr = ptr + sizeof(Aen::SectionHeader);
		infile.read(ptr, sizeof(Aen::LightHeader) - sizeof(Aen::SectionHeader));
		this->light = new Aen::LightStruct;
		infile.read((char*)&this->light->type, sizeof(*light));
	}

	void Light::printLight()
	{
		cout << "Light" << endl;
		cout << "position :" << light->translation[0] << " " << light->translation[1] << " " << light->translation[2] << endl;
		cout << "rotation :" << light->rotation[0] << " " << light->rotation[1] << " " << light->rotation[2] << endl;
		cout << "direction :" << light->direction[0] << " " << light->direction[1] << " " << light->direction[2] << endl;
		cout << "intensity :" << light->intensity << endl;
		cout << "light Type :" << light->type << endl;
		cout << "color :" << light->color[0] << " " << light->color[1] << " " << light->color[2] << endl;
		cout << "range :" << light->range << endl;
		cout << "attenuation :" << light->attenuation[0] << " " << light->attenuation[1] << " " << light->attenuation[2] << endl;
		cout << "angle :" << light->angle << endl;

		cout << endl;
	}

	Light::Light()
	{
	}

	Light::~Light()
	{
		delete this->light;
	}

	void Particle::readFromFile(ifstream& infile)
	{
		char* ptr = (char*)&particleHeader;
		ptr = ptr + sizeof(Aen::SectionHeader);
		infile.read(ptr, sizeof(Aen::ParticleHeader) - sizeof(Aen::SectionHeader));
		this->particle = new Aen::ParticleStruct;
		infile.read((char*)&this->particle->translation, sizeof(*particle));
	}

	void Particle::printParticle()
	{
		cout << "translation: " << particle->translation[0] << " " << particle->translation[1] << " " << particle->translation[2] << endl;
		cout << "type: " << particle->type << endl;
	}

	Particle::Particle()
	{
	}

	Particle::~Particle()
	{
		cout << "D particleStruct" << endl;
		delete particle;
	}

	LevelImporter::LevelImporter()
	{

	}

	LevelImporter::~LevelImporter()
	{
	}

	void LevelImporter::ReadFromFile(string filePath)
	{
		OpenFile(filePath);
		std::cout << "-------------" << endl;

		CompleteRoom tempRoom;

		while (infile.eof() == false)
		{
			Aen::SectionHeader sectionHeader;
			infile.read((char*)&sectionHeader, sizeof(Aen::SectionHeader));

			if (sectionHeader.type == Aen::TYPE::MODEL)
			{
				Model* model = new Model;
				model->readFromFile(infile);
				model->printModel();

				tempRoom.addModel(model);
				delete model;
			}
			else if (sectionHeader.type == Aen::TYPE::TEXTURE)
			{
				Texture* texture = new Texture;
				texture->readFromFile(infile);
				texture->printTexture();

				tempRoom.addTexture(texture);
				delete texture;
			}
			else if (sectionHeader.type == Aen::TYPE::MATERIAL)
			{
				Material* material = new Material;
				material->readFromFile(infile);
				material->printMaterial();

				tempRoom.addMaterial(material);
				delete material;
			}
			else if (sectionHeader.type == Aen::TYPE::LIGHT)
			{
				Light* light = new Light;
				light->readFromFile(infile);
				light->printLight();

				tempRoom.addLight(light);
				delete light;
			}
			else if (sectionHeader.type == Aen::TYPE::PARTICLE)
			{
				Particle* particle = new Particle;
				particle->readFromFile(infile);
				particle->printParticle();

				tempRoom.addParticle(particle);
				delete particle;
			}
			else if (sectionHeader.type == Aen::TYPE::ROOM)
			{
				Room* room = new Room;
				room->readFromFile(infile);
				room->printRoom();

				tempRoom.addRoom(room);
				delete room;
			}
			else if (sectionHeader.type == Aen::TYPE::UNKNOWN)
			{
				std::cout << "Unkown Header Type" << endl;
			}

			cout << endl;
		}

		roomVector.push_back(tempRoom);

		std::cout << "-------------------" << endl;
		CloseFile();

	}

	void LevelImporter::OpenFile(string filePath)
	{
		infile.open(filePath, std::ios::in | std::ios::binary);
		if (!infile)
		{
			cout << "Cannot open file!" << endl;
		}
		cout << "File is Open" << endl;
	}

	void LevelImporter::CloseFile()
	{
		if (infile.is_open())
		{
			infile.close();
		}
		else
		{
			cout << "File already closed" << endl;
		}
	}

	void LevelImporter::printArray()
	{
		for (size_t i = 0; i < roomVector.size(); i++)
		{
			roomVector[i].print();
		}
	}

	vector<CompleteRoom>& LevelImporter::GetRoomVector()
	{
		return roomVector;
	}



	CompleteRoom::CompleteRoom()
	{
	}

	CompleteRoom::~CompleteRoom()
	{
	}

	void CompleteRoom::print()
	{
		for (size_t i = 0; i < modelVector.size(); i++)
		{
			cout << "Name: " << modelVector[i].name << endl;
			cout << "Mesh: " << modelVector[i].mesh << endl;
			cout << "Translation: " << modelVector[i].translation[0] << " " << modelVector[i].translation[1] << " " << modelVector[i].translation[2] << endl;
			cout << "Rotation: " << modelVector[i].rotation[0] << " " << modelVector[i].rotation[1] << " " << modelVector[i].rotation[2] << endl;
			cout << "Scale: " << modelVector[i].scale[0] << " " << modelVector[i].scale[1] << " " << modelVector[i].scale[2] << endl;
			cout << "Type: " << modelVector[i].type << endl;
			cout << "Sound: " << modelVector[i].sound << endl;
			cout << endl << "-----------------------------------" << endl;
		}

		for (size_t i = 0; i < textureVector.size(); i++)
		{
			cout << "Name: " << textureVector[i].name << endl;
			cout << "Texture Type: " << textureVector[i].textureType << endl;
			cout << endl << "-----------------------------------" << endl;
		}

		for (size_t i = 0; i < materialVector.size(); i++)
		{
			cout << "diffuse: " << materialVector[i].diffuse[0] << " " << materialVector[i].diffuse[1] << " " << materialVector[i].diffuse[2] << endl;
			cout << "specular: " << materialVector[i].specular[0] << " " << materialVector[i].specular[1] << " " << materialVector[i].specular[2] << endl;
			cout << "emissive: " << materialVector[i].emissive[0] << " " << materialVector[i].emissive[1] << " " << materialVector[i].emissive[2] << endl;
			cout << "opacity: " << materialVector[i].opacity[0] << " " << materialVector[i].opacity[1] << " " << materialVector[i].opacity[2] << endl;
			cout << endl << "-----------------------------------" << endl;
		}

		for (size_t i = 0; i < lightVector.size(); i++)
		{
			cout << "type: " << lightVector[i].type << endl;
			cout << "translation: " << lightVector[i].translation[0] << " " << lightVector[i].translation[1] << " " << lightVector[i].translation[2] << endl;
			cout << "rotation: " << lightVector[i].rotation[0] << " " << lightVector[i].rotation[1] << " " << lightVector[i].rotation[2] << endl;
			cout << "direction: " << lightVector[i].direction[0] << " " << lightVector[i].direction[1] << " " << lightVector[i].direction[2] << endl;
			cout << "intensity: " << lightVector[i].intensity << endl;
			cout << "color: " << lightVector[i].color[0] << " " << lightVector[i].color[1] << " " << lightVector[i].color[2] << endl;
			cout << "range: " << lightVector[i].range << endl;
			cout << "attenuation: " << lightVector[i].attenuation[0] << " " << lightVector[i].attenuation[1] << " " << lightVector[i].attenuation[2] << endl;
			cout << "angle: " << lightVector[i].angle << endl;
			cout << endl << "-----------------------------------" << endl;

		}

		for (size_t i = 0; i < particleVector.size(); i++)
		{
			cout << "translation: " << particleVector[i].translation[0] << " " << particleVector[i].translation[1] << " " << particleVector[i].translation[2] << endl;
			cout << "type: " << particleVector[i].type << endl;
			cout << endl << "-----------------------------------" << endl;

		}
	}



	void CompleteRoom::addRoom(Room* input)
	{
		room.type = input->room->type;
		room.special = input->room->special;
		room.theme = input->room->theme;
		room.probability = input->room->probability;
		room.size = input->room->size;
	}

	void CompleteRoom::addModel(Model* input)
	{
		AenIF::Model temp;
		temp.name = input->model->name;
		temp.mesh = input->model->mesh;
		temp.type = input->model->type;
		temp.sound = input->model->sound;
		temp.rigidBody = input->model->rigidBody;
		temp.rigidBodyType = input->model->rigidBodyType;

		for (int i = 0; i < 3; i++)
		{
			temp.translation[i] = input->model->translation[i];
			temp.rotation[i] = input->model->rotation[i];
			temp.scale[i] = input->model->scale[i];
		}

		modelVector.push_back(temp);
	}

	void CompleteRoom::addTexture(Texture* input)
	{
		AenIF::Texture temp;
		temp.name = input->texture->name;
		temp.textureType = input->texture->textureType;
		textureVector.push_back(temp);
	}

	void CompleteRoom::addMaterial(Material* input)
	{
		AenIF::Material temp;
		for (int i = 0; i < 3; i++)
		{
			temp.diffuse[i] = input->material->diffuse[i];
			temp.specular[i] = input->material->specular[i];
			temp.emissive[i] = input->material->emissive[i];
			temp.opacity[i] = input->material->opacity[i];

		}
		materialVector.push_back(temp);
	}

	void CompleteRoom::addLight(Light* input)
	{
		AenIF::Light temp;
		temp.type = input->light->type;
		temp.intensity = input->light->intensity;
		temp.range = input->light->range;
		temp.angle = input->light->angle;

		for (int i = 0; i < 3; i++)
		{
			temp.translation[i] = input->light->translation[i];
			temp.rotation[i] = input->light->rotation[i];
			temp.direction[i] = input->light->direction[i];
			temp.color[i] = input->light->color[i];
			temp.attenuation[i] = input->light->attenuation[i];
		}
		lightVector.push_back(temp);
	}

	void CompleteRoom::addParticle(Particle* input)
	{
		AenIF::Particle temp;
		temp.type = input->particle->type;
		for (int i = 0; i < 3; i++)
		{
			temp.translation[i] = input->particle->translation[i];
		}
		particleVector.push_back(temp);
	}

	AenIF::Room& CompleteRoom::GetRoom()
	{
		return room;
	}

	vector<AenIF::Model>& CompleteRoom::GetModelVector()
	{
		return modelVector;
	}

	vector<AenIF::Texture>& CompleteRoom::GetTextureVector()
	{
		return textureVector;
	}

	vector<AenIF::Material>& CompleteRoom::GetMaterialVector()
	{
		return materialVector;
	}

	vector<AenIF::Light>& CompleteRoom::GetLightVector()
	{
		return lightVector;
	}

	vector<AenIF::Particle>& CompleteRoom::GetParticleVector()
	{
		return particleVector;
	}
}