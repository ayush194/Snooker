//Code Credits: Calvin1602@github.com

#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>
#include <sstream>
#include <unordered_map>
#include <unistd.h>

#include <glm/glm.hpp>

#include "objloader.hpp"

// Very, VERY simple OBJ loader.
// Here is a short list of features a real function would provide : 
// - Binary files. Reading a model should be just a few memcpy's away, not parsing a file at runtime. In short : OBJ is not very great.
// - Animations & bones (includes bones weights)
// - Multiple UVs
// - All attributes should be optional, not "forced"
// - More stable. Change a line in the OBJ file and it crashes.
// - More secure. Change another line and you can inject code.
// - Loading from memory, stream, etc

bool parseMTL(
	const char* path, 
	std::unordered_map<std::string, unsigned int>& mtlname_idx_map, 
	std::vector<Material*>& mtls
) {
	FILE * mtlfile = fopen(path, "r");
	if (mtlfile == NULL){
		fprintf(stderr, "MTL File is missing!\n");
		getchar();
		return false;
	}
	//printf("debug : opened mtl file\n");
	Material* newmtl = NULL;
	while (1) {
		char line_header[128];
		// read the first word of the line
		int res = fscanf(mtlfile, "%s", line_header);
		if (res == EOF) {
			break; // EOF = End Of File. Quit the loop.
		}

		if (strcmp(line_header, "newmtl") == 0) {
			//create a new material;
			char mtlname[30];
			fscanf(mtlfile, "%s\n", mtlname);
			newmtl = (Material*)malloc(sizeof(Material));
			strcpy(newmtl->name, mtlname);
			mtls.push_back(newmtl);
			mtlname_idx_map[std::string(mtlname)] = mtls.size()-1;
		} else if (strcmp(line_header, "Ns") == 0) {
			fscanf(mtlfile, "%u\n", &newmtl->ns);
		} else if (strcmp(line_header, "Ka") == 0) {
			fscanf(mtlfile, "%f %f %f\n", &(newmtl->ka).x, &(newmtl->ka).y, &(newmtl->ka).z);
		} else if (strcmp(line_header, "Kd") == 0) {
			fscanf(mtlfile, "%f %f %f\n", &(newmtl->kd).x, &(newmtl->kd).y, &(newmtl->kd).z);
		} else if (strcmp(line_header, "Ks") == 0) {
			fscanf(mtlfile, "%f %f %f\n", &(newmtl->ks).x, &(newmtl->ks).y, &(newmtl->ks).z);
		} else if (strcmp(line_header, "d") == 0) {
			fscanf(mtlfile, "%u\n", &newmtl->d);
		} else if (strcmp(line_header, "illum") == 0) {
			fscanf(mtlfile, "%u\n", &newmtl->illum);
		} else if (strcmp(line_header, "map_Kd") == 0) {
			//convert relative path to absolute path
			getcwd(newmtl->map_kd, 100);
			uint size = strlen(newmtl->map_kd);
			newmtl->map_kd[size] = '/';
			strcpy(newmtl->map_kd+size+1, path);
			int i = strlen(newmtl->map_kd)-1;
			while (newmtl->map_kd[i] != '/') i--;
			fscanf(mtlfile, "%s\n", newmtl->map_kd+i+1);
		} else if (strcmp(line_header, "map_Ks") == 0) {
			//convert relative path to absolute path
			getcwd(newmtl->map_ks, 100);
			uint size = strlen(newmtl->map_ks);
			newmtl->map_ks[size] = '/';
			strcpy(newmtl->map_ks+size+1, path);
			int i = strlen(newmtl->map_ks)-1;
			while (newmtl->map_ks[i] != '/') i--;
			fscanf(mtlfile, "%s\n", newmtl->map_ks+i+1);
		} else {
			// Probably a comment, eat up the rest of the line
			char stupid_buffer[1000];
			fgets(stupid_buffer, 1000, mtlfile);
		}
	}
	return true;
}

void splitByDelim(char* s, char d, int* a, int* b, int* c) {
	std::stringstream ss(s);
	std::string str;
	int tmp[3] = {-1, -1, -1};
	for(int i = 0; getline(ss, str, d); i++) {
		if (!str.empty()) {
			tmp[i] = stoi(str);
		}
	}
	*a = tmp[0];
	*b = tmp[1];
	*c = tmp[2];
}

bool loadOBJ(
	const char * path, 
	std::vector<Vertex>& out_vertices,
	std::vector<Material*>& out_mtls
){
	printf("Loading OBJ file %s...\n", path);

	std::vector<glm::vec3> temp_positions; 
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;
	std::unordered_map<std::string, unsigned int> temp_mtlname_idx_map;
	int mtl_idx = -1;

	FILE * file = fopen(path, "r");
	if (file == NULL) {
		fprintf(stderr, "Unable to open file! Path incorrect!\n");
		getchar();
		return false;
	}

	while (1) {

		char line_header[128];
		// read the first word of the line
		int res = fscanf(file, "%s", line_header);
		if (res == EOF) {
			break; // EOF = End Of File. Quit the loop.
		}

		// else : parse lineHeader
		
		if (strcmp(line_header, "mtllib") == 0) {
			char relative_mtlfilepath[50];
			fscanf(file, "%s\n", relative_mtlfilepath);
			char mtlfilepath[strlen(path) + strlen(relative_mtlfilepath)];
			char* lastslash;
			for(int i = 0; i < strlen(path); i++) {
				mtlfilepath[i] = path[i];
				if (path[i] == '/') lastslash = mtlfilepath + i;
			}
			strcpy(lastslash + 1, relative_mtlfilepath);
			if (!parseMTL(mtlfilepath, temp_mtlname_idx_map, out_mtls)) {
				fprintf(stderr, "File can't be read by our simple parser :-( Try exporting with other options\n");
				getchar();
				return false;
			}
		} else if (strcmp(line_header, "usemtl") == 0) {
			//material changed
			char mtlname[30];
			fscanf(file, "%s\n", mtlname);
			mtl_idx = temp_mtlname_idx_map[std::string(mtlname)];
		} else if (strcmp(line_header, "v") == 0) {
			glm::vec3 position;
			fscanf(file, "%f %f %f\n", &position.x, &position.y, &position.z );
			temp_positions.push_back(position);
		} else if (strcmp(line_header, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y );
			uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
			temp_uvs.push_back(uv);
		} else if (strcmp(line_header, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			temp_normals.push_back(normal);
		} else if (strcmp( line_header, "f") == 0) {
			int position_index[3], uv_index[3], normal_index[3];
			char tmp1[20], tmp2[20], tmp3[20];
			int matches = fscanf(file, "%s %s %s\n", tmp1, tmp2, tmp3);
			if (matches != 3){
				fprintf(stderr, "File can't be read by our simple parser :-( Try exporting with other options\n");
				fclose(file);
				return false;
			}
			splitByDelim(tmp1, '/', &position_index[0], &uv_index[0], &normal_index[0]);
			splitByDelim(tmp2, '/', &position_index[1], &uv_index[1], &normal_index[1]);
			splitByDelim(tmp3, '/', &position_index[2], &uv_index[2], &normal_index[2]);

			for(int i = 0; i < 3; i++) {
				if (position_index[i] == -1) {
					fprintf(stderr, "Error in parsing! Position of Vertex missing. :-(\n");
					fclose(file);
					return false;
				}

				//Vertex v = {temp_positions[position_index[i]-1], glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0, 0.0), mtl_idx};
				Vertex v(temp_positions[position_index[i]-1], glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0, 0.0), mtl_idx);
				if (uv_index[i] >= 0) {
					v.uv = temp_uvs[uv_index[i]-1];
				}
				if (normal_index[i] >= 0) {
					v.normal = temp_normals[normal_index[i]-1];
				}
				out_vertices.push_back(v);
			}
		}else{
			// Probably a comment, eat up the rest of the line
			char stupid_buffer[1000];
			fgets(stupid_buffer, 1000, file);
		}

	}

	fclose(file);
	return true;
}

