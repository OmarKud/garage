#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <GL/gl.h>

#include <vector>
#include <string>
#include <cstdint>

class Model
{
public:
    struct Vector { float x, y, z; };
    struct Color { float r, g, b, a; };

    struct Material
    {
        char   name[80]{};
        GLuint texId = 0;
        bool   textured = false;
        Color  ambient{ 0,0,0,1 };
        Color  diffuse{ 1,1,1,1 };
        Color  specular{ 0,0,0,1 };
    };

    struct MaterialFaces
    {
        std::vector<uint16_t> subFaces; // indices (3*n)
        int MatIndex = -1;              // index to Materials, -1 means no material
    };

    struct Object
    {
        char name[80]{};

        std::vector<float>    Vertexes;   // 3*numVerts
        std::vector<float>    Normals;    // 3*numVerts
        std::vector<float>    TexCoords;  // 2*numTex
        std::vector<uint16_t> Faces;      // 3*numFaces

        int numVerts = 0;
        int numFaces = 0;
        int numTexCoords = 0;

        bool textured = false;

        Vector pos{ 0,0,0 };
        Vector rot{ 0,0,0 };

        std::vector<MaterialFaces> MatFaces; // can be empty
    };

    Model();
    ~Model();

    bool Load(const char* filename);
    void Draw() const;

    // controls
    bool shownormals = false;
    bool lit = true;
    bool visible = true;

    Vector pos{ 0,0,0 };
    Vector rot{ 0,0,0 };
    float  scale = 1.0f;

private:
    // 3DS chunk header
    struct ChunkHeader {
        uint16_t id;
        uint32_t len;
    };

private:
    // parsing
    void Reset();

    void MainChunkProcessor(long length, long findex);
    void EditChunkProcessor(long length, long findex);

    void MaterialChunkProcessor(long length, long findex, int matindex);
    void MaterialNameChunkProcessor(long findex, int matindex);
    void AmbientColorChunkProcessor(long length, long findex, int matindex);
    void DiffuseColorChunkProcessor(long length, long findex, int matindex);
    void SpecularColorChunkProcessor(long length, long findex, int matindex);
    void TextureMapChunkProcessor(long length, long findex, int matindex);
    void MapNameChunkProcessor(long findex, int matindex);

    void ObjectChunkProcessor(long length, long findex, int objindex);
    void TriangularMeshChunkProcessor(long length, long findex, int objindex);
    void VertexListChunkProcessor(long findex, int objindex);
    void TexCoordsChunkProcessor(long findex, int objindex);
    void FacesDescriptionChunkProcessor(long length, long findex, int objindex);
    void FacesMaterialsListChunkProcessor(long findex, int objindex, int subfacesindex);

    void FloatColorChunkProcessor(long findex, Color& c);
    void IntColorChunkProcessor(long findex, Color& c);

    void CalculateNormals();

    bool ResolveTextureFullPath(const char* texName, char outFullPath[260]) const;

    bool ReadChunkHeader(ChunkHeader& h);
    bool SafeSeek(long offset, int origin);
    long FileTell() const;

private:
    FILE* bin3ds = nullptr;

    std::vector<Object>   Objects;
    std::vector<Material> Materials;

    std::string modelname;
    std::string pathDir; // directory of file

    int totalVerts = 0;
    int totalFaces = 0;
};
