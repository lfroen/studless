//
//  win_view_proxy.h
//  legoview
//
//  Created by Leonid on 8/8/15.
//  Copyright (c) 2015 lFroen. All rights reserved.
//

#ifndef __legoview__win_view_proxy__
#define __legoview__win_view_proxy__

#include "lc_model.h"

class View;

//
// stuff from lc_mainwindow.h - lot of code rely on pointer to "main window", so
// I will supply such pointer.
//
struct lcSearchOptions
{
    bool MatchInfo;
    bool MatchColor;
    bool MatchName;
    PieceInfo* Info;
    int ColorIndex;
    char Name[256];
};

class lcMainWindow //: public QMainWindow
{
    Q_OBJECT
    
public:
    lcMainWindow();
    ~lcMainWindow();
    
    void CreateWidgets();
    
    lcTool GetTool() const
    {
        return mTool;
    }
    
    lcTransformType GetTransformType() const
    {
        return mTransformType;
    }
    
    bool GetAddKeys() const
    {
        return mAddKeys;
    }
    
    float GetMoveXYSnap() const
    {
        const float SnapXYTable[] = { 0.0f, 1.0f, 5.0f, 8.0f, 10.0f, 20.0f, 40.0f, 60.0f, 80.0f, 160.0f };
        return mMoveSnapEnabled ? SnapXYTable[mMoveXYSnapIndex] : 0.0f;
    }
    
    float GetMoveZSnap() const
    {
        const float SnapZTable[] = { 0.0f, 1.0f, 5.0f, 8.0f, 10.0f, 20.0f, 24.0f, 48.0f, 96.0f, 192.0f };
        return mMoveSnapEnabled ? SnapZTable[mMoveZSnapIndex] : 0.0f;
    }
    
    int GetAngleSnap() const
    {
        const int AngleTable[] = { 0, 1, 5, 10, 15, 30, 45, 60, 90, 180 };
        return mAngleSnapEnabled ? AngleTable[mAngleSnapIndex] : 0.0f;
    }
    
    QString GetMoveXYSnapText() const
    {
        QString SnapXYText[] = { tr("0"), tr("1/20S"), tr("1/4S"), tr("1F"), tr("1/2S"), tr("1S"), tr("2S"), tr("3S"), tr("4S"), tr("8S") };
        return mMoveSnapEnabled ? SnapXYText[mMoveXYSnapIndex] : tr("None");
    }
    
    QString GetMoveZSnapText() const
    {
        QString SnapZText[] = { tr("0"), tr("1/20S"), tr("1/4S"), tr("1F"), tr("1/2S"), tr("1S"), tr("1B"), tr("2B"), tr("4B"), tr("8B") };
        return mMoveSnapEnabled ? SnapZText[mMoveZSnapIndex] : tr("None");
    }
    
    QString GetAngleSnapText() const
    {
        return mAngleSnapEnabled ? QString::number(GetAngleSnap()) : tr("None");
    }
    
    bool GetLockX() const
    {
        return mLockX;
    }
    
    bool GetLockY() const
    {
        return mLockY;
    }
    
    bool GetLockZ() const
    {
        return mLockZ;
    }
    
    bool GetRelativeTransform() const
    {
        return mRelativeTransform;
    }
    
    View* GetActiveView() const
    {
        return mActiveView;
    }
    
    const lcArray<View*>& GetViews()
    {
        return mViews;
    }
    
    void ResetCameras();
    void AddView(View* View);
    void RemoveView(View* View);
    void SetActiveView(View* ActiveView);
    void UpdateAllViews();
    
    void SetTool(lcTool Tool);
    void SetTransformType(lcTransformType TransformType);
    void SetColorIndex(int ColorIndex);
    void SetMoveSnapEnabled(bool Enabled);
    void SetAngleSnapEnabled(bool Enabled);
    void SetMoveXYSnapIndex(int Index);
    void SetMoveZSnapIndex(int Index);
    void SetAngleSnapIndex(int Index);
    void SetLockX(bool LockX);
    void SetLockY(bool LockY);
    void SetLockZ(bool LockZ);
    void SetRelativeTransform(bool RelativeTransform);
    
    void NewProject();
    bool OpenProject(const QString& FileName);
    void MergeProject();
    bool SaveProject(const QString& FileName);
    bool SaveProjectIfModified();
    void SetModelFromFocus();
//    void HandleCommand(lcCommandId CommandId);
    
    void AddRecentFile(const QString& FileName);
    void RemoveRecentFile(int FileIndex);
    
    void SplitHorizontal();
    void SplitVertical();
    void RemoveActiveView();
    void ResetViews();
    
    void TogglePrintPreview();
    void ToggleFullScreen();
    
    void UpdateFocusObject(lcObject* Focus);
    void UpdateSelectedObjects(int Flags, int SelectedCount, lcObject* Focus);
    void UpdateTimeline(bool Clear, bool UpdateItems);
    void UpdatePaste(bool Enabled);
    void UpdateCurrentStep();
    void SetAddKeys(bool AddKeys);
    void UpdateLockSnap();
    void UpdateSnap();
    void UpdateColor();
    void UpdateUndoRedo(const QString& UndoText, const QString& RedoText);
    void UpdateCurrentCamera(int CameraIndex);
    void UpdatePerspective();
    void UpdateCameraMenu();
    void UpdateModels();
    void UpdateCategories();
    void UpdateTitle();
    void UpdateModified(bool Modified);
    void UpdateRecentFiles();
    void UpdateShortcuts();
    
    lcVector3 GetTransformAmount();
    
    int mColorIndex;
    lcSearchOptions mSearchOptions;
    
    protected:
    void ClipboardChanged();
    void ActionTriggered();
    void ColorChanged(int ColorIndex);
    void PartSearchReturn();
    void PartSearchChanged(const QString& Text);
    
protected:
    
    View* mActiveView;
    lcArray<View*> mViews;
    
    bool mAddKeys;
    lcTool mTool;
    lcTransformType mTransformType;
    bool mMoveSnapEnabled;
    bool mAngleSnapEnabled;
    int mMoveXYSnapIndex;
    int mMoveZSnapIndex;
    int mAngleSnapIndex;
    bool mLockX;
    bool mLockY;
    bool mLockZ;
    bool mRelativeTransform;
    
};

extern class lcMainWindow* gMainWindow;

#endif /* defined(__legoview__win_view_proxy__) */
