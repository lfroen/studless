//
//  app_bridge.cpp
//  legoview
//
//  Created by Leonid on 8/7/15.
//  Copyright (c) 2015 lFroen. All rights reserved.
//

#include "app_bridge.h"

#include "lc_global.h"
#include "lc_application.h"
#include "lc_library.h"
#include "win_view_proxy.h"
#include "project.h"
#include "view.h"
#include "piece.h"
#include "multifile.h"
#include "camera.h"

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#include <TargetConditionals.h>

#import "resource-ldr.h"

#include <stdlib.h>

void lcPreferences::LoadDefaults()
{
    //
    // lfroen FIXME: will need to load from ios defaults. Hardcode it
    // from lc_profile.cpp
    //
    mFixedAxes = false;//lcGetProfileInt(LC_PROFILE_FIXED_AXES);
    mMouseSensitivity = 11;//lcGetProfileInt(LC_PROFILE_MOUSE_SENSITIVITY);
    mLightingMode = LC_LIGHTING_FLAT;//(lcLightingMode)lcGetProfileInt(LC_PROFILE_LIGHTING_MODE);
    mDrawAxes = 0;//lcGetProfileInt(LC_PROFILE_DRAW_AXES);
    mDrawEdgeLines = 1;//lcGetProfileInt(LC_PROFILE_DRAW_EDGE_LINES);
    mLineWidth = 1.0;//lcGetProfileFloat(LC_PROFILE_LINE_WIDTH);
    mDrawGridStuds = 1;//lcGetProfileInt(LC_PROFILE_GRID_STUDS);
    mGridStudColor = LC_RGBA(64, 64, 64, 192);//lcGetProfileInt(LC_PROFILE_GRID_STUD_COLOR);
    mDrawGridLines = 1;//lcGetProfileInt(LC_PROFILE_GRID_LINES);
    mGridLineSpacing = 5;//lcGetProfileInt(LC_PROFILE_GRID_LINE_SPACING);
    mGridLineColor = LC_RGBA(0, 0, 0, 255);//lcGetProfileInt(LC_PROFILE_GRID_LINE_COLOR);
}



lcApplication* g_App = 0;
MultiFile *g_MultiFile = 0;

lcApplication::lcApplication()
{
    mProject = NULL;
    mLibrary = NULL;
    mClipboard = NULL;
    
    mPreferences.LoadDefaults();
    
//    mFileOpenOnStartup = "proj0.ldr";
//    mFileOpenOnStartup = "Fair Ground Spinner.ldr";
    mFileOpenOnStartup = "Simple Truck.ldr";
#if (TARGET_IPHONE_SIMULATOR)
    mFileOpenOnStartup = "/Users/lfroen/Downloads/Garbage truck.ldr";
//    mFileOpenOnStartup = "/Users/lfroen/prog/mylego/proj1.ldr";
#endif
//    mFileOpenOnStartup = "/Users/lfroen/Downloads/usps-mail-truck.ldr";
//    mFileOpenOnStartup = "/Users/lfroen/Downloads/Piotreks Hurricane.ldr";
//    mFileOpenOnStartup = "Big Yellow Dump Truck.ldr";
}

lcApplication::~lcApplication()
{
    //delete mProject;
    delete mLibrary;
}

void lcApplication::GetFileList(const char* Path, lcArray<String>& FileList)
{
    GetFileList(PATH_LDRAW_PARTS, 0, FileList);
}

void lcApplication::GetFileList(lfPathType Path, const char* PathPrefix, lcArray<String>& FileList)
{
    FileList.RemoveAll();

    //g_MultiFile->GetFileList(Path, FileList);
}

bool lcApplication::LoadPiecesLibrary(const char* LibPath, const char* LibraryInstallPath, const char* LDrawPath)
{
    if (mLibrary == NULL)
        mLibrary = new lcPiecesLibrary();
    
//    if (LibPath && LibPath[0])
//        return mLibrary->Load(LibPath);
    
//    char* EnvPath = getenv("LEOCAD_LIB");
//    
//    if (EnvPath && EnvPath[0])
//    {
//        return mLibrary->Load(EnvPath);
//    }
    
//    QString CustomPath = "";//lcGetProfileString(LC_PROFILE_PARTS_LIBRARY);
//    
//    if (!CustomPath.isEmpty())
//        return mLibrary->Load(CustomPath.toLatin1().constData()); // todo: qstring
    
//    if (LibraryInstallPath && LibraryInstallPath[0])
//    {
//        char LibraryPath[LC_MAXPATH];
//        
//        strcpy(LibraryPath, LibraryInstallPath);
//        
//        size_t i = strlen(LibraryPath) - 1;
//        if ((LibraryPath[i] != '\\') && (LibraryPath[i] != '/'))
//            strcat(LibraryPath, "/");
//        
//        strcat(LibraryPath, "library.bin");
//        
//        if (mLibrary->Load(LibraryPath))
//        {
//            mLibrary->SetOfficialPieces();
//            return true;
//        }
//    }
    
    if (LDrawPath && LDrawPath[0])
    {
//        char LibraryPath[LC_MAXPATH];
//        
//        strcpy(LibraryPath, LDrawPath);
//        
//        size_t i = strlen(LibraryPath) - 1;
//        if ((LibraryPath[i] != '\\') && (LibraryPath[i] != '/'))
//            strcat(LibraryPath, "/");
        
        if (mLibrary->Load(LDrawPath))
            return true;
    }
    
    return false;
}

void lcApplication::SetProject(Project* Project)
{
    if ( mProject ) {
        delete mProject;
    }
    mProject = Project;
    
    const lcArray<View*>& Views = gMainWindow->GetViews();
    for (int ViewIdx = 0; ViewIdx < Views.GetSize(); ViewIdx++)
    {
        View* View = Views[ViewIdx];
        View->ClearCameras();
        View->SetModel(lcGetActiveModel());
    }
    
    Project->SetActiveModel(0);
    lcGetPiecesLibrary()->RemoveTemporaryPieces();
}

bool lcApplication::Initialize(int argc, char* argv[], const char* LibraryInstallPath, const char* LDrawPath)
{
    char* LibPath = NULL;
    
    gMainWindow = new lcMainWindow();

    if (!LoadPiecesLibrary(LibPath, LibraryInstallPath, LDrawPath)) {
            fprintf(stderr, "ERROR: Cannot load pieces library.");
            return false;
    }

    //
    // gles init happens here. Window (main) contain gl_QGLwidgets. Each widget have (pointer to?!) view. The
    // view iteself inherit from lcGlWidget. QGLWidget is Qt stuff, while lcGLWidget is just utility class.
    // This stuff can be seen at lcMainWindow::CreateWidgets() where it's created.
    //
    gMainWindow->CreateWidgets();
    
    mProject = 0;

    return true;
}
/*
vector3D triangleNormalFromVertex(int face_id, int vertex_id) {
    //This assumes that A->B->C is a counter-clockwise ordering
    vector3D A = mesh.face[face_id].vertex[vertex_id];
    vector3D B = mesh.face[face_id].vertex[(vertex_id+1)%3];
    vector3D C = mesh.face[face_id].vertex[(vertex_id+2)%3];
    
    
    vector3D N = cross(B-A,C-A);
    float sin_alpha = length(N) / (length(B-A) * length(C-A) );
    return normalize(N) * asin(sin_alpha);
}
*/
void normalForTriangle(lcVector3 &A, lcVector3 &B, lcVector3 &C, lcVector3 &N)
{
    lcVector3 vBA = B - A;
    lcVector3 vCA = C - A;
    N = lcCross(vBA, vCA);
    //N = lcCross(vCA, vBA);
//    float sin_alpha = N.Length() / ( vBA.Length() * vCA.Length() );
//    N *= asin(sin_alpha);
//    N.Normalize();
}

FILE *g_debugLog = stdout;
void bufferDump(void *buffStart, int startIndex, int vertexCount)
{
    lcVertex *vertexData = (lcVertex *)buffStart;
    fprintf(g_debugLog, "dump[%p startIdx %d count %d] = \n", buffStart, startIndex, vertexCount);
    for (int i = startIndex; i < vertexCount; i++) {
        lcVector3 &A = vertexData[i].Position;
        fprintf(g_debugLog, "%d (%d) = %s \n", i - startIndex, i, A.tostr());
    }
    fprintf(g_debugLog, "\n");
}

template <typename I>
void recalcForSectionAndIndexType(lcMesh *Mesh, lcMeshSection *Section) {
    lcVertex *vertexData = (lcVertex *)Mesh->mVertexData;
    int vertexDataSize = Mesh->mVertexDataSize;
    I *indexp = (I *)((unsigned char *)Mesh->mIndexData + Section->IndexOffset);
    //
    // now for each triangle
    //
    assert((Section->NumIndices % 3) == 0);
    for(int i = 0; i <  Section->NumIndices / 3; i++) {
        int indexA = *(indexp + i * 3 + 0);
        int indexB = *(indexp + i * 3 + 1);
        int indexC = *(indexp + i * 3 + 2);
        assert(indexA*sizeof(lcVertex) < vertexDataSize);
        assert(indexB*sizeof(lcVertex) < vertexDataSize);
        assert(indexC*sizeof(lcVertex) < vertexDataSize);
//        lcVector3 &A = vertexp[indexA].Position;
//        lcVector3 &B = vertexp[indexB].Position;
//        lcVector3 &C = vertexp[2].Position;
        lcVertex &A = vertexData[indexA];
        lcVertex &B = vertexData[indexB];
        lcVertex &C = vertexData[indexC];
        lcVector3 N;
        normalForTriangle(A.Position, B.Position, C.Position, N);
        A.Normal = N;
        B.Normal = N;
        C.Normal = N;
//        vertexp[0].Normal = N;
//        vertexp[1].Normal = N;
//        vertexp[2].Normal = N;
    }
}

void recalcNormalsForRenderMesh(lcRenderMesh &RenderMesh)
{
    lcMesh* Mesh = RenderMesh.Mesh;
    int LodIndex = RenderMesh.LodIndex;
    dispatch_apply(Mesh->mLods[LodIndex].NumSections,
                   dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^(size_t SectionIdx) {
        lcMeshSection* Section = &Mesh->mLods[LodIndex].Sections[SectionIdx];
        
        if (Section->PrimitiveType == GL_TRIANGLES) {
            if (Mesh->mIndexType == GL_UNSIGNED_SHORT) {
                recalcForSectionAndIndexType<uint16_t>(Mesh, Section);
            } else {
                recalcForSectionAndIndexType<uint32_t>(Mesh, Section);
            }
        }
    });
                   
//    for (int SectionIdx = 0; SectionIdx < Mesh->mLods[LodIndex].NumSections; SectionIdx++) {
//        lcMeshSection* Section = &Mesh->mLods[LodIndex].Sections[SectionIdx];
//        
//        if (Section->PrimitiveType == GL_TRIANGLES) {
//            if (Mesh->mIndexType == GL_UNSIGNED_SHORT) {
//                recalcForSectionAndIndexType<uint16_t>(Mesh, Section);
//            } else {
//                recalcForSectionAndIndexType<uint32_t>(Mesh, Section);
//            }
//        }
//    }
}

void recalcNormals(lcModel *Model)
{
    lcScene mScene;
    lcCamera Camera(true);
    Model->GetScene(mScene, &Camera, false);
    lcVector3 zeroNormal(0, 0, 0);
    for (int MeshIdx = 0; MeshIdx < mScene.mOpaqueMeshes.GetSize(); MeshIdx++) {
        lcRenderMesh& RenderMesh = mScene.mOpaqueMeshes[MeshIdx];
        recalcNormalsForRenderMesh(RenderMesh);
    }
    for (int MeshIdx = 0; MeshIdx < mScene.mTranslucentMeshes.GetSize(); MeshIdx++) {
        lcRenderMesh& RenderMesh = mScene.mTranslucentMeshes[MeshIdx];
        recalcNormalsForRenderMesh(RenderMesh);
    }
}

//
// If app is launched by "Open in..." action, then application
// object should be created _before_ view, so we can remember what
// file to load. At this time, however view is not available and gles
// context can not be created.
//
void initAppBridge(bool only_obj)
{
    assert(!only_obj || (g_App == 0));
    assert(!only_obj || (g_MultiFile == 0));
    
    if ( !g_App ) {
        g_MultiFile = new MultiFile(locateFile("library.bin").constData());
        g_App = new lcApplication();
    }
    if ( !only_obj ) {
        g_App->Initialize(0, 0, 0, "/Users/lfroen/prog/mylego/ldraw");

        lcContext *Context = gMainWindow->GetActiveView()->mContext;
        Context->SetDefaultState();

        //
        // in simulator I may want to access file on local filesystem
        //
#if (TARGET_IPHONE_SIMULATOR)
        if ( g_App->mFileOpenOnStartup[0] == '/' ) {
            openModelFile(g_App->mFileOpenOnStartup);
        } else {
            openModelFile(locateFile(g_App->mFileOpenOnStartup).constData());
        }
#else
        openModelFile(locateFile(g_App->mFileOpenOnStartup).constData());
#endif
//        openModelFile("/Users/lfroen/Dropbox/Lego/Akiyuki Ball Factory.mpd");
#if (TARGET_IPHONE_SIMULATOR)
        g_debugLog = fopen("/Users/lfroen/debug.log", "w");
#endif
        assert(g_debugLog);
        //recalcNormals(gMainWindow->GetActiveView()->mModel, gMainWindow->GetActiveView()->mCamera);
        //fclose(g_debugLog);
//        lcGetPiecesLibrary()->UpdateBuffers(Context);

    }
}

void setFileToOpen(const char *path)
{
    assert(g_App);
    g_App->mFileOpenOnStartup = path;
}

void openModelFile(const char *file)
{
    //printf("%s: loading model from[%s]\n", __func__, file);
    lcContext *Context = gMainWindow->GetActiveView()->mContext;
    Context->SetDefaultState();
//    Context->UnbindMesh();
//    glDisableVertexAttribArray(LC_ATTRIB_POSITION);
//    glDisableVertexAttribArray(LC_ATTRIB_NORMAL);
    gMainWindow->OpenProject(file);
    //recalcNormals(gMainWindow->GetActiveView()->mModel, gMainWindow->GetActiveView()->mCamera);
    recalcNormals(gMainWindow->GetActiveView()->mModel);
    lcGetPiecesLibrary()->UpdateBuffers(Context);
    glBindBuffer(GL_ARRAY_BUFFER, lcGetPiecesLibrary()->mVertexBuffer.Object);
    CHECK_GL_ERRORS_C();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lcGetPiecesLibrary()->mIndexBuffer.Object);
    CHECK_GL_ERRORS_C();

//    Context->SetViewMatrix(gMainWindow->GetActiveView()->mCamera->mWorldView);
}

char *getFileFromLDrawLib(char *name)
{
    //printf("%s: looking for %s\n", __func__, name);
    char *data = g_MultiFile->lookupByName(name);
    
    assert(data);
    
    return data;
}

void resizeViewport(int w, int h)
{
    View *v = gMainWindow->GetActiveView();
    v->mWidth = w;
    v->mHeight = h;
    v->SetViewpoint(LC_VIEWPOINT_HOME);
//    v->SetViewpoint(LC_VIEWPOINT_TOP);
}

void drawInternal()
{
    View *v = gMainWindow->GetActiveView();
    v->OnDraw();
}

lcArray<lcPiece*> g_piecesToMove;
int g_headIdx = -1;
int g_tailIdx = -1;
float g_movingSpeed;

int comparePiecesZPosition(const void *FirstP, const void * SecondP)
{
    lcPiece *First = *(lcPiece **)FirstP;
    lcPiece *Second = *(lcPiece **)SecondP;
    //printf("cmp: [%s] <> [%s]", First->GetName(), Second->GetName());
    return -(First->mModelWorld.GetTranslation().z - Second->mModelWorld.GetTranslation().z);
}

int getModelPieceCount()
{
    View *v = gMainWindow->GetActiveView();
    return v->mModel->GetPieces().GetSize();
}

void resetDisassemblyState()
{
    g_headIdx = -1;
    g_tailIdx = -1;
    g_piecesToMove.RemoveAll();
}

//
// Position of slider is position of _head_
//
void setDisassemblyPosition(int pos)
{
    View *v = gMainWindow->GetActiveView();
    if ( pos == g_headIdx ) {
        return;
    }
    if ( pos > g_headIdx ) {
        g_headIdx = pos;
        g_movingSpeed = v->mHeight / 3.0;
    } else {
        g_movingSpeed = -v->mHeight / 3.0;
        if ( pos  <= g_tailIdx ) {
            //
            // eventually tail == head - 1
            //
            g_tailIdx = pos - 1;
        } else {
            //
            // this is tricky part. Some parts [tail:pos] are supposed
            // to be removed, while others [pos+1, head] need to be
            // put back. Since I only want to animate movement to 1
            // direction, "put back" part will be instant
            //
            for (int i = pos; i < g_headIdx; i++) {
                lcPiece *piece = g_piecesToMove[i];
                piece->mModelWorld.SetTranslation(piece->mOriginalPosition);
                piece->SetHidden(false);
            }
        }
    }
}

int moveNextPieceAway()
{
    View *v = gMainWindow->GetActiveView();
    //
    // init - copy pieces array from
    // model, remember original location
    //
    if ( g_headIdx == -1 ) {
        g_piecesToMove = v->mModel->GetPieces();
        g_piecesToMove.Sort(comparePiecesZPosition);
        for (int i = 0; i < g_piecesToMove.GetSize(); i++) {
            g_piecesToMove[i]->SavePositionBeforeMove();
        }
        g_headIdx = 0;
    }
    if ( g_headIdx < g_piecesToMove.GetSize() ) {
        g_headIdx++;
    }
    g_movingSpeed = v->mHeight / 3.0;

    return g_headIdx;
}

int moveNextPieceBack()
{
    if ( g_tailIdx > -1 ) {
        lcPiece *movingPiece = g_piecesToMove[g_tailIdx];
        movingPiece->SetHidden(false);
        g_tailIdx--;
    }
    View *v = gMainWindow->GetActiveView();
    g_movingSpeed = -v->mHeight / 3.0;
    return g_tailIdx+1;
}

void updateMovingPieces(float timePassed)
{
    if ( (g_headIdx - g_tailIdx) <= 1 ) {
        return;
    }
    View *v = gMainWindow->GetActiveView();
    lcVector3 Distance = {0.0, 0.0, g_movingSpeed*timePassed};
    int _1st_moving = g_tailIdx + 1;
    int last_moving = g_headIdx;
//    int stop_from = 0x7fffffff, stop_to = -1;
//    printf("update moving: %d to %d\n", _1st_moving, last_moving);
    for (int i = _1st_moving; i < last_moving; i++) {
        lcPiece *movingPiece = g_piecesToMove[i];
        lcVector3 Position = movingPiece->mModelWorld.GetTranslation();
        Position += Distance;

        if ( g_movingSpeed < 0.0 ) {
            //
            // moving backward
            //
            if ( Position.z < movingPiece->mOriginalPosition.z) {
                Position.z = movingPiece->mOriginalPosition.z;
                //
                // as in case with tail - ensure that no mantter what,
                // every piece end in its place
                //
                if ( g_headIdx < g_piecesToMove.GetSize() ) {
                    g_piecesToMove[g_headIdx]->RestoreOriginalPosition();
                }
                g_headIdx--;
            }
            movingPiece->mModelWorld.SetTranslation(Position);
        } else {
            //
            // moving forward
            //
            movingPiece->mModelWorld.SetTranslation(Position);
            lcVector3 ViewPosition = v->ProjectPoint(Position);
            if ( (ViewPosition.y > v->mHeight) ) {
                movingPiece->SetHidden(true);
                //
                // ensure that in any case, no matter in which
                // order parts reach end-of-screen, they all
                // end hidden. It can happen that "tail+1" will
                // finish _before_ "tail"
                //
                if ( g_tailIdx >= 0 ) {
                    g_piecesToMove[g_tailIdx]->SetHidden(true);
                }
                g_tailIdx++;
            }
        }
    }
}


void resetZoomModel()
{
    View *v = gMainWindow->GetActiveView();
    v->ZoomExtents();
}

void zoomModel(float factor)
{
    View *v = gMainWindow->GetActiveView();
    //factor = (factor > 1.0) ? 0 : 0;
    //printf("%s: orthoheight=%.3f\n", __func__, v->mCamera->GetOrthoHeight());
    factor *= v->mCamera->GetOrthoHeight() / 300.0;
    v->mModel->Zoom(v->mCamera, factor);
    //v->mModel->ZoomExtents(v->mCamera, factor);
    //v->ZoomByFactor(factor);
}

void startRotateModel(float startX, float startY)
{
    View *v = gMainWindow->GetActiveView();
    v->mInputState.x = startX;
    v->mInputState.y = v->mHeight - startY;
    v->mTrackTool = LC_TRACKTOOL_ORBIT_XY;
    v->StartTracking(LC_TRACKBUTTON_LEFT);
    
    lcPiece* HitPiece = (lcPiece*)v->FindObjectUnderPointer(true).Object;
    if ( !HitPiece ) {
        lcArray<lcObject*> objectsInView = v->FindObjectsInBox(0.0, 0.0, v->mWidth, v->mHeight);
        for (int i = 0; i < objectsInView.GetSize(); i++) {
            lcObject *o = objectsInView[i];
            if ( o->IsPiece() ) {
                HitPiece = (lcPiece *)o;
                break;
            }
        }
    }
    if ( HitPiece ) {
        v->mRotateCenter = HitPiece->GetSectionPosition(LC_PIECE_SECTION_POSITION);
    } else {
        v->mRotateCenter = lcVector3(0, 0, 0);
    }
}

void rotateModel(float dX, float dY)
{
    View *v = gMainWindow->GetActiveView();
    const float MouseSensitivity = 1.0f / (21.0f - lcGetPreferences().mMouseSensitivity);

//    v->mModel->UpdateOrbitTool(v->mCamera,
//                               0.1f * MouseSensitivity * (/*mInputState.x - v->mMouseDownX*/dX),
//                               0.1f * MouseSensitivity * (/*mInputState.y - v->mMouseDownY*/-dY));
//
    {
        lcCamera *Camera = v->mCamera;
        lcModel *Model = v->mModel;
        float MouseX = 0.1f * MouseSensitivity * dX;
        float MouseY = -0.1f * MouseSensitivity * dY;
        Camera->Orbit(MouseX - Model->mMouseToolDistance.x, MouseY - Model->mMouseToolDistance.y,
                      v->mRotateCenter, 0, gMainWindow->GetAddKeys());
        Model->mMouseToolDistance.x = MouseX;
        Model->mMouseToolDistance.y = MouseY;
        gMainWindow->UpdateAllViews();

    }
}

void startDragModel(float startX, float startY)
{
    View *v = gMainWindow->GetActiveView();
    v->mInputState.x = startX;
    v->mInputState.y = v->mHeight - startY;
    v->mTrackTool = LC_TRACKTOOL_PAN;
    v->StartTracking(LC_TRACKBUTTON_LEFT);
}

void dragModel(float dX, float dY)
{
    View *v = gMainWindow->GetActiveView();

    float X = v->mMouseDownX + dX;
    float Y = v->mMouseDownY - dY;
    
    //
    // that shit taken from case LC_TRACKTOOL_PAN
    //
    {
        lcVector3 Points[4] =
        {
            lcVector3(X/*(float)mInputState.x*/, Y/*(float)mInputState.y*/, 0.0f),
            lcVector3(X/*(float)mInputState.x*/, Y/*(float)mInputState.y*/, 1.0f),
            lcVector3(v->mMouseDownX, v->mMouseDownY, 0.0f),
            lcVector3(v->mMouseDownX, v->mMouseDownY, 1.0f)
        };
        
        v->UnprojectPoints(Points, 4);
        
        const lcVector3& CurrentStart = Points[0];
        const lcVector3& CurrentEnd = Points[1];
        const lcVector3& MouseDownStart = Points[2];
        const lcVector3& MouseDownEnd = Points[3];
        lcVector3 Center = v->mModel->GetSelectionOrModelCenter();
        
        lcVector3 PlaneNormal(v->mCamera->mPosition - v->mCamera->mTargetPosition);
        lcVector4 Plane(PlaneNormal, -lcDot(PlaneNormal, Center));
        lcVector3 Intersection;
        
        if (lcLinePlaneIntersection(&Intersection, CurrentStart, CurrentEnd, Plane))
        {
            lcVector3 MoveStart;
            
            if (lcLinePlaneIntersection(&MoveStart, MouseDownStart, MouseDownEnd, Plane))
            v->mModel->UpdatePanTool(v->mCamera, MoveStart - Intersection);
        }
    }

}

void endDragModel()
{
    View *v = gMainWindow->GetActiveView();
    v->StopTracking(true);
}



