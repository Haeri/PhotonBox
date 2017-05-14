#include <map>
#include "headers/ModelData.h"
#include "external/obj_parser/obj_parser.hpp"
#include "headers/FileHandler.h"
#include "headers/OBJLoader.h"

ModelData::ModelData(const std::string &fileName, bool flipT, bool flipZ)
	: _objLoader(new OBJLoader(this,
                            obj::obj_parser::parse_blank_lines_as_comment |
                            obj::obj_parser::triangulate_faces            |
                            obj::obj_parser::translate_negative_indices))
,   _flipT(flipT)
,   _flipZ(flipZ)
{
    load(fileName);
}

ModelData::ModelData(bool flipT, bool flipZ)
	: _objLoader(new OBJLoader(this,
                            obj::obj_parser::parse_blank_lines_as_comment |
                            obj::obj_parser::triangulate_faces            |
                            obj::obj_parser::translate_negative_indices))
,   _flipT(flipT)
,   _flipZ(flipZ)
{}

ModelData::~ModelData()
{
	delete _objLoader;
}

ModelData::GroupMap ModelData::getData() const
{
	return _objLoader->getData();
}

ModelData &ModelData::load(const std::string &fileName)
{
    std::ifstream file(bRenderer::getFilePath(fileName), std::ifstream::in);
	if (!_objLoader) _objLoader = new OBJLoader(this, obj::obj_parser::parse_blank_lines_as_comment | obj::obj_parser::triangulate_faces | obj::obj_parser::translate_negative_indices);
		_objLoader->load(file);
    return *this;
}

