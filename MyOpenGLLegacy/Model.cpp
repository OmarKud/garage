#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <GL/gl.h>

#include "Model.h"
#include "Texture.h"

#include <cstdio>
#include <cstring>
#include <cmath>
#include <iostream>

// 3DS chunks
#define MAIN3DS         0x4D4D
#define EDIT3DS         0x3D3D

#define OBJECT          0x4000
#define TRIG_MESH       0x4100
#define VERT_LIST       0x4110
#define FACE_DESC       0x4120
#define FACE_MAT        0x4130
#define TEX_VERTS       0x4140

#define MATERIAL        0xAFFF
#define MAT_NAME        0xA000
#define MAT_AMBIENT     0xA010
#define MAT_DIFFUSE     0xA020
#define MAT_SPECULAR    0xA030
#define MAT_TEXMAP      0xA200
#define MAT_MAPNAME     0xA300

#define COLOR_RGB       0x0010
#define COLOR_TRU       0x0011
#define COLOR_TRUG      0x0012
#define COLOR_RGBG      0x0013

static bool FileExistsA_(const char* p)
{
    DWORD a = GetFileAttributesA(p);
    return (a != INVALID_FILE_ATTRIBUTES) && !(a & FILE_ATTRIBUTE_DIRECTORY);
}

Model::Model() { Reset(); }
Model::~Model() { Reset(); }

void Model::Reset()
{
    // Close file if open
    if (bin3ds) {
        fclose(bin3ds);
        bin3ds = nullptr;
    }

    // Free CPU memory (vectors auto-free)
    Objects.clear();
    Materials.clear();

    modelname.clear();
    pathDir.clear();

    totalFaces = 0;
    totalVerts = 0;

    // leave user-controlled flags as-is? better keep defaults:
    // (but if you want keep previous, remove these 3 lines)
    shownormals = false;
    lit = true;
    visible = true;

    pos = { 0,0,0 };
    rot = { 0,0,0 };
    scale = 1.0f;
}

bool Model::SafeSeek(long offset, int origin)
{
    if (!bin3ds) return false;
    return fseek(bin3ds, offset, origin) == 0;
}

long Model::FileTell() const
{
    if (!bin3ds) return 0;
    return ftell(bin3ds);
}

bool Model::ReadChunkHeader(ChunkHeader& h)
{
    if (!bin3ds) return false;
    size_t r1 = fread(&h.id, sizeof(h.id), 1, bin3ds);
    size_t r2 = fread(&h.len, sizeof(h.len), 1, bin3ds);
    return (r1 == 1 && r2 == 1 && h.len >= 6);
}

static void ReadCString80(FILE* f, char out80[80])
{
    // Reads null-terminated string into out80 (ensures null-terminated)
    for (int i = 0; i < 79; i++) {
        int c = fgetc(f);
        if (c == EOF) { out80[i] = 0; return; }
        out80[i] = (char)c;
        if (out80[i] == 0) return;
    }
    out80[79] = 0;

    // If string longer, consume until null to keep file pointer consistent
    int c;
    do { c = fgetc(f); } while (c != EOF && c != 0);
}

bool Model::Load(const char* filename)
{
    if (!filename || !filename[0]) return false;

    // reset previous data safely
    Reset();

    modelname = filename;

    // extract directory
    {
        std::string s = filename;
        size_t p1 = s.find_last_of('/');
        size_t p2 = s.find_last_of('\\');
        size_t p = std::string::npos;
        if (p1 != std::string::npos && p2 != std::string::npos) p = (p1 > p2 ? p1 : p2);
        else if (p1 != std::string::npos) p = p1;
        else p = p2;

        if (p != std::string::npos) pathDir = s.substr(0, p + 1);
        else pathDir.clear();
    }

    bin3ds = fopen(filename, "rb");
    if (!bin3ds) {
        std::cout << "Failed to open 3ds file: " << filename << "\n";
        return false;
    }

    // Read main header
    ChunkHeader main1{};
    if (!ReadChunkHeader(main1)) {
        std::cout << "Invalid 3DS header\n";
        Reset();
        return false;
    }

    if (main1.id != MAIN3DS) {
        std::cout << "Not a valid 3DS file (MAIN3DS missing)\n";
        Reset();
        return false;
    }

    // process
    MainChunkProcessor((long)main1.len, FileTell());

    // close file
    fclose(bin3ds);
    bin3ds = nullptr;

    // build normals
    CalculateNormals();

    // totals
    totalFaces = 0;
    totalVerts = 0;
    for (auto& o : Objects) {
        totalVerts += o.numVerts;
        totalFaces += (o.numFaces / 3);
    }

    std::cout << "3DS Loaded: objects=" << (int)Objects.size()
        << " materials=" << (int)Materials.size()
        << " verts=" << totalVerts
        << " faces=" << totalFaces << "\n";

    return !Objects.empty();
}

void Model::Draw() const
{
    if (!visible) return;
    if (Objects.empty()) return;

    glPushMatrix();

    glTranslatef(pos.x, pos.y, pos.z);
    glRotatef(rot.x, 1.f, 0.f, 0.f);
    glRotatef(rot.y, 0.f, 1.f, 0.f);
    glRotatef(rot.z, 0.f, 0.f, 1.f);
    glScalef(scale, scale, scale);

    glColor3f(1.f, 1.f, 1.f);

    for (const auto& obj : Objects)
    {
        if (obj.Vertexes.empty() || obj.Faces.empty()) continue;

        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, obj.Vertexes.data());

        if (lit && !obj.Normals.empty()) {
            glEnableClientState(GL_NORMAL_ARRAY);
            glNormalPointer(GL_FLOAT, 0, obj.Normals.data());
        }
        else {
            glDisableClientState(GL_NORMAL_ARRAY);
        }

        const bool hasUV = obj.textured && !obj.TexCoords.empty();
        if (hasUV) {
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glTexCoordPointer(2, GL_FLOAT, 0, obj.TexCoords.data());
        }
        else {
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        }

        // ✅ fallback if no material groups
        if (obj.MatFaces.empty())
        {
            glDisable(GL_TEXTURE_2D);
            glDrawElements(GL_TRIANGLES, (GLsizei)obj.Faces.size(), GL_UNSIGNED_SHORT, obj.Faces.data());
        }
        else
        {
            for (const auto& mf : obj.MatFaces)
            {
                if (mf.subFaces.empty()) continue;

                GLuint tex = 0;
                if (mf.MatIndex >= 0 && mf.MatIndex < (int)Materials.size())
                    tex = Materials[mf.MatIndex].texId;

                if (tex != 0 && hasUV) {
                    glEnable(GL_TEXTURE_2D);
                    glBindTexture(GL_TEXTURE_2D, tex);
                }
                else {
                    glDisable(GL_TEXTURE_2D);
                }

                glPushMatrix();
                glTranslatef(obj.pos.x, obj.pos.y, obj.pos.z);
                glRotatef(obj.rot.z, 0.f, 0.f, 1.f);
                glRotatef(obj.rot.y, 0.f, 1.f, 0.f);
                glRotatef(obj.rot.x, 1.f, 0.f, 0.f);

                glDrawElements(GL_TRIANGLES, (GLsizei)mf.subFaces.size(), GL_UNSIGNED_SHORT, mf.subFaces.data());

                glPopMatrix();
            }
        }

        // normals debug
        if (shownormals && !obj.Normals.empty())
        {
            glDisable(GL_TEXTURE_2D);
            if (lit) glDisable(GL_LIGHTING);

            glColor3f(0.f, 0.f, 1.f);
            for (int k = 0; k < obj.numVerts * 3; k += 3)
            {
                glBegin(GL_LINES);
                glVertex3f(obj.Vertexes[k], obj.Vertexes[k + 1], obj.Vertexes[k + 2]);
                glVertex3f(obj.Vertexes[k] + obj.Normals[k],
                    obj.Vertexes[k + 1] + obj.Normals[k + 1],
                    obj.Vertexes[k + 2] + obj.Normals[k + 2]);
                glEnd();
            }
            glColor3f(1.f, 1.f, 1.f);

            if (lit) glEnable(GL_LIGHTING);
        }
    }

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}



// ---------------- Parsing ----------------

void Model::MainChunkProcessor(long length, long findex)
{
    if (!bin3ds) return;

    ChunkHeader h{};
    SafeSeek(findex, SEEK_SET);

    long end = findex + length - 6;
    while (FileTell() < end)
    {
        long chunkStart = FileTell();

        if (!ReadChunkHeader(h)) break;

        // sanity: avoid infinite loops on bad files
        if (h.len < 6 || chunkStart + (long)h.len > end + 6) {
            SafeSeek(end, SEEK_SET);
            break;
        }

        switch (h.id)
        {
        case EDIT3DS:
            EditChunkProcessor((long)h.len, FileTell());
            break;
        default:
            break;
        }

        // skip chunk data
        SafeSeek(chunkStart + (long)h.len, SEEK_SET);
    }

    SafeSeek(findex, SEEK_SET);
}

void Model::EditChunkProcessor(long length, long findex)
{
    if (!bin3ds) return;

    // First pass: count
    int countObjects = 0;
    int countMaterials = 0;

    ChunkHeader h{};
    SafeSeek(findex, SEEK_SET);

    long end = findex + length - 6;
    while (FileTell() < end)
    {
        long chunkStart = FileTell();
        if (!ReadChunkHeader(h)) break;
        if (h.len < 6 || chunkStart + (long)h.len > end + 6) { SafeSeek(end, SEEK_SET); break; }

        if (h.id == OBJECT)   countObjects++;
        if (h.id == MATERIAL) countMaterials++;

        SafeSeek(chunkStart + (long)h.len, SEEK_SET);
    }

    // allocate vectors
    Objects.clear();
    Materials.clear();
    Objects.resize(countObjects);
    Materials.resize(countMaterials);

    // init materials defaults
    for (auto& m : Materials) {
        memset(m.name, 0, sizeof(m.name));
        m.texId = 0;
        m.textured = false;
        m.ambient = { 0,0,0,1 };
        m.diffuse = { 1,1,1,1 };
        m.specular = { 0,0,0,1 };
    }

    // second pass: load
    SafeSeek(findex, SEEK_SET);

    int oi = 0;
    int mi = 0;

    while (FileTell() < end)
    {
        long chunkStart = FileTell();
        if (!ReadChunkHeader(h)) break;
        if (h.len < 6 || chunkStart + (long)h.len > end + 6) { SafeSeek(end, SEEK_SET); break; }

        if (h.id == MATERIAL && mi < (int)Materials.size())
            MaterialChunkProcessor((long)h.len, FileTell(), mi++);

        if (h.id == OBJECT && oi < (int)Objects.size())
            ObjectChunkProcessor((long)h.len, FileTell(), oi++);

        SafeSeek(chunkStart + (long)h.len, SEEK_SET);
    }

    SafeSeek(findex, SEEK_SET);
}

void Model::MaterialChunkProcessor(long length, long findex, int matindex)
{
    if (!bin3ds) return;

    ChunkHeader h{};
    SafeSeek(findex, SEEK_SET);

    long end = findex + length - 6;
    while (FileTell() < end)
    {
        long chunkStart = FileTell();
        if (!ReadChunkHeader(h)) break;
        if (h.len < 6 || chunkStart + (long)h.len > end + 6) { SafeSeek(end, SEEK_SET); break; }

        switch (h.id)
        {
        case MAT_NAME:     MaterialNameChunkProcessor(FileTell(), matindex); break;
        case MAT_AMBIENT:  AmbientColorChunkProcessor((long)h.len, FileTell(), matindex); break;
        case MAT_DIFFUSE:  DiffuseColorChunkProcessor((long)h.len, FileTell(), matindex); break;
        case MAT_SPECULAR: SpecularColorChunkProcessor((long)h.len, FileTell(), matindex); break;
        case MAT_TEXMAP:   TextureMapChunkProcessor((long)h.len, FileTell(), matindex); break;
        default: break;
        }

        SafeSeek(chunkStart + (long)h.len, SEEK_SET);
    }
}

void Model::MaterialNameChunkProcessor(long findex, int matindex)
{
    SafeSeek(findex, SEEK_SET);
    ReadCString80(bin3ds, Materials[matindex].name);
}

void Model::AmbientColorChunkProcessor(long length, long findex, int matindex)
{
    ChunkHeader h{};
    SafeSeek(findex, SEEK_SET);

    long end = findex + length - 6;
    while (FileTell() < end)
    {
        long chunkStart = FileTell();
        if (!ReadChunkHeader(h)) break;
        if (h.len < 6 || chunkStart + (long)h.len > end + 6) { SafeSeek(end, SEEK_SET); break; }

        if (h.id == COLOR_RGB || h.id == COLOR_RGBG) FloatColorChunkProcessor(FileTell(), Materials[matindex].ambient);
        if (h.id == COLOR_TRU || h.id == COLOR_TRUG) IntColorChunkProcessor(FileTell(), Materials[matindex].ambient);

        SafeSeek(chunkStart + (long)h.len, SEEK_SET);
    }
}

void Model::DiffuseColorChunkProcessor(long length, long findex, int matindex)
{
    ChunkHeader h{};
    SafeSeek(findex, SEEK_SET);

    long end = findex + length - 6;
    while (FileTell() < end)
    {
        long chunkStart = FileTell();
        if (!ReadChunkHeader(h)) break;
        if (h.len < 6 || chunkStart + (long)h.len > end + 6) { SafeSeek(end, SEEK_SET); break; }

        if (h.id == COLOR_RGB || h.id == COLOR_RGBG) FloatColorChunkProcessor(FileTell(), Materials[matindex].diffuse);
        if (h.id == COLOR_TRU || h.id == COLOR_TRUG) IntColorChunkProcessor(FileTell(), Materials[matindex].diffuse);

        SafeSeek(chunkStart + (long)h.len, SEEK_SET);
    }
}

void Model::SpecularColorChunkProcessor(long length, long findex, int matindex)
{
    ChunkHeader h{};
    SafeSeek(findex, SEEK_SET);

    long end = findex + length - 6;
    while (FileTell() < end)
    {
        long chunkStart = FileTell();
        if (!ReadChunkHeader(h)) break;
        if (h.len < 6 || chunkStart + (long)h.len > end + 6) { SafeSeek(end, SEEK_SET); break; }

        if (h.id == COLOR_RGB || h.id == COLOR_RGBG) FloatColorChunkProcessor(FileTell(), Materials[matindex].specular);
        if (h.id == COLOR_TRU || h.id == COLOR_TRUG) IntColorChunkProcessor(FileTell(), Materials[matindex].specular);

        SafeSeek(chunkStart + (long)h.len, SEEK_SET);
    }
}

void Model::FloatColorChunkProcessor(long findex, Color& c)
{
    SafeSeek(findex, SEEK_SET);
    float r = 1, g = 1, b = 1;
    fread(&r, sizeof(r), 1, bin3ds);
    fread(&g, sizeof(g), 1, bin3ds);
    fread(&b, sizeof(b), 1, bin3ds);
    c = { r,g,b,1.0f };
}

void Model::IntColorChunkProcessor(long findex, Color& c)
{
    SafeSeek(findex, SEEK_SET);
    unsigned char r = 255, g = 255, b = 255;
    fread(&r, sizeof(r), 1, bin3ds);
    fread(&g, sizeof(g), 1, bin3ds);
    fread(&b, sizeof(b), 1, bin3ds);
    c = { r / 255.f, g / 255.f, b / 255.f, 1.0f };
}

void Model::TextureMapChunkProcessor(long length, long findex, int matindex)
{
    ChunkHeader h{};
    SafeSeek(findex, SEEK_SET);

    long end = findex + length - 6;
    while (FileTell() < end)
    {
        long chunkStart = FileTell();
        if (!ReadChunkHeader(h)) break;
        if (h.len < 6 || chunkStart + (long)h.len > end + 6) { SafeSeek(end, SEEK_SET); break; }

        if (h.id == MAT_MAPNAME)
            MapNameChunkProcessor(FileTell(), matindex);

        SafeSeek(chunkStart + (long)h.len, SEEK_SET);
    }
}

void Model::MapNameChunkProcessor(long findex, int matindex)
{
    SafeSeek(findex, SEEK_SET);

    char texName[80]{};
    ReadCString80(bin3ds, texName);

    char full[260]{};
    if (ResolveTextureFullPath(texName, full))
        Materials[matindex].texId = LoadTexture2D(full);
    else {
        // try raw name
        Materials[matindex].texId = LoadTexture2D(texName);
    }

    Materials[matindex].textured = (Materials[matindex].texId != 0);
}

void Model::ObjectChunkProcessor(long length, long findex, int objindex)
{
    ChunkHeader h{};
    SafeSeek(findex, SEEK_SET);

    ReadCString80(bin3ds, Objects[objindex].name);

    long end = findex + length - 6;
    while (FileTell() < end)
    {
        long chunkStart = FileTell();
        if (!ReadChunkHeader(h)) break;
        if (h.len < 6 || chunkStart + (long)h.len > end + 6) { SafeSeek(end, SEEK_SET); break; }

        if (h.id == TRIG_MESH)
            TriangularMeshChunkProcessor((long)h.len, FileTell(), objindex);

        SafeSeek(chunkStart + (long)h.len, SEEK_SET);
    }
}

void Model::TriangularMeshChunkProcessor(long length, long findex, int objindex)
{
    ChunkHeader h{};
    SafeSeek(findex, SEEK_SET);

    long end = findex + length - 6;

    // pass 1: verts/uv
    while (FileTell() < end)
    {
        long chunkStart = FileTell();
        if (!ReadChunkHeader(h)) break;
        if (h.len < 6 || chunkStart + (long)h.len > end + 6) { SafeSeek(end, SEEK_SET); break; }

        if (h.id == VERT_LIST) VertexListChunkProcessor(FileTell(), objindex);
        if (h.id == TEX_VERTS) { TexCoordsChunkProcessor(FileTell(), objindex); Objects[objindex].textured = true; }

        SafeSeek(chunkStart + (long)h.len, SEEK_SET);
    }

    // pass 2: faces/material groups
    SafeSeek(findex, SEEK_SET);
    while (FileTell() < end)
    {
        long chunkStart = FileTell();
        if (!ReadChunkHeader(h)) break;
        if (h.len < 6 || chunkStart + (long)h.len > end + 6) { SafeSeek(end, SEEK_SET); break; }

        if (h.id == FACE_DESC) FacesDescriptionChunkProcessor((long)h.len, FileTell(), objindex);

        SafeSeek(chunkStart + (long)h.len, SEEK_SET);
    }
}

void Model::VertexListChunkProcessor(long findex, int objindex)
{
    SafeSeek(findex, SEEK_SET);

    uint16_t numVerts = 0;
    fread(&numVerts, sizeof(numVerts), 1, bin3ds);

    // sanity
    if (numVerts == 0 || numVerts > 200000) return;

    auto& o = Objects[objindex];
    o.numVerts = (int)numVerts;
    o.Vertexes.assign(numVerts * 3, 0.f);
    o.Normals.assign(numVerts * 3, 0.f);

    for (int i = 0; i < numVerts * 3; i += 3)
    {
        float x = 0, y = 0, z = 0;
        fread(&x, sizeof(float), 1, bin3ds);
        fread(&z, sizeof(float), 1, bin3ds); // swap
        fread(&y, sizeof(float), 1, bin3ds);

        z = -z;

        o.Vertexes[i] = x;
        o.Vertexes[i + 1] = y;
        o.Vertexes[i + 2] = z;
    }
}

void Model::TexCoordsChunkProcessor(long findex, int objindex)
{
    SafeSeek(findex, SEEK_SET);

    uint16_t numCoords = 0;
    fread(&numCoords, sizeof(numCoords), 1, bin3ds);

    if (numCoords == 0 || numCoords > 200000) return;

    auto& o = Objects[objindex];
    o.numTexCoords = (int)numCoords;
    o.TexCoords.assign(numCoords * 2, 0.f);

    for (int i = 0; i < numCoords * 2; i += 2)
    {
        float u = 0, v = 0;
        fread(&u, sizeof(float), 1, bin3ds);
        fread(&v, sizeof(float), 1, bin3ds);
        o.TexCoords[i] = u;
        o.TexCoords[i + 1] = v;
    }
}

void Model::FacesDescriptionChunkProcessor(long length, long findex, int objindex)
{
    ChunkHeader h{};
    SafeSeek(findex, SEEK_SET);

    uint16_t numFaces = 0;
    fread(&numFaces, sizeof(numFaces), 1, bin3ds);

    if (numFaces == 0 || numFaces > 200000) return;

    auto& o = Objects[objindex];
    o.numFaces = (int)numFaces * 3;
    o.Faces.assign((size_t)numFaces * 3, 0);

    // Need vertices to compute normals
    if (o.Vertexes.empty()) {
        // still read faces but no normals
        for (int i = 0; i < numFaces * 3; i += 3)
        {
            uint16_t a, b, c, flags;
            fread(&a, sizeof(a), 1, bin3ds);
            fread(&b, sizeof(b), 1, bin3ds);
            fread(&c, sizeof(c), 1, bin3ds);
            fread(&flags, sizeof(flags), 1, bin3ds);

            o.Faces[i] = a; o.Faces[i + 1] = b; o.Faces[i + 2] = c;
        }
    }
    else {
        for (int i = 0; i < numFaces * 3; i += 3)
        {
            uint16_t a, b, c, flags;
            fread(&a, sizeof(a), 1, bin3ds);
            fread(&b, sizeof(b), 1, bin3ds);
            fread(&c, sizeof(c), 1, bin3ds);
            fread(&flags, sizeof(flags), 1, bin3ds);

            o.Faces[i] = a; o.Faces[i + 1] = b; o.Faces[i + 2] = c;

            // accumulate face normal
            const float ax = o.Vertexes[a * 3 + 0], ay = o.Vertexes[a * 3 + 1], az = o.Vertexes[a * 3 + 2];
            const float bx = o.Vertexes[b * 3 + 0], by = o.Vertexes[b * 3 + 1], bz = o.Vertexes[b * 3 + 2];
            const float cx = o.Vertexes[c * 3 + 0], cy = o.Vertexes[c * 3 + 1], cz = o.Vertexes[c * 3 + 2];

            const float ux = bx - cx, uy = by - cy, uz = bz - cz;
            const float vx = bx - ax, vy = by - ay, vz = bz - az;

            const float nx = (uy * vz - uz * vy);
            const float ny = (uz * vx - ux * vz);
            const float nz = (ux * vy - uy * vx);

            o.Normals[a * 3 + 0] += nx; o.Normals[a * 3 + 1] += ny; o.Normals[a * 3 + 2] += nz;
            o.Normals[b * 3 + 0] += nx; o.Normals[b * 3 + 1] += ny; o.Normals[b * 3 + 2] += nz;
            o.Normals[c * 3 + 0] += nx; o.Normals[c * 3 + 1] += ny; o.Normals[c * 3 + 2] += nz;
        }
    }

    // now parse sub-chunks inside FACE_DESC to read FACE_MAT groups
    long subs = FileTell();

    int matGroupCount = 0;
    long end = findex + length - 6;
    while (FileTell() < end)
    {
        long chunkStart = FileTell();
        if (!ReadChunkHeader(h)) break;
        if (h.len < 6 || chunkStart + (long)h.len > end + 6) { SafeSeek(end, SEEK_SET); break; }

        if (h.id == FACE_MAT) matGroupCount++;

        SafeSeek(chunkStart + (long)h.len, SEEK_SET);
    }

    if (matGroupCount > 0)
    {
        o.MatFaces.clear();
        o.MatFaces.resize(matGroupCount);

        SafeSeek(subs, SEEK_SET);

        int idx = 0;
        while (FileTell() < end && idx < matGroupCount)
        {
            long chunkStart = FileTell();
            if (!ReadChunkHeader(h)) break;
            if (h.len < 6 || chunkStart + (long)h.len > end + 6) { SafeSeek(end, SEEK_SET); break; }

            if (h.id == FACE_MAT)
            {
                FacesMaterialsListChunkProcessor(FileTell(), objindex, idx);
                idx++;
            }

            SafeSeek(chunkStart + (long)h.len, SEEK_SET);
        }
    }
    else
    {
        // ✅ fallback: no material assignment → draw all faces as one group
        o.MatFaces.clear();
    }

    SafeSeek(findex, SEEK_SET);
}

void Model::FacesMaterialsListChunkProcessor(long findex, int objindex, int subfacesindex)
{
    auto& o = Objects[objindex];
    SafeSeek(findex, SEEK_SET);

    char matName[80]{};
    ReadCString80(bin3ds, matName);

    int materialIndex = -1;
    for (int i = 0; i < (int)Materials.size(); i++) {
        if (strcmp(matName, Materials[i].name) == 0) { materialIndex = i; break; }
    }

    uint16_t numEntries = 0;
    fread(&numEntries, sizeof(numEntries), 1, bin3ds);

    if (numEntries == 0 || numEntries > 200000) {
        o.MatFaces[subfacesindex].MatIndex = materialIndex;
        o.MatFaces[subfacesindex].subFaces.clear();
        return;
    }

    o.MatFaces[subfacesindex].MatIndex = materialIndex;
    o.MatFaces[subfacesindex].subFaces.assign((size_t)numEntries * 3, 0);

    for (int i = 0; i < (int)numEntries * 3; i += 3)
    {
        uint16_t faceIndex = 0;
        fread(&faceIndex, sizeof(faceIndex), 1, bin3ds);

        // faceIndex references a triangle in the face list
        // o.Faces = [a,b,c, a,b,c, ...]
        size_t base = (size_t)faceIndex * 3;
        if (base + 2 < o.Faces.size())
        {
            o.MatFaces[subfacesindex].subFaces[i] = o.Faces[base];
            o.MatFaces[subfacesindex].subFaces[i + 1] = o.Faces[base + 1];
            o.MatFaces[subfacesindex].subFaces[i + 2] = o.Faces[base + 2];
        }
    }
}

void Model::CalculateNormals()
{
    for (auto& o : Objects)
    {
        if (o.Normals.empty() || o.numVerts <= 0) continue;

        for (int g = 0; g < o.numVerts; g++)
        {
            float x = o.Normals[g * 3 + 0];
            float y = o.Normals[g * 3 + 1];
            float z = o.Normals[g * 3 + 2];

            float len = std::sqrt(x * x + y * y + z * z);
            if (len < 1e-8f) len = 1.f;

            o.Normals[g * 3 + 0] = x / len;
            o.Normals[g * 3 + 1] = y / len;
            o.Normals[g * 3 + 2] = z / len;
        }
    }
}

bool Model::ResolveTextureFullPath(const char* texName, char outFullPath[260]) const
{
    if (!texName || !texName[0]) return false;

    sprintf_s(outFullPath, 260, "%s%s", pathDir.c_str(), texName);
    if (FileExistsA_(outFullPath)) return true;

    const char* folders[] = { "textures\\", "Textures\\", "maps\\", "Maps\\", "images\\", "Images\\" };
    for (auto f : folders)
    {
        sprintf_s(outFullPath, 260, "%s%s%s", pathDir.c_str(), f, texName);
        if (FileExistsA_(outFullPath)) return true;
    }

    outFullPath[0] = 0;
    return false;
}
