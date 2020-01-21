//
//  legomesh.cpp
//  
//
//  Created by Leonid on 13/09/2015.
//
//
#include <assert.h>
#include <cmath>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <algorithm>

#include "legomesh.h"

#include "ldrawcolor.h"

#ifdef __ANDROID_API__
#include <android/log.h>
#define LOG_TAG "studless"
#define LOG_INFO(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#endif

char* strtoupr(char *string)
{
    for (char *c = string; *c; c++)
        *c = (char)toupper(*c);
    
    return string;
}

char *skip_token(char *p)
{
    while (*p && !isspace(*p)) {
        p++;
    }
    while (*p && isspace(*p) && (*p != '\n')) {
        p++;
    }
    return p;
}

char *get_next_token(char *startPtr, char *token, int maxcount)
{
    char *next_p = startPtr;
    char *p = token;
    
    while (!isspace(*next_p) && *next_p && ((p - token) < maxcount)) {
        *p++ = *next_p++;
    }
    *p++ = 0;
    while (*next_p && isspace(*next_p) && (*next_p != '\n')) {
        next_p++;
    }
    return next_p;
}

char *get_refmodel_name(char *startPtr, char *token, int maxcount)
{
    char *next_p = startPtr;
    char *p = token;

    //
    // it turns out, that "name" here is not a token. In MPD files this is just rest of the line,
    // including spaces. Probably the best way to detect where it ends, is to strip spaces from
    // the _end_ of line
    //
    while ((*next_p != '\n') && (*next_p != '\r') && *next_p && ((p - token) < maxcount)) {
        *p++ = *next_p++;
    }
    p--;
    while (isspace(*p)) {
        p--;
    }
    p++;
    *p = 0;
    return next_p;
}

char *simple_atof(char *p, float *v)
{
    float r = 0.0;
    bool neg = false;
    if (*p == '-') {
        neg = true;
        ++p;
    }
    while (isdigit(*p)) {
        r = (r*10.0f) + (*p - '0');
        ++p;
    }
    if (*p == '.') {
        float f = 0.0;
        float n = 1;
        ++p;
        while (isdigit(*p)) {
            f = (f*10.0f) + (*p - '0');
            ++p;
            n *= 10.0f;
        }
        r += f / n;
    }
    if ( *p == 'e' || *p == 'E' ) {
        p++;
        bool negexp = false;
        if ( *p == '-' ) {
            negexp = true;
            p++;
        }
        int expval = 0;
        while (isdigit(*p)) {
            expval = expval * 10 + (*p++ - '0');
        }
        if ( negexp ) expval = -expval;
        //r *= std::powf(10.0, expval);
        r *= powf(10.0, expval);
    }
    if (neg) {
        r = -r;
    }
    *v = r;
    return p;
}

char *simple_atoi(char *p, int *i)
{
    int v = 0;
    while (*p && isdigit(*p)) {
        v = 10*v + (*p++ - '0');
    }
    *i = v;
    return p;
}

char *load_line_array(char *buffer, float *array, int count)
{
    for (int i = 0; i < count; i++) {
        buffer = simple_atof(buffer, array + i);
        buffer = skip_token(buffer);
    }
    return buffer;
}

uint32_t colorForCode(int codeIndex)
{
    if ( codeIndex >= sizeof(g_ldrawColors) / sizeof(g_ldrawColors[0])) {
        return g_ldrawColors[1].color;
    }
    if ( (codeIndex == 16) || (codeIndex == 24) ) {
        return (uint32_t)codeIndex;
    }
    return g_ldrawColors[codeIndex].color;
}

uint32_t complementColorForCode(int codeIndex)
{
    if ( codeIndex >= sizeof(g_ldrawColors) / sizeof(g_ldrawColors[0])) {
        return g_ldrawColors[1].complement;
    }
    return g_ldrawColors[codeIndex].complement;
}

BoundingBox::BoundingBox() : mMinPoint(FLT_MAX, FLT_MAX, FLT_MAX),
    mMaxPoint(-FLT_MAX, -FLT_MAX, -FLT_MAX)
{
}

void BoundingBox::addVertex(Vector3 &v)
{
    if ( v.x > mMaxPoint.x ) mMaxPoint.x = v.x;
    if ( v.y > mMaxPoint.y ) mMaxPoint.y = v.y;
    if ( v.z > mMaxPoint.z ) mMaxPoint.z = v.z;

    if ( v.x < mMinPoint.x ) mMinPoint.x = v.x;
    if ( v.y < mMinPoint.y ) mMinPoint.y = v.y;
    if ( v.z < mMinPoint.z ) mMinPoint.z = v.z;
}

void VertexBuffer::addVertex(Vector3 &point, Vector3 &normal, uint32_t color)
{
    Vertex v;
    v.position = point;
#ifdef USE_NORMALS
    v.normal = normal;
#endif
    if ( (color == 16) || (color == 24) ) {
        v.color = Vector4U8(0, 0, 0, (uint8_t)color);
    } else {
        v.color = Vector4U8(color);
    }
    vertexBuffer.push_back(v);
}

void VertexBuffer::addVertex(Vector3 &point, uint32_t color)
{
    Vertex v;
    v.position = point;
#ifdef USE_NORMALS
    v.normal = Vector3(0.0, 0.0, 0.0);
#endif
    if ( (color == 16) || (color == 24) ) {
        v.color = Vector4U8(0, 0, 0, (uint8_t)color);
    } else {
        v.color = Vector4U8(color);
    }
    vertexBuffer.push_back(v);
}

void VertexBuffer::clone(VertexBuffer &buf)
{
    vertexBuffer.resize(buf.vertexBuffer.size());
    for (int i = 0; i < buf.vertexBuffer.size(); i++) {
        vertexBuffer[i] = buf.vertexBuffer[i];
    }
}

void VertexBuffer::copyWithTransform(VertexBuffer &buf, Matrix44 &transform, int colorCode)
{
    size_t startPosVertex = vertexBuffer.size();
    
    vertexBuffer.resize(startPosVertex + buf.vertexBuffer.size());
    
    for (int i = 0; i < buf.vertexBuffer.size(); i++) {
        Vector3 point = lcMul31(buf.vertexBuffer[i].position, transform);
#ifdef USE_NORMALS
        Vector3 normal = lcMul31(buf.vertexBuffer[i].normal, transform);
#endif
        Vector4U8 refColor = buf.vertexBuffer[i].color;
        if ( (refColor.w == 16) && (colorCode != 16) ) {
            uint32_t rgbColor = colorForCode(colorCode);
            refColor = Vector4U8(rgbColor);
        }
        if ( (refColor.w == 24) && (colorCode != 24) && (colorCode != 16) ) {
            uint32_t rgbColor = complementColorForCode(colorCode);
            refColor = Vector4U8(rgbColor);
        }
        Vertex v;
        v.position = point;
#ifdef USE_NORMALS
        v.normal = normal;
#endif
        v.color = refColor;
        vertexBuffer[startPosVertex + i] = v;
    }
}

void VertexBuffer::copyWithTransformInvertTri(VertexBuffer &/*buf*/, Matrix44 &/*transform*/, int /*colorCode*/)
{
    assert(0);
}

//
// There's little to no chance to find matching vertex long before current. So, start
// looking from most recent, and go back for limited depth
//
#define MAX_INDEX_LOOKUP_DEPTH 17
void VertexBufferIndexed::addVertex(Vector3 &point, Vector3 &normal, uint32_t color)
{
    int lookupEnd = (int)vertexBuffer.size() > MAX_INDEX_LOOKUP_DEPTH ?
        (int)vertexBuffer.size() - MAX_INDEX_LOOKUP_DEPTH : 0;
    for (int i = (int)vertexBuffer.size() - 1; i >= lookupEnd; i--) {
        if ( vertexBuffer[i].position == point ) {
            indexBuffer.push_back(i);
            return;
        }
    }
    uint32_t pos = (uint32_t)vertexBuffer.size();
    VertexBuffer::addVertex(point, normal, color);
    indexBuffer.push_back(pos);
}

void VertexBufferIndexed::addVertex(Vector3 &point, uint32_t color)
{
    int lookupEnd = (int)vertexBuffer.size() > MAX_INDEX_LOOKUP_DEPTH ?
        (int)vertexBuffer.size() - MAX_INDEX_LOOKUP_DEPTH : 0;
    for (int i = (int)vertexBuffer.size() - 1; i >= lookupEnd; i--) {
        if ( vertexBuffer[i].position == point ) {
            indexBuffer.push_back(i);
            return;
        }
    }
    uint32_t pos = (uint32_t)vertexBuffer.size();
    VertexBuffer::addVertex(point, color);
    indexBuffer.push_back(pos);
}

void VertexBufferIndexed::clone(VertexBuffer &buf)
{
    VertexBuffer::clone(buf);
    
    VertexBufferIndexed &ibuf = *((VertexBufferIndexed *)(&buf));
    indexBuffer.resize(ibuf.indexBuffer.size());
    for (int i = 0; i < ibuf.indexBuffer.size(); i++) {
        indexBuffer[i] = ibuf.indexBuffer[i];
    }
}

void VertexBufferIndexed::copyWithTransform(VertexBuffer &buf, Matrix44 &transform, int colorCode)
{
    size_t startPosVertex = vertexBuffer.size();
    size_t startPosIndex = indexBuffer.size();

    VertexBuffer::copyWithTransform(buf, transform, colorCode);
   
    VertexBufferIndexed &ibuf = *((VertexBufferIndexed *)(&buf));
    indexBuffer.resize(startPosIndex + ibuf.indexBuffer.size());
    
    for (int i = 0; i < ibuf.indexBuffer.size(); i++) {
        indexBuffer[startPosIndex + i] = ibuf.indexBuffer[i] + (uint32_t)startPosVertex;
    }
}

void VertexBufferIndexed::copyWithTransformInvertTri(VertexBuffer &buf, Matrix44 &transform, int colorCode)
{
    size_t startPosVertex = vertexBuffer.size();
    size_t startPosIndex = indexBuffer.size();
    
    VertexBuffer::copyWithTransform(buf, transform, colorCode);
    
    VertexBufferIndexed &ibuf = *((VertexBufferIndexed *)(&buf));
    indexBuffer.resize(startPosIndex + ibuf.indexBuffer.size());
    
    for (int i = 0; i < ibuf.indexBuffer.size(); i += 3) {
        indexBuffer[startPosIndex + i + 0] = ibuf.indexBuffer[i + 0] + (uint32_t)startPosVertex;
        indexBuffer[startPosIndex + i + 1] = ibuf.indexBuffer[i + 2] + (uint32_t)startPosVertex;
        indexBuffer[startPosIndex + i + 2] = ibuf.indexBuffer[i + 1] + (uint32_t)startPosVertex;
    }
}

LdrawLib *LdrawLib::sSharedInstance = 0;

LdrawLib::LdrawLib(const char *file)
{
    mLdrawFile = new MultiFile(file);
    assert(sSharedInstance == 0);
    sSharedInstance = this;
}

//
// http://www.ldraw.org/article/218.html
//
#define LDRAW_META_BFC              0x00434642
#define LDRAW_META_BFC_CERTIFY      0x54524543
#define LDRAW_META_BFC_NOCERTIFY    0x45434f4e
#define LDRAW_META_BFC_CW           0x00005743
#define LDRAW_META_BFC_CCW          0x00574343
#define LDRAW_META_BFC_CLIP         0x50494c43
#define LDRAW_META_BFC_NOCLIP       0x4c434f4e
#define LDRAW_META_BFC_INVERTNEXT   0x45564e49

char *LdrawLib::readLine_META(char *startPtr, ParseContext *ctx)
{
    char metaToken[256];
    
    *((uint32_t *)(&metaToken[0])) = 0;
    char *next_p = get_next_token(startPtr, metaToken, sizeof(metaToken));
    strtoupr(metaToken);
    switch (*((uint32_t *)(&metaToken[0]))) {
        case LDRAW_META_BFC:
            *((uint32_t *)(&metaToken[0])) = 0;
            next_p = get_next_token(next_p, metaToken, sizeof(metaToken));
            switch ( *((uint32_t *)(&metaToken[0])) ) {
                case LDRAW_META_BFC_CERTIFY:
                    *((uint32_t *)(&metaToken[0])) = 0;
                    next_p = get_next_token(next_p, metaToken, sizeof(metaToken));
                    ctx->bfcStateKnown = true;
                    ctx->bfcEnable = true;
                    switch ( *((uint32_t *)(&metaToken[0])) ) {
                        case LDRAW_META_BFC_CW:
                            ctx->bfcCCW = false;
                            break;
                        case LDRAW_META_BFC_CCW:
                            ctx->bfcCCW = true;
                            break;
                        default:
                            break;
                    }
                    break;
                case LDRAW_META_BFC_NOCERTIFY:
                    ctx->bfcStateKnown = true;
                    ctx->bfcEnable = false;
                    break;
                case LDRAW_META_BFC_CW:
                    if ( !ctx->bfcStateKnown || ctx->bfcEnable ) {
                        ctx->bfcStateKnown = true;
                        ctx->bfcEnable = true;
                        ctx->bfcCCW = true;
                    }
                    break;
                case LDRAW_META_BFC_CCW:
                    if ( !ctx->bfcStateKnown || ctx->bfcEnable ) {
                        ctx->bfcStateKnown = true;
                        ctx->bfcEnable = true;
                        ctx->bfcCCW = false;
                    }
                    break;
                case LDRAW_META_BFC_CLIP:
                    if ( !ctx->bfcStateKnown || ctx->bfcEnable ) {
                        ctx->bfcStateKnown = true;
                        ctx->bfcEnable = true;
                        ctx->bfcCCW = false;
                    }
                    break;
                case LDRAW_META_BFC_NOCLIP:
                    if ( !ctx->bfcStateKnown || ctx->bfcEnable ) {
                        ctx->bfcStateKnown = true;
                        ctx->bfcEnable = true;
                        ctx->bfcCCW = false;
                    }
                    break;
                case LDRAW_META_BFC_INVERTNEXT:
                    if ( !ctx->bfcStateKnown || ctx->bfcEnable ) {
                        ctx->bfcStateKnown = true;
                        ctx->bfcEnable = true;
                        ctx->bfcInvertNext = true;
                    }
                    break;
                default:
                    break;
            }
            break;
            
        default:
            if ( ctx->DescriptionPtr == 0 ) {
                ctx->DescriptionPtr = startPtr;
            }
            break;
    }
    //printf("%s: meta token=%s\n", __func__, metaToken);
    while (*next_p && (*next_p != '\n')) {
        next_p++;
    }
    return next_p;
}

//
// Line Type 1
//
// Line type 1 is a sub-file reference. The generic format is:
//
// 1 <colour> x y z a b c d e f g h i <file>
//
char *LdrawLib::readLine_REF(char *startPtr, ParseContext *ctx)
{
    float array[16];
    
    char *next_p = load_line_array(startPtr, array, 13);
    if ( !next_p ) {
        return 0;
    }
    int colorCode = (int)array[0];
    
    float *fm = array + 1;
    Matrix44 IncludeTransform(Vector4(fm[3], fm[6], fm[9],  0.0f),
                              Vector4(fm[4], fm[7], fm[10], 0.0f),
                              Vector4(fm[5], fm[8], fm[11], 0.0f),
                              Vector4(fm[0], fm[1], fm[2],  1.0f));

    //
    // determinant
    //
    Vector4 &l0 = IncludeTransform.r[0];
    Vector4 &l1 = IncludeTransform.r[1];
    Vector4 &l2 = IncludeTransform.r[2];
    float transD =
        l0[0] * (l1[1]*l2[2] - l1[2]*l2[1]) -
        l0[1] * (l1[0]*l2[2] - l1[2]*l2[0]) +
        l0[2] * (l1[0]*l2[1] - l1[1]*l2[0]);
    
    LegoPiece *refPiece;

    char refName[256];
    next_p = get_refmodel_name(next_p, refName, sizeof(refName));
    refPiece = lookup(refName);    
    assert(refPiece);
    ctx->bfcEnable &= refPiece->BFC();
    
    //
    // now copy with transform
    //
    bool transIsInverse = transD < 0.0;
    if (ctx->bfcEnable && (ctx->bfcInvertNext ^ transIsInverse)) {
        ctx->currentTriBuffer.copyWithTransformInvertTri(refPiece->mTriVertexBuffer, IncludeTransform, colorCode);
    } else {
        ctx->currentTriBuffer.copyWithTransform(refPiece->mTriVertexBuffer, IncludeTransform, colorCode);
    }
    ctx->currentLineBuffer.copyWithTransform(refPiece->mLineVertexBuffer, IncludeTransform, colorCode);

    while (*next_p && (*next_p != '\n')) {
        next_p++;
    }
    return next_p;
}

//
// Line Type 2
//
// Line type 2 is a line drawn between two points. The generic format is:
//
// 2 <colour> x1 y1 z1 x2 y2 z2
//
// <colour> is a number representing the colour of the part, typically this is 24 - the edge colour. See the Colours section for allowable colour numbers.
// x1 y1 z1 is the coordinate of the first point
// x2 y2 z2 is the coordinate of the second point
//
char *LdrawLib::readLine_LINE(char *startPtr, ParseContext *ctx)
{
    float array[1+3*2];
    
    char *next_p = load_line_array(startPtr, array, 7);
    if ( !next_p ) {
        return 0;
    }
    int colorCode = (int)array[0];
    Vector3 p0(array[1], array[2], array[3]);
    Vector3 p1(array[4], array[5], array[6]);

    uint32_t rgbColor = colorForCode(colorCode);
    ctx->currentLineBuffer.addVertex(p0, rgbColor);
    ctx->currentLineBuffer.addVertex(p1, rgbColor);
    
    while (*next_p && (*next_p != '\n')) {
        next_p++;
    }
    return next_p;
}

//
// Line Type 3
//
// Line type 3 is a filled triangle drawn between three points. The generic format is:
//
// 3 <colour> x1 y1 z1 x2 y2 z2 x3 y3 z3
//
// <colour> is a number representing the colour of the part. See the Colours section for allowable colour numbers.
// x1 y1 z1 is the coordinate of the first point
// x2 y2 z2 is the coordinate of the second point
// x3 y3 z3 is the coordinate of the third point
//
char *LdrawLib::readLine_TRI(char *startPtr, ParseContext *ctx)
{
    float array[1+3*3];
    
    char *next_p = load_line_array(startPtr, array, 1+3*3);
    if ( !next_p ) {
        return 0;
    }
    int colorCode = (int)array[0];
    Vector3 p0(array[1], array[2], array[3]);
    Vector3 p1(array[4], array[5], array[6]);
    Vector3 p2(array[7], array[8], array[9]);

    Vector3 v01(p1 - p0);
    Vector3 v02(p2 - p0);
    Vector3 n(lcCross(v01, v02));
    uint32_t rgbColor = colorForCode(colorCode);
    if ( ctx->bfcEnable && !ctx->bfcCCW ) {
        ctx->currentTriBuffer.addVertex(p0, n, rgbColor);
        ctx->currentTriBuffer.addVertex(p2, n, rgbColor);
        ctx->currentTriBuffer.addVertex(p1, n, rgbColor);
    } else {
        ctx->currentTriBuffer.addVertex(p0, n, rgbColor);
        ctx->currentTriBuffer.addVertex(p1, n, rgbColor);
        ctx->currentTriBuffer.addVertex(p2, n, rgbColor);
    }
    while (*next_p && (*next_p != '\n')) {
        next_p++;
    }
    return next_p;
}

//
// Line Type 4
//
// Line type 4 is a filled quadrilateral (also known as a "quad") drawn between four points. The generic format is:
//
// 4 <colour> x1 y1 z1 x2 y2 z2 x3 y3 z3 x4 y4 z4
//
// <colour> is a number representing the colour of the part. See the Colours section for allowable colour numbers.
// x1 y1 z1 is the coordinate of the first point
// x2 y2 z2 is the coordinate of the second point
// x3 y3 z3 is the coordinate of the third point
// x4 y4 z4 is the coordinate of the fourth point
//
char *LdrawLib::readLine_QUAD(char *startPtr, ParseContext *ctx)
{
    float array[1+3*4];
    
    char *next_p = load_line_array(startPtr, array, 1+3*4);
    if ( !next_p ) {
        return 0;
    }
    int colorCode = (int)array[0];
    Vector3 p0(array[1], array[2], array[3]);
    Vector3 p1(array[4], array[5], array[6]);
    Vector3 p2(array[7], array[8], array[9]);
    Vector3 p3(array[10], array[11], array[12]);
    
    Vector3 v01(p1 - p0);
    Vector3 v02(p2 - p0);
    Vector3 n(lcCross(v01, v02));

    uint32_t rgbColor = colorForCode(colorCode);
    if ( ctx->bfcEnable && !ctx->bfcCCW ) {
        ctx->currentTriBuffer.addVertex(p0, n, rgbColor);
        ctx->currentTriBuffer.addVertex(p2, n, rgbColor);
        ctx->currentTriBuffer.addVertex(p1, n, rgbColor);
        
        ctx->currentTriBuffer.addVertex(p2, n, rgbColor);
        ctx->currentTriBuffer.addVertex(p0, n, rgbColor);
        ctx->currentTriBuffer.addVertex(p3, n, rgbColor);
    } else {
        ctx->currentTriBuffer.addVertex(p0, n, rgbColor);
        ctx->currentTriBuffer.addVertex(p1, n, rgbColor);
        ctx->currentTriBuffer.addVertex(p2, n, rgbColor);

        ctx->currentTriBuffer.addVertex(p2, n, rgbColor);
        ctx->currentTriBuffer.addVertex(p3, n, rgbColor);
        ctx->currentTriBuffer.addVertex(p0, n, rgbColor);
    }
    while (*next_p && (*next_p != '\n')) {
        next_p++;
    }
    return next_p;
}

LegoPiece *LdrawLib::loadBuffer(char *buffer, const char *name)
{
#ifdef USE_INDEXED_BUFFER_TRI
    VertexBufferIndexed vertexTriBuffer;
#else
    VertexBuffer vertexTriBuffer;
#endif
#ifdef USE_INDEXED_BUFFER_LINES
    VertexBufferIndexed vertexLineBuffer;
#else
    VertexBuffer vertexLineBuffer;
#endif
    ParseContext ctx(vertexTriBuffer, vertexLineBuffer);
    
    while ( *buffer ) {
        LDRAW_LINE_TYPE lineType = INVALID;
        buffer = simple_atoi(buffer, (int *)&lineType);
        buffer = skip_token(buffer);
        switch (lineType) {
            case META:
                buffer = readLine_META(buffer, &ctx);
                break;
            case REF:
                buffer = readLine_REF(buffer, &ctx);
                ctx.bfcInvertNext = false;
                break;
            case LINE:
                buffer = readLine_LINE(buffer, &ctx);
                break;
            case TRI:
                buffer = readLine_TRI(buffer, &ctx);
                break;
            case QUAD:
                buffer = readLine_QUAD(buffer, &ctx);
                break;
            case OPT_LINE:
                while (*buffer && (*buffer != '\n')) {
                    buffer++;
                }
                break;
            default:
                return 0;
        }
        if ( ! buffer ) {
            break;
        }
        //
        // skip to end of string or start of token
        //
        while (*buffer && isspace(*buffer)) {
            buffer++;
        }
    }
    bool bfcForPiece = ctx.bfcStateKnown && ctx.bfcEnable && ctx.bfcCullEn;
    LegoPiece *piece = new LegoPiece(name, ctx.DescriptionPtr, vertexTriBuffer, vertexLineBuffer, bfcForPiece);
    
    mPieceLU[name] = piece;
    
    return piece;
}

LegoPiece *LdrawLib::lookup(char *name, bool mayFail)
{
    LegoPiece *piece;
    strtoupr(name);
    {
        char *tmp = name;
        while (*tmp) {
            if ( *tmp == '\\' ) *tmp = '/';
            tmp++;
        }
    }
    if ( mPieceLU.count(name) != 0 ) {
        piece = mPieceLU[name];
    } else {
        char *buf = mLdrawFile->lookupByName(name);
        //
        // ldraw library can not have unresolved parts
        //
        if ( (buf == 0) && mayFail ) {
            return 0;
        }
        assert(buf);
        if ( !buf ) {
            return 0;
        }
        piece = loadBuffer(buf, name);
    }
    return piece;
}

void LdrawLib::debugDump()
{
    for (std::map<std::string, LegoPiece *>::iterator i = mPieceLU.begin(); i != mPieceLU.end(); i++) {
        LegoPiece *p = i->second;
        printf("[%s] : tri=[%lu] line=[%lu]\n", i->first.c_str(),
               p->mTriVertexBuffer.vertexBuffer.size(),
               p->mLineVertexBuffer.vertexBuffer.size());
    }
}

LegoModel::LegoModel(const char *name)
{
    if ( name ) {
        mName = strdup(name);
    } else {
        mName = 0;
    }
    mTotalPieceCount = 0;
}

//LegoModel::LegoModel(LegoPiece *piece, int colorCode)
//{
//    mName = 0;
//    mTotalPieceCount = 1;
//    uint32_t rgbColor = colorForCode(colorCode);
//    uint32_t complementColor = complementColorForCode(colorCode);
//    Vector4 colorVec(LC_RGBA_RED(rgbColor), LC_RGBA_GREEN(rgbColor),
//                     LC_RGBA_BLUE(rgbColor), LC_RGBA_ALPHA(rgbColor));
//    Vector4 complementColorVec(LC_RGBA_RED(complementColor), LC_RGBA_GREEN(complementColor),
//                               LC_RGBA_BLUE(complementColor), LC_RGBA_ALPHA(complementColor));
//    LEGO_PIECE_REF pieceRef = {
//        .hidden = false,
//        .transform = Matrix44Identity(),
//        .colorCode = colorCode,
//        .complementColorVec = complementColorVec,
//        .colorVec = colorVec,
//    };
//    mPiecesMulti[piece].push_back(pieceRef);
//    
//    updateBoundingBox();
//}

LegoModel::~LegoModel()
{
    for (std::map<LegoPiece *, std::vector<LEGO_PIECE_REF>>::iterator i = mPiecesMulti.begin(); i != mPiecesMulti.end(); i++) {
        LegoPiece *p = i->first;
        p->releaseVAO();
    }
    if (mName) {
        free(mName);
    }
    glDeleteBuffers(1, &mVBO);
    glDeleteBuffers(1, &mIBO);
}

void LegoModel::copyWithTransform(LegoModel *subModel, Matrix44 &transform)
{
    for (std::map<LegoPiece *, std::vector<LEGO_PIECE_REF>>::iterator i = subModel->mPiecesMulti.begin();
        i != subModel->mPiecesMulti.end(); i++) {
        LegoPiece *p = i->first;
        std::vector<LEGO_PIECE_REF> &pieceRefArray = i->second;
        
        p->updateBoundingBox();
        BoundingBox &box = p->mBounds;
        for (std::vector<LEGO_PIECE_REF>::iterator j = pieceRefArray.begin(); j != pieceRefArray.end(); j++) {
            LEGO_PIECE_REF pieceRef = *j;
            pieceRef.transform = lcMul(transform, pieceRef.transform);
            mPiecesMulti[p].push_back(pieceRef);
            Vector3 maxPointTransformed = lcMul31(box.mMaxPoint, transform);
            mBounds.addVertex(maxPointTransformed);
            Vector3 minPointTransformed = lcMul31(box.mMinPoint, transform);
            mBounds.addVertex(minPointTransformed);
        }
    }
}


// "FILE"
#define LDRAW_META_FILE 0x454c4946
#define LDRAW_META_GHOST 0x534f4847
char *LegoModel::readLine_META(char *startPtr, LdrawLib *lib, bool &startNewModel)
{
    char metaToken[256];
    char *next_p = get_next_token(startPtr, metaToken, sizeof(metaToken));
    switch (*((uint32_t *)(&metaToken[0]))) {
        //
        // FILE <token> directive: if we're at the beginning, go on. Otherwise
        // this means separator between models.
        //
        case LDRAW_META_FILE:
            if ( mPiecesMulti.empty() && mUnresolvedRefs.empty() ) {
                next_p = get_refmodel_name(next_p, metaToken, sizeof(metaToken));
                strtoupr(metaToken);
                mName = strdup(metaToken);
            } else {
                //
                // rewind buffer pointer to start of FILE line, so
                // next parser iteration will pick it
                //
                while (*next_p != '0') {
                    next_p--;
                }
                startNewModel = true;
                return next_p;
            }
            break;

        //
        // 0 GHOST ...
        //
        case LDRAW_META_GHOST:
            //
            // eat line type token
            //
            next_p = get_next_token(next_p, metaToken, sizeof(metaToken));
            if ( *((uint16_t *)(&metaToken[0])) == 0x31 ) {
                next_p = readLine_REF(next_p, lib);
            }
            break;
        default:
            break;
    }
    while (*next_p && (*next_p != '\n')) {
        next_p++;
    }
    return next_p;
}

char *LegoModel::readLine_REF(char *startPtr, LdrawLib *lib)
{
    float array[16];
    
    char *next_p = load_line_array(startPtr, array, 13);
    if ( !next_p ) {
        return 0;
    }
    int colorCode = (int)array[0];
    uint32_t rgbColor = colorForCode(colorCode);
    uint32_t complementColor = complementColorForCode(colorCode);
    
    float *fm = array + 1;
    Matrix44 IncludeTransform(Vector4(fm[3], fm[6], fm[9],  0.0f),
                              Vector4(fm[4], fm[7], fm[10], 0.0f),
                              Vector4(fm[5], fm[8], fm[11], 0.0f),
                              Vector4(fm[0], fm[1], fm[2],  1.0f));
    
    LegoPiece *refPiece;
    
    char refName[256];
    next_p = get_refmodel_name(next_p, refName, sizeof(refName));
    
    refPiece = lib->lookup(refName, true);
    if ( refPiece ) {
        std::vector<LEGO_PIECE_REF> &reflist = mPiecesMulti[refPiece];
        Vector4 colorVec(LC_RGBA_RED(rgbColor), LC_RGBA_GREEN(rgbColor),
                         LC_RGBA_BLUE(rgbColor), LC_RGBA_ALPHA(rgbColor));
        Vector4 complementColorVec(LC_RGBA_RED(complementColor), LC_RGBA_GREEN(complementColor),
                         LC_RGBA_BLUE(complementColor), LC_RGBA_ALPHA(complementColor));
        LEGO_PIECE_REF pieceRef = {
                .colorVec = colorVec,
                .complementColorVec = complementColorVec,
                .mMV = {}, .mMVP =  {},
                .transform = IncludeTransform,
                .colorCode = colorCode,
                .hidden = false, };
        reflist.push_back(pieceRef);
    } else {
        LEGO_PIECE_UNRESOLVED_REF unresolved = {.rgbColor =  rgbColor, .transform =  IncludeTransform};
        std::vector<LEGO_PIECE_UNRESOLVED_REF> &unresList = mUnresolvedRefs[refName];
        unresList.push_back(unresolved);
    }
    return next_p;
}

char *LegoModel::loadFromBuffer(char *buffer, LdrawLib *lib)
{
    bool startNewModel = false;
    int lineCount = 0;
    while ( *buffer && !startNewModel ) {
        LdrawLib::LDRAW_LINE_TYPE lineType = LdrawLib::INVALID;
        buffer = simple_atoi(buffer, (int *)&lineType);
        buffer = skip_token(buffer);
        //
        // contrary to stuff in ldraw library, model can contain only meta and ref lines
        //
        switch (lineType) {
            case LdrawLib::META:
                buffer = readLine_META(buffer, lib, startNewModel);
                if ( startNewModel ) {
                    updateBoundingBox();
                    return buffer;
                }
                break;
            case LdrawLib::REF:
                buffer = readLine_REF(buffer, lib);
                break;
            //
            // I'm not sure how one add line primitive to model level, but it seems
            // safe to simply skip it.
            // Update: it some models, people _do_ include primitives inside MPD files. I have
            // absolutely no idea what should I do with it. Skip this for safety.
            //
            default:
                while (*buffer && (*buffer != '\n')) {
                    buffer++;
                }
                break;
        }
        while (*buffer && isspace(*buffer)) {
            buffer++;
        }
        lineCount++;
    }
    
    updateBoundingBox();

    return 0;
}

LegoModel *LegoModel::createWithSinglePiece(LegoPiece *piece, int colorCode)
{
    LegoModel *topModel = new LegoModel();
    std::vector<LEGO_PIECE_REF> &reflist = topModel->mPiecesMulti[piece];

    uint32_t rgbColor = colorForCode(colorCode);
    uint32_t complementColor = complementColorForCode(colorCode);

    Vector4 colorVec(LC_RGBA_RED(rgbColor), LC_RGBA_GREEN(rgbColor),
                     LC_RGBA_BLUE(rgbColor), LC_RGBA_ALPHA(rgbColor));
    Vector4 complementColorVec(LC_RGBA_RED(complementColor), LC_RGBA_GREEN(complementColor),
                               LC_RGBA_BLUE(complementColor), LC_RGBA_ALPHA(complementColor));
    LEGO_PIECE_REF pieceRef = {
            .colorVec = colorVec,
            .complementColorVec = complementColorVec,
            .mMV = {}, .mMVP = {},
            .transform = Matrix44Identity(),
            .colorCode = colorCode,
            .hidden = false,
            };
    reflist.push_back(pieceRef);

    topModel->updateBoundingBox();
    
    return topModel;
}

LegoModel *LegoModel::createFromBuffer(char *buffer, LdrawLib *lib)
{
    LegoModel *topModel = new LegoModel();
    buffer = topModel->loadFromBuffer(buffer, lib);
    if ( buffer ) {
        std::map<std::string, LegoModel *> subModels;
        while ( buffer && *buffer ) {
            LegoModel *subModel = new LegoModel();
            buffer = subModel->loadFromBuffer(buffer, lib);
            subModels[subModel->Name()] = subModel;
        }
        topModel->resolveFromList(subModels);
    }
    return topModel;
}

LegoModel *LegoModel::createFromConvertedList(std::map<std::string, std::vector<LEGO_PIECE_REF>> &lxfList, LdrawLib *lib)
{
    LegoModel *topModel = new LegoModel();
    char luname[64];
    for (auto i = lxfList.begin(); i != lxfList.end(); i++) {
        luname[0] = 0;
        strcat(luname, i->first.c_str());
        strcat(luname, ".dat");
        LegoPiece *piece = lib->lookup(luname, true);
        if ( piece ) {
            std::vector<LEGO_PIECE_REF> &refList = i->second;
            topModel->mPiecesMulti[piece] = refList;
        }
    }
    topModel->updateBoundingBox();
    return topModel;
}

void LegoModel::resolveFromList(std::map<std::string, LegoModel *> &subModels)
{
    for (auto i = mUnresolvedRefs.begin(); i != mUnresolvedRefs.end(); i++) {
        if ( subModels.count(i->first) ) {
            LegoModel *subModel = subModels[i->first];
            subModel->resolveFromList(subModels);
            
            std::vector<LEGO_PIECE_UNRESOLVED_REF> &unresRefList = i->second;
            for (auto j = unresRefList.begin(); j != unresRefList.end(); j++) {
                LEGO_PIECE_UNRESOLVED_REF &unresRef = *j;
                copyWithTransform(subModel, unresRef.transform);
            }
            mTotalPieceCount += subModel->mTotalPieceCount;
        }
    }
}

void LegoModel::updateBoundingBox()
{
    for (std::map<LegoPiece *, std::vector<LEGO_PIECE_REF>>::iterator i = mPiecesMulti.begin(); i != mPiecesMulti.end(); i++) {
        LegoPiece *p = i->first;
        p->updateBoundingBox();
        BoundingBox &box = p->mBounds;
        std::vector<LEGO_PIECE_REF> &pieceRefArray = i->second;
        for (std::vector<LEGO_PIECE_REF>::iterator j = pieceRefArray.begin(); j != pieceRefArray.end(); j++) {
            Matrix44 &transform = j->transform;
            Vector3 transfMin = lcMul31(box.mMinPoint, transform);
            Vector3 transfMax = lcMul31(box.mMaxPoint, transform);
            mBounds.addVertex(transfMin);
            mBounds.addVertex(transfMax);
            mTotalPieceCount++;
        }
    }
    mBounds.mCenter = 0.5 * (mBounds.mMaxPoint + mBounds.mMinPoint);
}

void LegoModel::createBuffers()
{
    size_t totalVertexDataSize = 0;
    size_t totalIndexDataSize = 0;
    for (auto i = mPiecesMulti.begin(); i != mPiecesMulti.end(); i++) {
        totalVertexDataSize += i->first->mTriVertexBuffer.vertexDataSize();
        totalVertexDataSize += i->first->mLineVertexBuffer.vertexDataSize();
#ifdef USE_INDEXED_BUFFER_TRI
        totalIndexDataSize += (&(i->first->mTriVertexBuffer))->indexDataSize();
#endif
#ifdef USE_INDEXED_BUFFER_LINES
        totalIndexDataSize += (&(i->first->mLineVertexBuffer))->indexDataSize();
#endif
    }
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glGenBuffers(1, &mIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);

    //
    // only allocate memory, no data copying here
    //
    glBufferData(GL_ARRAY_BUFFER, totalVertexDataSize, 0, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, totalIndexDataSize, 0, GL_STATIC_DRAW);

#if DEBUG_GL_BUFFER
    printf("%s: VBO size %zu IBO size %zu\n", __func__, totalVertexDataSize, totalIndexDataSize);
#endif
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void LegoModel::bufferData()
{
    size_t vertexOffset = 0, indexOffset = 0;
    for (auto i = mPiecesMulti.begin(); i != mPiecesMulti.end(); i++) {
        LegoPiece *p = i->first;
        p->bufferData(vertexOffset, indexOffset, mVBO, mIBO);
    }
}

void LegoModel::loadGlBuffers()
{
    createBuffers();
    bufferData();
}

void LegoModel::drawGL(LdrawRender *renderer, LegoPiece *p, LEGO_PIECE_REF &pieceRef, bool useVAO)
{
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);

#if BFC_EN
    if ( p->BFC() ) {
        glEnable(GL_CULL_FACE);
    } else {
        glDisable(GL_CULL_FACE);
    }
#endif
    if ( !useVAO || !p->bindLTriVAO() ) {
        p->setGlTriAttribs();
    }

    renderer->updateShaderUniforms(pieceRef.colorVec, pieceRef.mMV, pieceRef.mMVP);
#ifdef USE_INDEXED_BUFFER_TRI
    //            {
    //                GLint buffid;
    //                glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &buffid);
    //                assert(buffid);
    //                glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &buffid);
    //                assert(buffid);
    //            }
    VertexBufferIndexed *triVbuffer = &p->mTriVertexBuffer;
    glDrawElements(GL_TRIANGLES, (GLsizei)triVbuffer->indexDataCount(), GL_UNSIGNED_INT,
                   (void *)p->mTriIndexOffset);
#else
    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)p->mTriVertexBuffer.vertexDataCount());
#endif

    if ( !useVAO || !p->bindLineVAO() ) {
        p->setGlLineAttribs();
    }
    
    renderer->updateShaderUniforms(pieceRef.complementColorVec, pieceRef.mMV, pieceRef.mMVP);
#ifdef USE_INDEXED_BUFFER_LINES
    //            {
    //                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
    //                GLint buffid;
    //                glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &buffid);
    //                assert(buffid);
    //            }
    VertexBufferIndexed *lineVbuffer = &p->mLineVertexBuffer;
    glDrawElements(GL_LINES, (GLsizei)lineVbuffer->indexDataCount(), GL_UNSIGNED_INT,
                   (void *)p->mLineIndexOffset);
#else
    glDrawArrays(GL_LINES, 0, (GLsizei)p->mLineVertexBuffer.vertexDataCount());
#endif

}

void LegoModel::drawGL(LdrawRender *renderer)
{
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);

    for (std::map<LegoPiece *, std::vector<LEGO_PIECE_REF>>::iterator i = mPiecesMulti.begin(); i != mPiecesMulti.end(); i++) {
        LegoPiece *p = i->first;
        std::vector<LEGO_PIECE_REF> &pieceRefArray = i->second;

#if BFC_EN
        if ( p->BFC() ) {
            glEnable(GL_CULL_FACE);
        } else {
            glDisable(GL_CULL_FACE);
        }
#endif
        if ( !p->bindLTriVAO() ) {
            p->setGlTriAttribs();
        }

        for (std::vector<LEGO_PIECE_REF>::iterator j = pieceRefArray.begin(); j != pieceRefArray.end(); j++) {
            LEGO_PIECE_REF &pieceRef = *j;
            
            if ( pieceRef.hidden ) {
                continue;
            }
            renderer->updateShaderUniforms(pieceRef.colorVec, pieceRef.mMV, pieceRef.mMVP);
//            renderer->updateShaderUniforms(pieceRef.colorVec, iii, iii);
#ifdef USE_INDEXED_BUFFER_TRI
//            {
//                GLint buffid;
//                glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &buffid);
//                assert(buffid);
//                glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &buffid);
//                assert(buffid);
//            }
            VertexBufferIndexed *triVbuffer = &p->mTriVertexBuffer;
            glDrawElements(GL_TRIANGLES, (GLsizei)triVbuffer->indexDataCount(), GL_UNSIGNED_INT,
                           (void *)p->mTriIndexOffset);
#else
            glDrawArrays(GL_TRIANGLES, 0, (GLsizei)p->mTriVertexBuffer.vertexDataCount());
#endif
        }
        
        if ( !p->bindLineVAO() ) {
            p->setGlLineAttribs();
        }

        for (std::vector<LEGO_PIECE_REF>::iterator j = pieceRefArray.begin(); j != pieceRefArray.end(); j++) {
            LEGO_PIECE_REF &pieceRef = *j;
            if ( pieceRef.hidden ) {
                continue;
            }
            renderer->updateShaderUniforms(pieceRef.complementColorVec, pieceRef.mMV, pieceRef.mMVP);
#ifdef USE_INDEXED_BUFFER_LINES
//            {
//                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
//                GLint buffid;
//                glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &buffid);
//                assert(buffid);
//            }
            VertexBufferIndexed *lineVbuffer = &p->mLineVertexBuffer;
            glDrawElements(GL_LINES, (GLsizei)lineVbuffer->indexDataCount(), GL_UNSIGNED_INT,
                           (void *)p->mLineIndexOffset);
#else
            glDrawArrays(GL_LINES, 0, (GLsizei)p->mLineVertexBuffer.vertexDataCount());
#endif
        }
    }
}

void LegoModel::updateTransform(Matrix44 &view, Matrix44 &project)
{
    for (std::map<LegoPiece *, std::vector<LEGO_PIECE_REF>>::iterator i = mPiecesMulti.begin(); i != mPiecesMulti.end(); i++) {
        std::vector<LEGO_PIECE_REF> &refsArray = i->second;
        for (std::vector<LEGO_PIECE_REF>::iterator j = refsArray.begin(); j != refsArray.end(); j++) {
            j->mMV = lcMul(view, j->transform);
            j->mMVP = lcMul(project, j->mMV);
        }
    }
}

void LegoModel::preparePositionList()
{
    mAbsMovingSpeed = 1.0;
    mPiecesToMove.reserve(mTotalPieceCount);
    for (std::map<LegoPiece *, std::vector<LEGO_PIECE_REF>>::iterator i = mPiecesMulti.begin(); i != mPiecesMulti.end(); i++) {
        LegoPiece *p = i->first;
        std::vector<LEGO_PIECE_REF> &pieceRefArray = i->second;
        for (std::vector<LEGO_PIECE_REF>::iterator j = pieceRefArray.begin(); j != pieceRefArray.end(); j++) {
            Matrix44 &transform = j->transform;
            Vector3 startingPosition(transform[3].x, transform[3].y, transform[3].z);
            PIECE_REAL_POS piecePos = { .piece =  p, .position =  startingPosition, .origPosition =  startingPosition,
                .pieceRef =  &(*j)};
            mPiecesToMove.push_back(piecePos);
        }
    }
    std::sort(mPiecesToMove.begin(), mPiecesToMove.end(), [](PIECE_REAL_POS a, PIECE_REAL_POS b) {
        return b.position.y > a.position.y;
    });
}

void LegoModel::createReport(LEGO_MODEL_REPORT &report)
{
    for (auto i = mPiecesMulti.begin(); i != mPiecesMulti.end(); i++) {
        std::vector<LEGO_PIECE_REF> &pieceRefs = i->second;
        std::map<int, int> &pieceRefsMap = report[i->first];
        for (auto j = pieceRefs.begin(); j != pieceRefs.end(); j++) {
            pieceRefsMap[j->colorCode]++;
        }
    }
}

void LegoModel::resetDisassemblyState()
{
    mMoveHeadIdx = -1;
    mMoveTailIdx = -1;
}

int LegoModel::getDisassemblyState()
{
    return mMoveHeadIdx;
}

void LegoModel::setDisassemblyPosition(int pos)
{
//    os_log("kuku");
    if ( pos == mMoveHeadIdx ) {
        return;
    }
    if ( pos > mMoveHeadIdx ) {
        mMoveHeadIdx = pos;
        mMovingSpeed = mAbsMovingSpeed;
        assert(mMoveHeadIdx < mTotalPieceCount);
    } else {
        mMovingSpeed = -mAbsMovingSpeed;
        if ( pos  <= mMoveTailIdx ) {
            //
            // eventually tail == head - 1
            //
            mMoveTailIdx = pos - 1;
        } else {
            assert(mMoveHeadIdx < mTotalPieceCount);
            //
            // this is tricky part. Some parts [tail:pos] are supposed
            // to be removed, while others [pos+1, head] need to be
            // put back. Since I only want to animate movement to 1
            // direction, "put back" part will be instant
            //
            for (int i = pos; i < mMoveHeadIdx; i++) {
                PIECE_REAL_POS &piecePos = mPiecesToMove[i];
                piecePos.pieceRef->transform = piecePos.originalTransform;
                piecePos.position = piecePos.origPosition;
                piecePos.pieceRef->hidden = false;
            }
        }
    }
}

int LegoModel::moveNextPieceAway()
{
    //
    // init - copy pieces array from
    // model, remember original location
    //
    if ( mMoveHeadIdx == -1 ) {
        preparePositionList();
        for (auto i = mPiecesToMove.begin(); i != mPiecesToMove.end(); i++) {
            i->originalTransform = i->pieceRef->transform;
        }
        mMoveHeadIdx = 0;
    }
    //
    // mMoveHeadIdx is pointing to "index of removed element + 1",
    // so it's maximum value is (totalParts + 1)
    //
    if ( mMoveHeadIdx < (mPiecesToMove.size()) ) {
        mMoveHeadIdx++;
    }
    mMovingSpeed = mAbsMovingSpeed;
    
//    assert(mMoveHeadIdx < mTotalPieceCount);
    
    return mMoveHeadIdx;
}

int LegoModel::moveNextPieceBack()
{
    if ( mMoveTailIdx > -1 ) {
        PIECE_REAL_POS &movingPiece = mPiecesToMove[mMoveTailIdx];
        movingPiece.pieceRef->hidden = false;
        mMoveTailIdx--;
    }
    mMovingSpeed = -mAbsMovingSpeed;
    return mMoveTailIdx+1;
}

void LegoModel::updateMovingPieces(float timePassed)
{
    if ( (mMoveHeadIdx - mMoveTailIdx) <= 1 ) {
        return;
    }

    int _1st_moving = mMoveTailIdx + 1;
    int last_moving = mMoveHeadIdx;
    for (int i = _1st_moving; i < last_moving; i++) {
        PIECE_REAL_POS &piecePos = mPiecesToMove[i];
        //
        // I want piece to reach out-of-view in 1 second
        //
        // 0 < normalizedY < 1
        //
        float normalizedY = (1.0f + piecePos.pieceRef->mMVP[3].y / piecePos.pieceRef->mMVP[3].w) / 2.0f;
        float leftToGo = 1.0f - normalizedY;
        
        //
        // In ldraw -Y is up
        //
        float currentInc = -leftToGo * timePassed * mMovingSpeed * piecePos.pieceRef->mMVP[3].w;
//        printf("%s: timepassed=%.2f inc=%.2f normalizedY=%.2f\n", __func__, timePassed, currentInc, normalizedY);
        Matrix44 moveTransform = Matrix44Translation(Vector3(0.0, currentInc, 0.0));
        piecePos.pieceRef->transform = lcMul(moveTransform, piecePos.pieceRef->transform);
        piecePos.position = lcMul31(piecePos.position, moveTransform);
        if ( mMovingSpeed < 0.0 ) {
            //
            // moving back
            //
            if ( piecePos.position.y > piecePos.origPosition.y) {
                piecePos.position.y = piecePos.origPosition.y;
                piecePos.pieceRef->transform = piecePos.originalTransform;
                //
                // as in case with tail - ensure that no matter what,
                // every piece end in its place
                //
                if ( mMoveHeadIdx < mPiecesToMove.size() ) {
                    mPiecesToMove[mMoveHeadIdx].pieceRef->transform = mPiecesToMove[mMoveHeadIdx].originalTransform;
                    mPiecesToMove[mMoveHeadIdx].position = mPiecesToMove[mMoveHeadIdx].origPosition;
                    mPiecesToMove[mMoveHeadIdx].pieceRef->hidden = false;
                }
                mMoveHeadIdx--;
            }
        } else {
            //
            // moving forward (away)
            //
            Vector3 screenPos(piecePos.pieceRef->mMVP[3].x, piecePos.pieceRef->mMVP[3].y, piecePos.pieceRef->mMVP[3].z);
            screenPos /= piecePos.pieceRef->mMVP[3].w;
            //
            // compare normalized Y coordinate
            //
            if ( screenPos.y > 0.920) {
                piecePos.pieceRef->hidden = true;
                //
                // ensure that in any case, no matter in which
                // order parts reach end-of-screen, they all
                // end hidden. It can happen that "tail+1" will
                // finish _before_ "tail"
                //
                if ( mMoveTailIdx >= 0 ) {
                    mPiecesToMove[mMoveTailIdx].pieceRef->hidden = true;
                }
                mMoveTailIdx++;
            }
        }
    }
}

LegoPiece::LegoPiece(const char *name, char *descPtr, VertexBuffer &triBuffer, VertexBuffer &lineBuffer, bool bfc)
{
    mName = strdup(name);
    mDescriptionPtr = descPtr;
    mDescription = 0;
    
    mLineVertexBuffer.clone(lineBuffer);
    mTriVertexBuffer.clone(triBuffer);
    
    mTriVAO = -1;
    mLineVAO = -1;
    
    mBFC = bfc;
}

const char *LegoPiece::Description()
{
    if ( mDescription ) {
        return mDescription;
    }
    static char stringBuff[256];
    char *p = stringBuff;
    const char *src = mDescriptionPtr;
    if ( (*src == '~') || (*src == '=') ) src++;
    while (*src && (*src != '\r') && (*src != '\n') && ((p - &stringBuff[0]) < sizeof(stringBuff))) {
        *p++ = *src++;
    }
    *p = 0;
    mDescription = strdup(stringBuff);
    return mDescription;
}

void LegoPiece::releaseVAO()
{
    glDeleteVertexArrays(1, &mTriVAO);
    glDeleteVertexArrays(1, &mLineVAO);
    mTriVAO = -1;
    mLineVAO = -1;
}

void LegoPiece::bufferData(size_t &vertexOffset, size_t &indexOffset, GLuint vao, GLuint ibo)
{
    glBindBuffer(GL_ARRAY_BUFFER, vao);
#ifdef USE_VAO
    glGenVertexArrays(1, &mTriVAO);
    glBindVertexArray(mTriVAO);
#endif
    mTriBuffStart = (uint8_t *)vertexOffset;
#if DEBUG_GL_BUFFER
    printf("%s: piece %p\n", __func__, this);
    
    printf("\t tri vertex buffer @ %zu\n", vertexOffset);
#endif
    glBufferSubData(GL_ARRAY_BUFFER, vertexOffset,
                    mTriVertexBuffer.vertexDataSize(), mTriVertexBuffer.vertexData());
    
    setGlTriAttribs();
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    vertexOffset += mTriVertexBuffer.vertexDataSize();

    //
    // lines buffer
    //
#ifdef USE_VAO
    glGenVertexArrays(1, &mLineVAO);
    glBindVertexArray(mLineVAO);
#endif
#if DEBUG_GL_BUFFER
    printf("\t line vertex buffer @ %zu\n", vertexOffset);
#endif
    mLineBuffStart = (uint8_t *)vertexOffset;
    glBufferSubData(GL_ARRAY_BUFFER, vertexOffset,
                    mLineVertexBuffer.vertexDataSize(), mLineVertexBuffer.vertexData());
    
    setGlLineAttribs();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    vertexOffset += mLineVertexBuffer.vertexDataSize();
    //
    // index buffer
    //
#ifdef USE_INDEXED_BUFFER_TRI
#if DEBUG_GL_BUFFER
    printf("\t tri index buffer @ %zu\n", indexOffset);
#endif
    mTriIndexOffset = indexOffset;
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, indexOffset,
                    mTriVertexBuffer.indexDataSize(), mTriVertexBuffer.indexData());
    indexOffset += mTriVertexBuffer.indexDataSize();
#endif
#ifdef USE_INDEXED_BUFFER_LINES
#if DEBUG_GL_BUFFER
    printf("\t line index buffer @ %zu\n", indexOffset);
#endif
    mLineIndexOffset = indexOffset;
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, indexOffset,
                    mLineVertexBuffer.indexDataSize(), mLineVertexBuffer.indexData());
    indexOffset += mLineVertexBuffer.indexDataSize();
#endif
}


void LegoPiece::updateBoundingBox()
{
    assert(!mTriVertexBuffer.vertexBuffer.empty() || !mLineVertexBuffer.vertexBuffer.empty());
    for (int i = 0; i < mTriVertexBuffer.vertexDataCount(); i++) {
        Vertex &v = mTriVertexBuffer.vertexBuffer[i];
        mBounds.addVertex(v.position);
    }
    if ( mTriVertexBuffer.vertexBuffer.empty() ) {
        for (int i = 0; i < mLineVertexBuffer.vertexDataCount(); i++) {
            Vertex &v = mLineVertexBuffer.vertexBuffer[i];
            mBounds.addVertex(v.position);
        }
    }
    mBounds.mCenter = 0.5 * (mBounds.mMaxPoint + mBounds.mMinPoint);
    assert(fabs(mBounds.mMinPoint.x) < 6666);
    assert(fabs(mBounds.mMinPoint.y) < 6666);
    assert(fabs(mBounds.mMinPoint.z) < 6666);
}

std::string strVector(Vector3 &v)
{
    char s[64];
    sprintf(s, "{%.2f %.2f %.2f}", v.x, v.y, v.z);
    
    return std::string(s);
}

void LegoPiece::debugDumpTri()
{
    /*
    for (int i = 0; i < mTriVertexBuffer.indexBuffer.size() / 3; i++) {
        printf("tri[%d] = \n", i);
        for (int j = 0; j < 3; j++) {
            int idx = mTriVertexBuffer.indexBuffer[i *3 + j];
            Vertex &v = mTriVertexBuffer.vertexBuffer[idx];
            std::string sp = strVector(v.position);
            std::string sn = strVector(v.normal);
            printf("\t[%s %s]\n", sp.c_str(), sn.c_str());
        }
        printf("\n");
    }
 */
}

bool LegoPiece::bindLineVAO()
{
    if ( mLineVAO != -1 ) {
        glBindVertexArray(mLineVAO);
        return true;
    }
    return false;
}

bool LegoPiece::bindLTriVAO()
{
    if ( mTriVAO != -1 ) {
        glBindVertexArray(mTriVAO);
        return true;
    }
    return false;
}

void LegoPiece::setGlLineAttribs()
{
    glEnableVertexAttribArray(LdrawRender::VertexAttribPosition);
    glVertexAttribPointer(LdrawRender::VertexAttribPosition, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (void *)(mLineBuffStart + offsetof(Vertex, position)));
    
#ifdef USE_NORMALS
    glEnableVertexAttribArray(LdrawRender::VertexAttribNormal);
    glVertexAttribPointer(LdrawRender::VertexAttribNormal, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (void *)(mLineBuffStart + offsetof(Vertex, normal)));
#endif
    glEnableVertexAttribArray(LdrawRender::VertexAttribColor);
    glVertexAttribPointer(LdrawRender::VertexAttribColor, 4, GL_UNSIGNED_BYTE, GL_FALSE,
                          sizeof(Vertex), (void *)(mLineBuffStart + offsetof(Vertex, color)));
}

void LegoPiece::setGlTriAttribs()
{
    glEnableVertexAttribArray(LdrawRender::VertexAttribPosition);
    glVertexAttribPointer(LdrawRender::VertexAttribPosition, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (void *)(mTriBuffStart + offsetof(Vertex, position)));
#ifdef USE_NORMALS
    glEnableVertexAttribArray(LdrawRender::VertexAttribNormal);
    glVertexAttribPointer(LdrawRender::VertexAttribNormal, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (void *)(mTriBuffStart + offsetof(Vertex, normal)));
#endif
    glEnableVertexAttribArray(LdrawRender::VertexAttribColor);
    glVertexAttribPointer(LdrawRender::VertexAttribColor, 4, GL_UNSIGNED_BYTE, GL_FALSE,
                          sizeof(Vertex), (void *)(mTriBuffStart + offsetof(Vertex, color)));
}

GLuint LdrawRender::compileShader(GLenum type, const char *source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    GLint logLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc((size_t)logLength);
        glGetShaderInfoLog(shader, logLength, &logLength, log);
        printf("%s: shader compilation failed, log message '%s'\n", __func__, log);
        free(log);
        abort();
    }

    return shader;
}

void LdrawRender::linkProgram()
{
    glLinkProgram(mProgram);
    GLint result;
    glGetProgramiv(mProgram,GL_LINK_STATUS,&result);
    if ( result != GL_TRUE) {
        GLint logLength;
        glGetProgramiv(mProgram, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0) {
            GLchar *log = (GLchar *) malloc((size_t)logLength);
            glGetProgramInfoLog(mProgram, logLength, &logLength, log);
            printf("%s: shader link failed, log message '%s'\n", __func__, log);
            free(log);
            abort();
        }
    }
}

void LdrawRender::loadShaders(const char *vshader, const char *fshader)
{
    GLuint vertShader, fragShader;

    mProgram = glCreateProgram();
    
    vertShader = compileShader(GL_VERTEX_SHADER, vshader);
    fragShader = compileShader(GL_FRAGMENT_SHADER, fshader);
    glAttachShader(mProgram, vertShader);
    
    glAttachShader(mProgram, fragShader);
    
    // This needs to be done prior to linking.
    glBindAttribLocation(mProgram, VertexAttribPosition, "position");
    glBindAttribLocation(mProgram, VertexAttribNormal, "normal");
    glBindAttribLocation(mProgram, VertexAttribColor, "color");

    linkProgram();

    mModelViewProjectMatrixU = glGetUniformLocation(mProgram, "modelViewProjectionMatrix");
    mNormalMatrixU = glGetUniformLocation(mProgram, "normalMatrix");
    mColor4Code1624U = glGetUniformLocation(mProgram, "color4code1624");
}

void LdrawRender::loadDefaultShaders_OSX()
{
    const char *vShaderSource =
    "#version 410\n"
    "    layout(location = 0) in vec4 position;\n"
    "    layout(location = 1) in vec3 normal;\n"
    "    layout(location = 2) in vec4 color;\n"
    "                                           \n"
    "    out mediump vec4 colorVarying;     \n"
    "                                           \n"
    "    out mediump vec3 normalFrag;       \n"
    "    out mediump vec3 positionFrag;     \n"
    "                                           \n"
    "    uniform mat4 modelViewProjectionMatrix;\n"
    "    uniform mat4 normalMatrix;             \n"
    "                                           \n"
    "    uniform vec4 color4code1624;           \n"
    "                                           \n"
    "    void main()                                                            \n"
    "    {                                                                      \n"
    "        normalFrag = normalize(vec3(normalMatrix * vec4(normal, 0.0)));    \n"
    "                                                                           \n"
    "        positionFrag = normalize(vec3(normalMatrix * position));           \n"
    "                                                                           \n"
    "        if ( (color.a == 16.0) || (color.a == 24.0)) {                     \n"
    "            colorVarying = color4code1624;                                 \n"
    "        } else {                                                           \n"
    "            colorVarying = color;                                          \n"
    "        }                                                                  \n"
    "        colorVarying = colorVarying / 255.0;                               \n"
    "        gl_Position = modelViewProjectionMatrix * position;                \n"
    "   }                                                                       \n";
    
    const char *fShaderSource =
    "#version 410\n"
    "in mediump vec4 colorVarying;     \n"
    "                                       \n"
    "in mediump vec3 normalFrag;       \n"
    "in mediump vec3 positionFrag;     \n"
    "out vec4 fragColor\n;"
    "                                       \n"
    "void main()                            \n"
    "{                                      \n"
    "    mediump vec3 LightPos = normalize(vec3(0.5, 0.5, 0.5));                \n"
    "    mediump vec3 lightVector = normalize(LightPos - positionFrag);         \n"
    "    mediump float diffuse = max(abs(dot(normalFrag, lightVector)), 0.7);   \n"
    "    //diffuse = min(diffuse, 1.0);                                         \n"
    "    diffuse = 1.0;                                                       \n"
    "                                                                           \n"
    "    fragColor = vec4(colorVarying.rgb * diffuse, colorVarying.a);       \n"
    "    //gl_FragColor = vec4(normalFrag, 1.0);                                \n"
    " }                                                                         \n"
    "";
    
    return loadShaders(vShaderSource, fShaderSource);
 
}

void LdrawRender::loadDefaultShaders()
{
    const char *vShaderSource =
    "    attribute vec4 position;\n"
    "    attribute vec3 normal;\n"
    "    attribute vec4 color;\n"
    "                                           \n"
    "    varying mediump vec4 colorVarying;     \n"
    "                                           \n"
    "    varying mediump vec3 normalFrag;       \n"
    "    varying mediump vec3 positionFrag;     \n"
    "                                           \n"
    "    uniform mat4 modelViewProjectionMatrix;\n"
    "    uniform mat4 normalMatrix;             \n"
    "                                           \n"
    "    uniform vec4 color4code1624;           \n"
    "                                           \n"
    "    void main()                                                            \n"
    "    {                                                                      \n"
    "        normalFrag = normalize(vec3(normalMatrix * vec4(normal, 0.0)));    \n"
    "                                                                           \n"
    "        positionFrag = normalize(vec3(normalMatrix * position));           \n"
    "                                                                           \n"
    "        if ( (color.a == 16.0) || (color.a == 24.0) ) {                     \n"
    "            colorVarying = color4code1624;                                 \n"
    "        } else {                                                           \n"
    "            colorVarying = color;                                          \n"
    "        }                                                                  \n"
    "        colorVarying = colorVarying / 255.0;                               \n"
    "        gl_Position = modelViewProjectionMatrix * position;                \n"
    "   }                                                                       \n";

    const char *fShaderSource =
    "varying mediump vec4 colorVarying;     \n"
    "                                       \n"
    "varying mediump vec3 normalFrag;       \n"
    "varying mediump vec3 positionFrag;     \n"
    "                                       \n"
    "void main()                            \n"
    "{                                      \n"
    "    mediump vec3 LightPos = normalize(vec3(0.5, 0.5, 0.5));                \n"
    "    mediump vec3 lightVector = normalize(LightPos - positionFrag);         \n"
    "    mediump float diffuse = max(abs(dot(normalFrag, lightVector)), 0.7);   \n"
    "    diffuse = min(diffuse, 1.0);                                         \n"
    "    //diffuse = max(diffuse, 0.01);                                         \n"
    "    //diffuse = 0.50;                                                       \n"
    "                                                                           \n"
    "    gl_FragColor = vec4(colorVarying.rgb * diffuse, colorVarying.a);       \n"
            //"    gl_FragColor = vec4(colorVarying.rgb * diffuse, 1.0);       \n"
            "    //gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);\n"
            "    //gl_FragColor = vec4(normalFrag, 1.0);                                \n"
            //"    gl_FragColor = colorVarying;                                \n"
    " }                                                                         \n"
    "";

    return loadShaders(vShaderSource, fShaderSource);
}

void LdrawRender::initDraw()
{
    glUseProgram(mProgram);

    //183, 215, 243
    glClearColor(0.7176f, 0.8431f, 0.9529f, 1.0f);

//    glDisable(GL_BLEND);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_TRUE);
    
    glDisable(GL_CULL_FACE);
    //glEnable(GL_CULL_FACE);
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    glEnable(GL_POLYGON_OFFSET_FILL);
//    glPolygonOffset(0.5f, 0.1f);
    glPolygonOffset(0.0001f, 0.0f);

    glLineWidth(1.0f);

    mLastColor1624 = Vector4(0.0, 0.0, 0.0, 0.0);
}

void LdrawRender::setModel(LegoModel *model)
{
    mModel = model;
    initViewMatrix(0);
}

void LdrawRender::renderFrame()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    mModel->drawGL(this);
}

void LdrawRender::renderFrame(LegoPiece *piece, LEGO_PIECE_REF &pieceRef, bool useVAO)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    pieceRef.mMV = lcMul(mViewMatrix, pieceRef.transform);
    pieceRef.mMVP = lcMul(mProjectMatrix, pieceRef.mMV);

    mModel->drawGL(this, piece, pieceRef, useVAO);
}

void LdrawRender::updateTransform()
{
    mModel->updateTransform(mViewMatrix, mProjectMatrix);
}

void LdrawRender::updateShaderUniforms(Vector4 &color1624, Matrix44 &mv, Matrix44 &mvp)
{
    glUniformMatrix4fv(mModelViewProjectMatrixU, 1, 0, mvp);
    Matrix44 nm = Matrix44Transpose(Matrix44Inverse(mv));
    //    glUniformMatrix4fv(mNormalMatrixU, 1, 0, mv);
    glUniformMatrix4fv(mNormalMatrixU, 1, 0, nm);
    if ( color1624 != mLastColor1624 ) {
        glUniform4fv(mColor4Code1624U, 1, color1624);
        mLastColor1624 = color1624;
    }
}

void LdrawRender::updateShaderUniforms(Vector4 &color1624)
{
    glUniform4fv(mColor4Code1624U, 1, color1624);
}


void LdrawRender::setProjectMatrix(int width, int height)
{
    mAspect = (float)width / (float)height;
    //
    // since I have 24bit depth buffer, zFar can be really far
    //
    mProjectMatrix = Matrix44Perspective(mFOV, mAspect, 1, 8000);
    glViewport(0, 0, width, height);
}

void LdrawRender::initViewMatrix(const BoundingBox *boundBox)
{
    mFOV = 60.0f * (float)M_PI / 180.0f;
    const BoundingBox &box = boundBox ? *boundBox : mModel->Bounds();
    float maxVisibleX = box.mMaxPoint.x - box.mMinPoint.x;
    float maxVisibleY = box.mMaxPoint.y - box.mMinPoint.y;
    float maxVisible = fmaxf(maxVisibleX, maxVisibleY) / 2.0f;
    mDistance = maxVisible / tanf(mFOV / 2.0f) + maxVisible / 2.0f;

    //
    // Ensure that there's enough distance on z-axis
    //
    mDistance += box.mMaxPoint.z - box.mMinPoint.z;
    //
    // above calculation will fix X/Y box exactly to viewport
    //
    Vector3 cameraAt = vectorForPolar(mDistance, -(float)M_PI * 0.1f, (float)M_PI * 0.8f);

    mViewCenter = box.mCenter;
    mViewCenter.z = 0.0;

    mCameraPos = cameraAt + mViewCenter;

    //
    // invert Y in "up" vector
    //
    mViewMatrix = Matrix44LookAt(mCameraPos, mViewCenter,
                                 Vector3(0.0, -1.0f, 0.0));
}

void LdrawRender::rotateByXangleYangle(float angleX, float angleY)
{
    angleX = angleX * (float)M_PI / 180.0f;
    angleY = angleY * (float)M_PI / 180.0f;
    
    Vector3 cameraAt = vectorForPolar(mDistance, angleY, angleX);
    mCameraPos = cameraAt + mViewCenter;
    
    mViewMatrix = Matrix44LookAt(mCameraPos, mViewCenter,
                                 Vector3(0.0, -1.0f, 0.0));

}

void LdrawRender::moveByXoffsetYoffset(float offsetX, float offsetY)
{
    //
    // offsets are in model space
    //
    Vector3 translateOrigin(offsetX, offsetY, 0.0);
    mViewCenter = mViewCenter + translateOrigin;
    mViewMatrix = Matrix44LookAt(mCameraPos, mViewCenter,
                                 Vector3(0.0, -1.0f, 0.0));
}

void LdrawRender::setZoomFactor(float factor)
{
    float currentFov = atanf(tanf(mFOV / factor));
    mProjectMatrix = Matrix44Perspective(currentFov, mAspect, 1, 8000);
}


InputProcess::InputProcess(LdrawRender *renderer) :
        mOffset(0.0, 0.0), mCurrentOffset(0.0, 0.0),
        mRotation(0.0, 0.0), mCurrentRotation(0.0, 0.0)
{
    mZoom = 1.0;
    mCurrentZoom = 1.0;
    
    mTargetRenderer = renderer;
    
    //
    // initialize same as initial values for view matrix: -M_PI * 0.1, M_PI * 0.8
    //
    mRotation.x = -(float)M_PI * 0.1f * 180.0f / (float)M_PI;
    mRotation.y = (float)M_PI * 0.8f * 180.0f / (float)M_PI;
    setModeRotate();
}

void InputProcess::setAspect(float w, float h)
{
    mWidth = w;
    mHeight = h;
}

void InputProcess::setModeRotate()
{
    mInputMode = INPUT_MODE_ROTATE;
}

void InputProcess::setModePan()
{
    mInputMode = INPUT_MODE_PAN;
}

void InputProcess::dragStart()
{
}

void InputProcess::dragEnd()
{
    switch (mInputMode) {
        case INPUT_MODE_ROTATE:
            mRotation = mCurrentRotation;
            break;
        case INPUT_MODE_PAN:
            mOffset = mCurrentOffset;
            mCurrentOffset = Vector2(0.0, 0.0);
            break;
        default:
            break;
    }
}

void InputProcess::dragByOffset(float x, float y)
{
    switch (mInputMode) {
        case INPUT_MODE_ROTATE:
            //
            // dragging across the screen will rotate camera 180 degrees.
            // 'X' is inverted, since vertical coordinate in view are
            // inverse to OGL
            //
            mCurrentRotation.x = mRotation.x - 180.0f * y / mHeight;
            mCurrentRotation.y = mRotation.y + 180.0f * x / mWidth;
            
            mTargetRenderer->rotateByXangleYangle(mCurrentRotation.y, mCurrentRotation.x);
            break;
        case INPUT_MODE_PAN:
            //
            // offsets are in viewport plain, but camera moving
            // in model space. Also, offset in cumulative in
            // renderer itself (added to view center)
            //
            if ( cosf(mRotation.y * (float)M_PI / 180.0f) > 0.0 ) {
                x = -x;
            }
            mTargetRenderer->moveByXoffsetYoffset(mCurrentOffset.x - x, mCurrentOffset.y - y);
            mCurrentOffset.x = x;
            mCurrentOffset.y = y;
            break;
        default:
            break;
    }
}

void InputProcess::pitchStart()
{
}

void InputProcess::pitchEnd()
{
    mZoom = mCurrentZoom;
    mCurrentZoom = 1.0;
}

void InputProcess::pitchChangeBy(float scale)
{
    mCurrentZoom = mZoom * scale;
    if ( mCurrentZoom < 0.9 ) {
        mCurrentZoom = 0.9;
    }
    mTargetRenderer->setZoomFactor(mCurrentZoom);
}














