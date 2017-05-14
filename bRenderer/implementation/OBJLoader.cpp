#include "headers/OBJLoader.h"
#include "headers/Configuration.h"
#include "headers/Logger.h"
#include "headers/FileHandler.h"

void OBJLoader::info_callback(const std::string& filename, std::size_t line_number, const std::string& message)
{
    std::cerr << filename << ":" << line_number << ": " << "info: " << message << std::endl;
}

void OBJLoader::warning_callback(const std::string& filename, std::size_t line_number, const std::string& message)
{
    std::cerr << filename << ":" << line_number << ": " << "warning: " << message << std::endl;
}

void OBJLoader::error_callback(const std::string& filename, std::size_t line_number, const std::string& message)
{
    std::cerr << filename << ":" << line_number << ": " << "error: " << message << std::endl;
}

void OBJLoader::geometric_vertex_callback(obj::float_type x, obj::float_type y, obj::float_type z)
{
    _vertices.push_back(vmml::Vector3f(x, y, z));
}

void OBJLoader::texture_vertex_callback(obj::float_type u, obj::float_type v)
{
    _texCoords.push_back(vmml::Vector2f(u, v));
}

void OBJLoader::vertex_normal_callback(obj::float_type x, obj::float_type y, obj::float_type z)
{
    _normals.push_back(vmml::Vector3f(x, y, z));
}

template< bool POSITION, bool TEX_COORD, bool NORMAL >
void OBJLoader::genVertex(const IndexData &d)
{
    Vertex v;
    
    if (POSITION)
    {
        v.position.x = _vertices[d.vertexIndex].x();
        v.position.y = _vertices[d.vertexIndex].y();
        v.position.z = _vertices[d.vertexIndex].z();
        if (_data->getFlipZ())
        {
            v.position.z *= -1.0f;
        }
    }
    else
    {
        v.position.x = 0;
        v.position.y = 0;
        v.position.z = 0;
    }
    
    if (TEX_COORD)
    {
        v.texCoord.s = _texCoords[d.texCoordsIndex].x();
        v.texCoord.t = _texCoords[d.texCoordsIndex].y();
        if (_data->getFlipT())
        {
            v.texCoord.t = 1.0f - v.texCoord.t;
        }
    }
    else
    {
        v.texCoord.s = 0;
        v.texCoord.t = 0;
    }
    
    if (NORMAL)
    {
        v.normal.x = _normals[d.normalIndex].x();
        v.normal.y = _normals[d.normalIndex].y();
        v.normal.z = _normals[d.normalIndex].z();
        _vertices[d.vertexIndex].normal = _normals[d.normalIndex];
    }
    else
    {
        v.normal.x = 0;
        v.normal.y = 0;
        v.normal.z = 0;
        _vertices[d.vertexIndex].normal = vmml::Vector3f::ZERO;
    }
    
    v.tangent.x = 0.0;
    v.tangent.y = 0.0;
    v.tangent.z = 0.0;
    v.bitangent.x = 0.0;
    v.bitangent.y = 0.0;
    v.bitangent.z = 0.0;
    
    _group->vboVertices.push_back(v);
    _group->vboIndices.push_back(_group->vboIndices.size());
}

template< bool NORMAL >
void OBJLoader::genFace(const IndexData &d1, const IndexData &d2, const IndexData &d3)
{
    FaceData f;
    f.v1 = d1.vertexIndex;
    f.v2 = d2.vertexIndex;
    f.v3 = d3.vertexIndex;
    f.normal = vmml::Vector3f::ZERO;
    
    _faces.push_back(f);
}

void OBJLoader::triangular_face_geometric_vertices_callback(obj::index_type v1, obj::index_type v2, obj::index_type v3)
{
    IndexData d1, d2, d3;
    d1.vertexIndex = v1 - 1;
    d2.vertexIndex = v2 - 1;
    d3.vertexIndex = v3 - 1;
    
    _group->indices.push_back(d1);
    _group->indices.push_back(d2);
    _group->indices.push_back(d3);
    
    genVertex< true, false, false >(d1);
    genVertex< true, false, false >(d2);
    genVertex< true, false, false >(d3);
    
    genFace< false >(d1, d2, d3);
}

void OBJLoader::triangular_face_geometric_vertices_texture_vertices_callback(const obj::index_2_tuple_type& v1_vt1, const obj::index_2_tuple_type& v2_vt2, const obj::index_2_tuple_type& v3_vt3)
{
    IndexData d1, d2, d3;
    d1.vertexIndex = boost::get<0>(v1_vt1) -1;
    d1.texCoordsIndex = boost::get<1>(v1_vt1) -1;
    d2.vertexIndex = boost::get<0>(v2_vt2) -1;
    d2.texCoordsIndex = boost::get<1>(v2_vt2) -1;
    d3.vertexIndex = boost::get<0>(v3_vt3) -1;
    d3.texCoordsIndex = boost::get<1>(v3_vt3) -1;
    
    _group->indices.push_back(d1);
    _group->indices.push_back(d2);
    _group->indices.push_back(d3);
    
    genVertex< true, true, false >(d1);
    genVertex< true, true, false >(d2);
    genVertex< true, true, false >(d3);
    
    genFace< false >(d1, d2, d3);
}

void OBJLoader::triangular_face_geometric_vertices_vertex_normals_callback(const obj::index_2_tuple_type& v1_vn1, const obj::index_2_tuple_type& v2_vn2, const obj::index_2_tuple_type& v3_vn3)
{
    IndexData d1, d2, d3;
    d1.vertexIndex = boost::get<0>(v1_vn1) -1;
    d1.normalIndex = boost::get<1>(v1_vn1) -1;
    d2.vertexIndex = boost::get<0>(v2_vn2) -1;
    d2.normalIndex = boost::get<1>(v2_vn2) -1;
    d3.vertexIndex = boost::get<0>(v3_vn3) -1;
    d3.normalIndex = boost::get<1>(v3_vn3) -1;
    
    _group->indices.push_back(d1);
    _group->indices.push_back(d2);
    _group->indices.push_back(d3);
    
    genVertex< true, false, true >(d1);
    genVertex< true, false, true >(d2);
    genVertex< true, false, true >(d3);
}

void OBJLoader::triangular_face_geometric_vertices_texture_vertices_vertex_normals_callback(const obj::index_3_tuple_type& v1_vt1_vn1, const obj::index_3_tuple_type& v2_vt2_vn2, const obj::index_3_tuple_type& v3_vt3_vn3)
{
    IndexData d1, d2, d3;
    d1.vertexIndex = boost::get<0>(v1_vt1_vn1) -1;
    d1.texCoordsIndex = boost::get<1>(v1_vt1_vn1) -1;
    d1.normalIndex = boost::get<2>(v1_vt1_vn1) -1;
    d2.vertexIndex = boost::get<0>(v2_vt2_vn2) -1;
    d2.texCoordsIndex = boost::get<1>(v2_vt2_vn2) -1;
    d2.normalIndex = boost::get<2>(v2_vt2_vn2) -1;
    d3.vertexIndex = boost::get<0>(v3_vt3_vn3) -1;
    d3.texCoordsIndex = boost::get<1>(v3_vt3_vn3) -1;
    d3.normalIndex = boost::get<2>(v3_vt3_vn3) -1;
    
    _group->indices.push_back(d1);
    _group->indices.push_back(d2);
    _group->indices.push_back(d3);
    
    genVertex< true, true, true >(d1);
    genVertex< true, true, true >(d2);
    genVertex< true, true, true >(d3);
}

void OBJLoader::polygonal_face_geometric_vertices_begin_callback(obj::index_type v1, obj::index_type v2, obj::index_type v3)
{
    // (*ostream_) << "f " << v1 << " " << v2 << " " << v3;
}

void OBJLoader::polygonal_face_geometric_vertices_vertex_callback(obj::index_type v)
{
    // (*ostream_) << " " << v;
}

void OBJLoader::polygonal_face_geometric_vertices_end_callback()
{
    // (*ostream_) << "\n";
}

void OBJLoader::polygonal_face_geometric_vertices_texture_vertices_begin_callback(const obj::index_2_tuple_type& v1_vt1, const obj::index_2_tuple_type& v2_vt2, const obj::index_2_tuple_type& v3_vt3)
{
    // (*ostream_) << "f " << boost::get<0>(v1_vt1) << "/" << boost::get<1>(v1_vt1) << " " << boost::get<0>(v2_vt2) << "/" << boost::get<1>(v2_vt2) << " " << boost::get<0>(v3_vt3) << "/" << boost::get<1>(v3_vt3);
}

void OBJLoader::polygonal_face_geometric_vertices_texture_vertices_vertex_callback(const obj::index_2_tuple_type& v_vt)
{
    // (*ostream_) << " " << boost::get<0>(v_vt) << "/" << boost::get<1>(v_vt);
}

void OBJLoader::polygonal_face_geometric_vertices_texture_vertices_end_callback()
{
    // (*ostream_) << "\n";
}

void OBJLoader::polygonal_face_geometric_vertices_vertex_normals_begin_callback(const obj::index_2_tuple_type& v1_vn1, const obj::index_2_tuple_type& v2_vn2, const obj::index_2_tuple_type& v3_vn3)
{
    // (*ostream_) << "f " << boost::get<0>(v1_vn1) << "//" << boost::get<1>(v1_vn1) << " " << boost::get<0>(v2_vn2) << "//" << boost::get<1>(v2_vn2) << " " << boost::get<0>(v3_vn3) << "//" << boost::get<1>(v3_vn3);
}

void OBJLoader::polygonal_face_geometric_vertices_vertex_normals_vertex_callback(const obj::index_2_tuple_type& v_vn)
{
    // (*ostream_) << " " << boost::get<0>(v_vn) << "//" << boost::get<1>(v_vn);
}

void OBJLoader::polygonal_face_geometric_vertices_vertex_normals_end_callback()
{
    // (*ostream_) << "\n";
}

void OBJLoader::polygonal_face_geometric_vertices_texture_vertices_vertex_normals_begin_callback(const obj::index_3_tuple_type& v1_vt1_vn1, const obj::index_3_tuple_type& v2_vt2_vn2, const obj::index_3_tuple_type& v3_vt3_vn3)
{
    // (*ostream_) << "f " << boost::get<0>(v1_vt1_vn1) << "/" << boost::get<1>(v1_vt1_vn1) << "/" << boost::get<2>(v1_vt1_vn1) << " " << boost::get<0>(v2_vt2_vn2) << "/" << boost::get<1>(v2_vt2_vn2) << "/" << boost::get<2>(v2_vt2_vn2) << " " << boost::get<0>(v3_vt3_vn3) << "/" << boost::get<1>(v3_vt3_vn3) << "/" << boost::get<2>(v3_vt3_vn3);
}

void OBJLoader::polygonal_face_geometric_vertices_texture_vertices_vertex_normals_vertex_callback(const obj::index_3_tuple_type& v_vt_vn)
{
    // (*ostream_) << " " << boost::get<0>(v_vt_vn) << "/" << boost::get<1>(v_vt_vn) << "/" << boost::get<2>(v_vt_vn);
}

void OBJLoader::polygonal_face_geometric_vertices_texture_vertices_vertex_normals_end_callback()
{
    // (*ostream_) << "\n";
}

void OBJLoader::group_name_callback(const std::string& group_name)
{
    bRenderer::log("g " + group_name);
    createGroup(group_name);
}

void OBJLoader::object_name_callback(const std::string& object_name)
{
    bRenderer::log("o " + object_name);
}

void OBJLoader::material_library_callback(const std::string& filename)
{
    bRenderer::log("mtllib " + filename);
    loadObjMtl(filename, _materials);
}

void OBJLoader::material_name_callback(const std::string& material_name)
{
    bRenderer::log("usemtl " + material_name);
    MaterialData &material = _materials[material_name];
    
    material.name = material_name;
    _group->materialData = material;
}

void OBJLoader::comment_callback(const std::string& comment)
{
    //bRenderer::log(comment);
}

bool OBJLoader::load(std::istream& istream)
{
    createGroup(bRenderer::DEFAULT_GROUP_NAME());
    obj::obj_parser obj_parser(_flags);
    
    std::string ifilename;
    obj_parser.info_callback(boost::bind(&OBJLoader::info_callback, this, ifilename, _1, _2));
    obj_parser.warning_callback(boost::bind(&OBJLoader::warning_callback, this, ifilename, _1, _2));
    obj_parser.error_callback(boost::bind(&OBJLoader::error_callback, this, ifilename, _1, _2));
    
    obj_parser.geometric_vertex_callback(boost::bind(&OBJLoader::geometric_vertex_callback, this, _1, _2, _3));
    obj_parser.texture_vertex_callback(boost::bind(&OBJLoader::texture_vertex_callback, this, _1, _2));
    obj_parser.vertex_normal_callback(boost::bind(&OBJLoader::vertex_normal_callback, this, _1, _2, _3));
    obj_parser.face_callbacks(
                              boost::bind(&OBJLoader::triangular_face_geometric_vertices_callback, this, _1, _2, _3),
                              boost::bind(&OBJLoader::triangular_face_geometric_vertices_texture_vertices_callback, this, _1, _2, _3),
                              boost::bind(&OBJLoader::triangular_face_geometric_vertices_vertex_normals_callback, this, _1, _2, _3),
                              boost::bind(&OBJLoader::triangular_face_geometric_vertices_texture_vertices_vertex_normals_callback, this, _1, _2, _3),
                              boost::bind(&OBJLoader::polygonal_face_geometric_vertices_begin_callback, this, _1, _2, _3),
                              boost::bind(&OBJLoader::polygonal_face_geometric_vertices_vertex_callback, this, _1),
                              boost::bind(&OBJLoader::polygonal_face_geometric_vertices_end_callback, this),
                              boost::bind(&OBJLoader::polygonal_face_geometric_vertices_texture_vertices_begin_callback, this, _1, _2, _3),
                              boost::bind(&OBJLoader::polygonal_face_geometric_vertices_texture_vertices_vertex_callback, this, _1),
                              boost::bind(&OBJLoader::polygonal_face_geometric_vertices_texture_vertices_end_callback, this),
                              boost::bind(&OBJLoader::polygonal_face_geometric_vertices_vertex_normals_begin_callback, this, _1, _2, _3),
                              boost::bind(&OBJLoader::polygonal_face_geometric_vertices_vertex_normals_vertex_callback, this, _1),
                              boost::bind(&OBJLoader::polygonal_face_geometric_vertices_vertex_normals_end_callback, this),
                              boost::bind(&OBJLoader::polygonal_face_geometric_vertices_texture_vertices_vertex_normals_begin_callback, this, _1, _2, _3),
                              boost::bind(&OBJLoader::polygonal_face_geometric_vertices_texture_vertices_vertex_normals_vertex_callback, this, _1),
                              boost::bind(&OBJLoader::polygonal_face_geometric_vertices_texture_vertices_vertex_normals_end_callback, this)
                              );
    obj_parser.group_name_callback(boost::bind(&OBJLoader::group_name_callback, this, _1));
    obj_parser.object_name_callback(boost::bind(&OBJLoader::object_name_callback, this, _1));
    obj_parser.material_library_callback(boost::bind(&OBJLoader::material_library_callback, this, _1));
    obj_parser.material_name_callback(boost::bind(&OBJLoader::material_name_callback, this, _1));
    obj_parser.comment_callback(boost::bind(&OBJLoader::comment_callback, this, _1));
    
    bool ret = obj_parser.parse(istream);
    
    // if there is not only vertex- but also face data, update normals accordingly
    if (_faces.size() > 0)
    {
        createFaceNormals();
        createVertexNormals();
        
        for (auto i = _groups.begin(); i != _groups.end(); ++i)
        {
            for (auto j = i->second->vboIndices.begin(); j != i->second->vboIndices.end(); ++j)
            {
                auto idx = i->second->indices[*j].vertexIndex;
                
                vmml::Vector3f cNormal = _vertices[idx].normal;
                Vector3 &normal = i->second->vboVertices[*j].normal;
                normal.x = cNormal.x();
                normal.y = cNormal.y();
                normal.z = cNormal.z();
                
                vmml::Vector3f cTangent = _vertices[idx].tangent;
                Vector3 &tangent = i->second->vboVertices[*j].tangent;
                tangent.x = cTangent.x();
                tangent.y = cTangent.y();
                tangent.z = cTangent.z();
                
                vmml::Vector3f cBitangent = _vertices[idx].bitangent;
                Vector3 &bitangent = i->second->vboVertices[*j].bitangent;
                bitangent.x = cBitangent.x();
                bitangent.y = cBitangent.y();
                bitangent.z = cBitangent.z();
            }
        }
    }
    
    // delete empty groups
    auto i = _groups.begin();
    while (i != _groups.end())
    {
        if (i->second->vboIndices.size() == 0 || i->second->vboVertices.size() == 0)
            _groups.erase(i++);
        else
            ++i;
    }
    
    return ret;
}

MaterialData OBJLoader::loadMaterial(const std::string &fileName, const std::string &materialName)
{
    MaterialMap materials;
    loadObjMtl(fileName, materials, materialName);
    
    return materials[materialName];
}

void OBJLoader::loadObjMtl(const std::string &fileName, MaterialMap &materials, const std::string &materialName)
{
    std::string file = bRenderer::getFilePath(fileName);
    std::ifstream inFile(file.c_str(), std::ifstream::in);
    
    std::string line, key;
    std::string matName;
    
    while (inFile.good() && !inFile.eof() && std::getline(inFile, line))
    {
        key = "";
        std::stringstream ss(line);
        ss >> key >> std::ws;
        
        if (key == bRenderer::WAVEFRONT_MATERIAL_NEWMTL())
        {
            ss >> matName >> std::ws;
        }
        else if (materialName.empty() || matName == materialName)
        {
            if (key == bRenderer::WAVEFRONT_MATERIAL_AMBIENT_COLOR())
            {
                vmml::Vector3f kA;
                ss >> kA[0] >> std::ws >> kA[1] >> std::ws >> kA[2] >> std::ws;
                materials[matName].vectors[bRenderer::WAVEFRONT_MATERIAL_AMBIENT_COLOR()] = kA;
            }
            else if (key == bRenderer::WAVEFRONT_MATERIAL_DIFFUSE_COLOR())
            {
                vmml::Vector3f kD;
                ss >> kD[0] >> std::ws >> kD[1] >> std::ws >> kD[2] >> std::ws;
                materials[matName].vectors[bRenderer::WAVEFRONT_MATERIAL_DIFFUSE_COLOR()] = kD;
            }
            else if (key == bRenderer::WAVEFRONT_MATERIAL_SPECULAR_COLOR())
            {
                vmml::Vector3f kS;
                ss >> kS[0] >> std::ws >> kS[1] >> std::ws >> kS[2] >> std::ws;
                materials[matName].vectors[bRenderer::WAVEFRONT_MATERIAL_SPECULAR_COLOR()] = kS;
            }
            else if (key == bRenderer::WAVEFRONT_MATERIAL_TRANSMISSION_FILTER())
            {
                vmml::Vector3f tF;
                ss >> tF[0] >> std::ws >> tF[1] >> std::ws >> tF[2] >> std::ws;
                materials[matName].vectors[bRenderer::WAVEFRONT_MATERIAL_TRANSMISSION_FILTER()] = tF;
            }
            else if (key == bRenderer::WAVEFRONT_MATERIAL_SPECULAR_EXPONENT())
            {
                GLfloat nS;
                ss >> nS >> std::ws;
                materials[matName].scalars[bRenderer::WAVEFRONT_MATERIAL_SPECULAR_EXPONENT()] = nS;
            }
            else if (key == bRenderer::WAVEFRONT_MATERIAL_OPTICAL_DENSITY())
            {
                GLfloat nI;
                ss >> nI >> std::ws;
                materials[matName].scalars[bRenderer::WAVEFRONT_MATERIAL_OPTICAL_DENSITY()] = nI;
            }
            else if (key == bRenderer::WAVEFRONT_MATERIAL_ILLUMINATION_MODEL())
            {
                GLfloat illum;
                ss >> illum >> std::ws;
                materials[matName].scalars[bRenderer::WAVEFRONT_MATERIAL_ILLUMINATION_MODEL()] = illum;
            }
            else if (key == bRenderer::WAVEFRONT_MATERIAL_DIFFUSE_MAP())
            {
                auto &mat = materials[matName].textures[bRenderer::DEFAULT_SHADER_UNIFORM_DIFFUSE_MAP()];
                ss >> mat >> std::ws;
            }
            else if (key == bRenderer::WAVEFRONT_MATERIAL_SPECULAR_MAP())
            {
                auto &mat = materials[matName].textures[bRenderer::DEFAULT_SHADER_UNIFORM_SPECULAR_MAP()];
                ss >> mat >> std::ws;
            }
            else if (key == bRenderer::WAVEFRONT_MATERIAL_NORMAL_MAP())
            {
                auto &mat = materials[matName].textures[bRenderer::DEFAULT_SHADER_UNIFORM_NORMAL_MAP()];
                ss >> mat >> std::ws;
            }
            else if (key == bRenderer::WAVEFRONT_MATERIAL_DISSOLVE())
            {
                GLfloat d;
                ss >> d >> std::ws;
                materials[matName].scalars[bRenderer::DEFAULT_SHADER_UNIFORM_TRANSPARENCY()] = d;
            }
            else if (key == bRenderer::WAVEFRONT_MATERIAL_REFLECTION())
            {
                std::string type;
                std::string mat;
                ss >> type >> std::ws;
                if (type == bRenderer::WAVEFRONT_MATERIAL_REFLECTION_TYPE())
                    ss >> type >> std::ws;
                ss >> mat >> std::ws;
                if (type == bRenderer::WAVEFRONT_MATERIAL_REFLECTION_TYPE_SPHERE())
                    materials[matName].textures[bRenderer::DEFAULT_SHADER_UNIFORM_SPHERE_MAP()] = mat;
                else
                    materials[matName].cubeTextures[type] = mat;
                
            }
        }
    }
}

void OBJLoader::createFaceNormals()
{
    for (int i = 0; i < _faces.size(); ++i)
    {
        // obtain reference to face
        FaceData &face = _faces[i];
        
        // obtain indices of this face's three vertices
        Index indexV1 = face.v1;
        Index indexV2 = face.v2;
        Index indexV3 = face.v3;
        
        // add this face's index to list of adjacent faces of each vertex
        _vertices[indexV1].faces.push_back(i);
        _vertices[indexV2].faces.push_back(i);
        _vertices[indexV3].faces.push_back(i);
        
        // obtain each of this face's vertex positions
        const vmml::Vector3f &p1 = _vertices[indexV1].position;
        const vmml::Vector3f &p2 = _vertices[indexV2].position;
        const vmml::Vector3f &p3 = _vertices[indexV3].position;
        
        // obtain each of this face's texture coordinates
        const vmml::Vector2f &t1 = _texCoords[indexV1];
        const vmml::Vector2f &t2 = _texCoords[indexV2];
        const vmml::Vector2f &t3 = _texCoords[indexV3];
        
        vmml::Vector3f e = p2 - p1;
        vmml::Vector3f f = p3 - p1;
        
        // calculate normal for this face
        vmml::Vector3f normal = e.cross(f);
        
        // set face normal
        face.normal = vmml::normalize(normal);
        
        // TODO: calculate tangent for face
        //        face.tangent = ... ;
        //        ...
        //        vmml::matrix<2, 3, float> mat ;
        //        mat.set_row(0, ... );
        //        mat.set_row(1, ... );
        //        ...
    }
}

void OBJLoader::createVertexNormals()
{
    for (VertexData &vertex : _vertices)
    {
        // only calculate vertex normal if not present
        vmml::Vector3f n = vmml::Vector3f::ZERO;
        if (vertex.normal.squared_length() < std::numeric_limits< float >::epsilon())
        {
            vmml::Vector3f normalSum = vmml::Vector3f::ZERO;
            for (Index &face : vertex.faces)
            {
                normalSum += _faces[face].normal;
            }
            n = vmml::normalize(normalSum);
        }
        else
        {
            n = vmml::normalize(vertex.normal);
        }
        
        vmml::Vector3f tangentSum = vmml::Vector3f::ZERO;
        for (Index &face : vertex.faces)
        {
            tangentSum += _faces[face].tangent;
        }
        
        vertex.normal = n;
        // TODO: calculate tangent for vertex, orthogonalize
        // vertex.tangent = ... ;
    }
}
