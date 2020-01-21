//
//  legomesh.h
//  
//
//  Created by Leonid on 13/09/2015.
//
//

#ifndef ____legomesh__
#define ____legomesh__

#include <stdio.h>
#include <vector>
#include <string>
#include <map>
#include <list>

#if (__APPLE__)
#include <TargetConditionals.h>
#if (TARGET_OS_IOS)
#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>
#elif (TARGET_OS_MAC)
#import <OpenGL/gl3.h>
#import <OpenGL/gl3ext.h>
#endif
#else
#ifdef __ANDROID_API__
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#else
typedef unsigned int GLuint;
typedef int GLint;
typedef int GLenum;
#endif
#endif

#include "multifile.h"

#include "vec_math.h"

std::string strVector(Vector3 &v);

uint32_t colorForCode(int codeIndex);
uint32_t complementColorForCode(int codeIndex);

#define USE_NORMALS
#define USE_INDEXED_BUFFER_LINES
#define USE_INDEXED_BUFFER_TRI
#define USE_VAO

#define BFC_EN 0

#define DEBUG_GL_BUFFER 0

typedef struct {
    Vector3 position;
#ifdef USE_NORMALS
    Vector3 normal;
#endif
    Vector4U8 color;
} Vertex;

class VertexBuffer {
public:
    VertexBuffer() { }
    std::vector<Vertex> vertexBuffer;
    
    virtual void addVertex(Vector3 &point, uint32_t color);
    virtual void addVertex(Vector3 &point, Vector3 &normal, uint32_t color);
    
    virtual void clone(VertexBuffer &buf);
    
    virtual void copyWithTransform(VertexBuffer &buf, Matrix44 &transform, int colorCode);
    //
    // Invoked when invertnext is set by bfc command
    //
    virtual void copyWithTransformInvertTri(VertexBuffer &buf, Matrix44 &transform, int colorCode);
    
    void *vertexData() { return vertexBuffer.data(); }
    size_t vertexDataSize() { return vertexBuffer.size() * sizeof(Vertex); }
    size_t vertexDataCount() { return vertexBuffer.size(); }

};

class VertexBufferIndexed : public VertexBuffer {
public:
    std::vector<uint32_t> indexBuffer;

    void addVertex(Vector3 &point, uint32_t color);
    void addVertex(Vector3 &point, Vector3 &normal, uint32_t color);

    void *indexData() { return indexBuffer.data(); }
    size_t indexDataSize() { return indexBuffer.size() * sizeof(uint32_t); }
    size_t indexDataCount() { return indexBuffer.size(); }

    void clone(VertexBuffer &buf);
    
    void copyWithTransform(VertexBuffer &buf, Matrix44 &transform, int colorCode);

    //
    // Invoked when invertnext is set by bfc command
    //
    void copyWithTransformInvertTri(VertexBuffer &buf, Matrix44 &transform, int colorCode);
};

class LdrawLib;
class LegoModel;
class LdrawRender;

class BoundingBox {
public:
    Vector3 mMaxPoint, mMinPoint;
    Vector3 mCenter;
    BoundingBox();
    
    void addVertex(Vector3 &v);
};

class LegoPiece {
    char *mName;
    
    //
    // This is _not_ 0-terminated string. This is ptr to beginning of
    // description, in mmapped file. String is terminated by '\n' as
    // LDraw standard says.
    // The idea is that I don't need description until I call for
    // parts report. Only then it will be converted to string (NSString)
    //
    char *mDescriptionPtr, *mDescription;

    //
    // those pointers are set when _model_ is loaded and its VBO
    // created.
    //
    uint8_t *mTriBuffStart;
    uint8_t *mLineBuffStart;
    GLuint mTriVAO, mLineVAO;
    
    BoundingBox mBounds;
    
    bool mBFC;
    
public:
    friend class LegoModel;
#ifdef USE_INDEXED_BUFFER_TRI
    VertexBufferIndexed mTriVertexBuffer;
    size_t mTriIndexOffset;
#else
    VertexBuffer mTriVertexBuffer;
#endif
#ifdef USE_INDEXED_BUFFER_LINES
    VertexBufferIndexed mLineVertexBuffer;
    size_t mLineIndexOffset;
#else
    VertexBuffer mLineVertexBuffer;
#endif
    std::string Name() { return std::string(mName); }
    LegoPiece(const char *name, char *descPtr, VertexBuffer &triBuffer, VertexBuffer &lineBuffer, bool bfc);

//    const char *DescriptionPtr() { return mDescriptionPtr; }
    const char *Description();
    
    void bufferData(size_t &vertexOffset, size_t &bufferOffset, GLuint vao, GLuint ibo);
    
    //
    // when model is deleted, its VBO and IBO will be deleted, and thus
    // all VAO pointing to them are invalidated
    //
    void releaseVAO();
    
    bool bindLineVAO();
    bool bindLTriVAO();
    
    void setGlLineAttribs();
    void setGlTriAttribs();
    
    void updateBoundingBox();
    
    const BoundingBox &Bounds() { return mBounds; }
    
    bool BFC() { return mBFC; }
    
    void debugDumpTri();
};

//
// mMV and mMVP passed as pointers to gl* functions. So it looks like it
// should be aligned to 8 bytes boundary. Otherwise some models fail in
// misterious way
//
typedef struct {
    Vector4 colorVec, complementColorVec;
    Matrix44  __attribute__((aligned(8))) mMV, mMVP;
//    Matrix44  mMV, mMVP;
    Matrix44 transform;
    int colorCode;
    bool hidden;
} LEGO_PIECE_REF;

//
// only sub-model may be unresolved; and while line type "1" contain
// color specification, there's no sense to put there anything but "16"
// in this case.
//
typedef struct {
    uint32_t rgbColor;
    Matrix44 transform;
} LEGO_PIECE_UNRESOLVED_REF;

//
// this is report on used pieces, per color index
// LEGO_MODEL_REPORT[piece ptr] -> [color index] -> count
//
typedef std::map<LegoPiece *, std::map<int, int> > LEGO_MODEL_REPORT;

class LegoModel {
    char *mName;

    int mTotalPieceCount;
    
    std::map<std::string, std::vector<LEGO_PIECE_UNRESOLVED_REF> > mUnresolvedRefs;
    
    std::map<LegoPiece *, std::vector<LEGO_PIECE_REF> > mPiecesMulti;

    BoundingBox mBounds;

    char *readLine_META(char *startPtr, LdrawLib *lib, bool &startNewModel);
    char *readLine_REF(char *startPtr, LdrawLib *lib);
    
    GLuint mVBO, mIBO;
    
    void createBuffers();
    void bufferData();
    
    void updateBoundingBox();
    
    char *loadFromBuffer(char *buffer, LdrawLib *lib);
    void copyWithTransform(LegoModel *subModel, Matrix44 &transform);
    
    //
    // parts moving animation
    //
    typedef struct {
        LegoPiece *piece;
        Vector3 position, origPosition;
        LEGO_PIECE_REF *pieceRef;
        Matrix44 originalTransform;
    } PIECE_REAL_POS;
    std::vector<PIECE_REAL_POS> mPiecesToMove;
    int mMoveHeadIdx = -1;
    int mMoveTailIdx = -1;
    float mMovingSpeed, mAbsMovingSpeed;
public:
    LegoModel(const char *name = 0);

    ~LegoModel();
    
    std::string Name() { return std::string(mName); }

    //
    // loading from ldraw source
    //
    static LegoModel *createFromBuffer(char *buffer, LdrawLib *lib);
    
    //
    // create model with single piece - useful for library debug
    //
    static LegoModel *createWithSinglePiece(LegoPiece *piece, int colorCode);
    
    //
    // create from list after LXF translation
    //
    static LegoModel *createFromConvertedList(std::map<std::string, std::vector<LEGO_PIECE_REF> > &lxfList, LdrawLib *lib);
    
    void resolveFromList(std::map<std::string, LegoModel *> &subModels);
    
    int PieceCount() { return mTotalPieceCount; }
    
    //
    // OpenGL stuff
    void loadGlBuffers();
    void drawGL(LdrawRender *renderer);
    void drawGL(LdrawRender *renderer, LegoPiece *p, LEGO_PIECE_REF &pieceRef, bool useVAO);
    void updateTransform(Matrix44 &view, Matrix44 &project);
   
    const BoundingBox &Bounds() { return mBounds; }

    void createReport(LEGO_MODEL_REPORT &report);
    
    //
    // disassembly animation
    //
    void resetDisassemblyState();
    int getDisassemblyState();
    void setDisassemblyPosition(int pos);
    int moveNextPieceAway();
    int moveNextPieceBack();
    void updateMovingPieces(float timePassed);
    void preparePositionList();
};

class LdrawLib {
    static LdrawLib *sSharedInstance;
public:
    std::map<std::string, LegoPiece *> mPieceLU;
    
    MultiFile *mLdrawFile;
    
    LegoPiece *loadBuffer(char *buffer, const char *name);
    
    class ParseContext {
    public:
        //
        // BFC support
        //
        bool bfcStateKnown;
        bool bfcEnable;
        bool bfcCCW;
        bool bfcCullEn;
        bool bfcInvertNext;
        
        char name[256];

        char *DescriptionPtr;
        
        ParseContext(VertexBuffer &triBuffer, VertexBuffer &lineBuffer) :
        currentTriBuffer(triBuffer), currentLineBuffer(lineBuffer), DescriptionPtr(0)
        {
            name[0] = 0;
            
            bfcStateReset();
        }
        
        void bfcStateReset()
        {
            bfcStateKnown = false;
            bfcCullEn = true;
            bfcCCW = true;
            bfcInvertNext = false;
            bfcEnable = false;
        }
        
        VertexBuffer &currentTriBuffer;
        VertexBuffer &currentLineBuffer;
    };

    char *readLine_META(char *startPtr, ParseContext *ctx);
    char *readLine_REF(char *startPtr, ParseContext *ctx);
    char *readLine_LINE(char *startPtr, ParseContext *ctx);
    char *readLine_TRI(char *startPtr, ParseContext *ctx);
    char *readLine_QUAD(char *startPtr, ParseContext *ctx);
    
public:
    typedef enum { META = 0, REF = 1, LINE = 2, TRI = 3, QUAD = 4, OPT_LINE = 5, INVALID = 6} LDRAW_LINE_TYPE;

    LdrawLib(const char *file);
    
    LegoPiece *lookup(char *name, bool mayFail = false);
    
    static LdrawLib *getSharedInstance() { return sSharedInstance; }
    
    void debugDump();
};

class LdrawRender {
    GLuint mProgram;
    
    GLuint compileShader(GLenum type, const char *source);
    void linkProgram();
    
    float mAspect;
    float mDistance;
    float mFOV;
    
    Matrix44 mViewMatrix;
    Matrix44 mProjectMatrix;
    
    Vector3 mCameraPos;
    Vector3 mViewCenter;
    
    LegoModel *mModel;
    
    Vector4 mLastColor1624;

public:
    GLint mModelViewProjectMatrixU, mNormalMatrixU;
    GLint mColor4Code1624U;
    
    typedef enum {
        VertexAttribPosition,
        VertexAttribNormal,
        VertexAttribColor,
        VertexAttribTexCoord } VertexAttrib;

    void setModel(LegoModel *model);

    void loadShaders(const char *vshader, const char *fshader);
    void loadDefaultShaders();
    void loadDefaultShaders_OSX();
    
    void setProjectMatrix(int width, int height);
    void initViewMatrix(const BoundingBox *boundBox);
    
    void initDraw();
    
    void updateTransform();
    void renderFrame();
    void renderFrame(LegoPiece *piece, LEGO_PIECE_REF &pieceRef, bool useVAO);
    
    void updateShaderUniforms(Vector4 &color1624, Matrix44 &mv, Matrix44 &mvp);
    void updateShaderUniforms(Vector4 &color1624);

    void rotateByXangleYangle(float angleX, float angleY);
    void moveByXoffsetYoffset(float offsetX, float offsetY);
    
    void setZoomFactor(float factor);
    
};

class InputProcess {
    Vector2 mOffset, mCurrentOffset;
    Vector2 mRotation, mCurrentRotation;
    
    float mZoom, mCurrentZoom;
    
    LdrawRender *mTargetRenderer;
    
    typedef enum {INPUT_MODE_PAN, INPUT_MODE_ROTATE} INPUT_MODE_E;
    
    INPUT_MODE_E mInputMode;
    
    float mWidth, mHeight;
public:
    InputProcess(LdrawRender *);
    
    void setAspect(float w, float h);
    
    void setModeRotate();
    void setModePan();
    
    void dragStart();
    void dragEnd();
    void dragByOffset(float x, float y);

    void pitchStart();
    void pitchEnd();
    void pitchChangeBy(float scale);
    
};

#endif /* defined(____legomesh__) */
