#include "graphics_common/wavefront.hpp"
#include "graphics_common/graphics.hpp"
#include "game.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

struct vertex_pos_t {
	float x, y, z;
};

struct vertex_texcoord_t {
	float u, v;
};

struct vertex_normal_t {
	float x, y, z;
};

struct face_t {
	unsigned short posIndices[3];
	unsigned short texCoordIndices[3];
	unsigned short normalIndices[3];
};

static std::vector<std::string> explode(const char* str, char delim, unsigned int predictedNum = 0) {
	std::vector<std::string> results;
	results.reserve(predictedNum);

	size_t len = strlen(str);
	std::string word = "";

	for (size_t i = 0; i < len; ++i) {
		char c = str[i];

		if (c == delim) {
			if (!word.empty()) {
				results.push_back(word);
				word.clear();
			}
		}
		else {
			word += c;
		}
	}

	if (!word.empty()) {
		results.push_back(word);
	}

	return results;
}

S_CBuffer OBJ::Load(const char *source) {
	std::string src(source);
	std::istringstream ss(src);
	std::string line;

	std::vector<vertex_pos_t> positions;
	std::vector<vertex_texcoord_t> texCoords;
	std::vector<vertex_normal_t> normals;
	std::vector<face_t> faces;

	while (std::getline(ss, line)) {
		std::vector<std::string> parts = explode(line.c_str(), ' ', 3);
		std::string type = parts[0];

		if (type == "v") {
			float val1 = std::stof(parts[1]);
			float val2 = std::stof(parts[2]);
			float val3 = std::stof(parts[3]);
			positions.push_back({ val1, val2, val3 });
		}
		else if (type == "vt") {
			float val1 = std::stof(parts[1]);
			float val2 = std::stof(parts[2]);
			texCoords.push_back({ val1, val2 });
		}
		else if (type == "vn") {
			float val1 = std::stof(parts[1]);
			float val2 = std::stof(parts[2]);
			float val3 = std::stof(parts[3]);
			normals.push_back({ val1, val2, val3 });
		}
		else if (type == "f") {
			face_t face;

			for (int i = 0; i < 3; ++i) {
				std::string *vert = &parts[i + 1];
				std::vector<std::string> values = explode(vert->c_str(), '/', 3);

				face.posIndices[i] = (unsigned short)std::stoi(values[0]);
				face.texCoordIndices[i] = (unsigned short)std::stoi(values[1]);
				face.normalIndices[i] = (unsigned short)std::stoi(values[2]);
			}

			faces.push_back(face);
		}
	}

	std::vector<vertex_t> vertices;

	for (face_t face : faces) {
		for (int i = 0; i < 3; ++i) {
			vertex_pos_t pos = positions[face.posIndices[i] - 1];
			vertex_texcoord_t texCoord = texCoords[face.texCoordIndices[i] - 1];
			vertex_normal_t normal = normals[face.normalIndices[i] - 1];

			vertices.push_back({
				pos.x, pos.y, pos.z, 1.0f,
				texCoord.u, texCoord.v,
				normal.x, normal.y, normal.z
			});
		}
	}

	S_CBuffer buf = GFX->CreateBuffer(BufferType::VERTEX_BUFFER);
	buf->Orphan(vertices.size() * sizeof(vertex_t), sizeof(vertex_t), &vertices[0]);
	return buf;
}

S_CBuffer OBJ::LoadFromFile(const char *file) {
	std::ifstream in(file);
	std::string line;
	std::string source;

	while (std::getline(in, line)) {
		source += line + "\n";
	}

	return Load(source.c_str());
}