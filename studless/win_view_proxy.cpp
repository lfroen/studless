//
//  win_view_proxy.cpp
//  legoview
//
//  Created by Leonid on 8/8/15.
//  Copyright (c) 2015 lFroen. All rights reserved.
//
#include "lc_global.h"
#include "view.h"
#include "project.h"
#include "lc_mesh.h"

#include "win_view_proxy.h"


lcMainWindow *gMainWindow = 0;

lcMainWindow::lcMainWindow()
{
    
    mActiveView = NULL;
    mTransformType = LC_TRANSFORM_RELATIVE_TRANSLATION;
    
    mColorIndex = lcGetColorIndex(4);
    mTool = LC_TOOL_SELECT;
    mAddKeys = false;
//    mMoveSnapEnabled = true;
//    mAngleSnapEnabled = true;
    mMoveSnapEnabled = false;
    mAngleSnapEnabled = false;
    mMoveXYSnapIndex = 4;
    mMoveZSnapIndex = 3;
    mAngleSnapIndex = 5;
    mLockX = false;
    mLockY = false;
    mLockZ = false;
    mRelativeTransform = true;
    
    memset(&mSearchOptions, 0, sizeof(mSearchOptions));
    
    
    gMainWindow = this;
}

void lcMainWindow::CreateWidgets()
{
    
    View* NewView = new View(NULL);
    //
    // since I don't have Qt, there's no QGLWidget. So, move GL init stuff directly here
    //
//    QWidget* ViewWidget = new lcQGLWidget(CentralFrame, mPiecePreviewWidget, NewView, true);
//    CentralLayout->addWidget(ViewWidget, 0, 0, 1, 1);
//    ViewWidget->setFocus();
    
    lcContext::CreateResources();
    View::CreateResources(NewView->mContext);
    
    gPlaceholderMesh = new lcMesh;
    gPlaceholderMesh->CreateBox();
    
    AddView(NewView);
    
    SetActiveView(NewView);
    
}

bool lcMainWindow::OpenProject(const QString& FileName)
{
//    if (!SaveProjectIfModified())
//        return false;
    
    QString LoadFileName = FileName;
    
    if (LoadFileName.isEmpty())
    {
//        LoadFileName = lcGetActiveProject()->GetFileName();
//        
//        if (LoadFileName.isEmpty())
//            LoadFileName = lcGetProfileString(LC_PROFILE_PROJECTS_PATH);
//        
//        LoadFileName = QFileDialog::getOpenFileName(this, tr("Open Project"), LoadFileName, tr("Supported Files (*.lcd *.ldr *.dat *.mpd);;All Files (*.*)"));
//        
//        if (LoadFileName.isEmpty())
//            return false;

        //
        // I won't get here unless filename passed
        //
        return false;
    }
    
    Project* NewProject = new Project();
    
    if (NewProject->Load(LoadFileName))
    {
        g_App->SetProject(NewProject);
        //AddRecentFile(LoadFileName);
        
        for (int ViewIdx = 0; ViewIdx < mViews.GetSize(); ViewIdx++)
        {
            View* View = mViews[ViewIdx];
            
            if (!View->mCamera->IsSimple())
                View->SetDefaultCamera();
            
            View->ZoomExtents();
        }
        
        UpdateAllViews();
        
        return true;
    }
    
    //QMessageBox::information(this, tr("LeoCAD"), tr("Error loading '%1'.").arg(LoadFileName));
    qDebug() << "Load project [" << LoadFileName << "] failed \n";
    delete NewProject;
    
    return false;
}

void lcMainWindow::UpdateAllViews()
{
}

void lcMainWindow::SetActiveView(View* ActiveView)
{
    if (mActiveView == ActiveView)
        return;
    
    mActiveView = ActiveView;
    
    UpdateCameraMenu();
    UpdatePerspective();
}


void lcMainWindow::AddView(View* View)
{
    mViews.Add(View);
    
    // always current
    //View->MakeCurrent();
    
    if (!mActiveView)
    {
        mActiveView = View;
        UpdatePerspective();
    }
}

//
// original code doesn't do anything except for setting some
// UI state and status text
//
void lcMainWindow::UpdateFocusObject(lcObject* Focus)
{
}

void lcMainWindow::UpdateCameraMenu()
{
}

void lcMainWindow::UpdateCurrentCamera(int CameraIndex)
{
}

void lcMainWindow::UpdatePerspective()
{
}

void lcMainWindow::UpdateTimeline(bool Clear, bool UpdateItems)
{
}

void lcMainWindow::UpdateSelectedObjects(int Flags, int SelectedCount, lcObject* Focus)
{
}