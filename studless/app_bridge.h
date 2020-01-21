//
//  app_bridge.h
//  legoview
//
//  Created by Leonid on 8/7/15.
//  Copyright (c) 2015 lFroen. All rights reserved.
//

#ifndef __legoview__app_bridge__
#define __legoview__app_bridge__

#ifdef __cplusplus
extern "C" {
#endif

    void initAppBridge(bool only_obj);
    void setFileToOpen(const char *path);

    void resizeViewport(int w, int h);

    void drawInternal(void);
    
    //
    // "disassembly" animation
    //
    int getModelPieceCount(void);
    int moveNextPieceAway(void);
    int moveNextPieceBack(void);
    void updateMovingPieces(float timePassed);
    void setDisassemblyPosition(int position);
    void resetDisassemblyState(void);
    //
    // UI ifc
    //
    void startDragModel(float startX, float startY);
    void dragModel(float dX, float dY);
    void endDragModel(void);

    void startRotateModel(float startX, float startY);
    void rotateModel(float dX, float dY);

    void zoomModel(float factor);
    void resetZoomModel(void);
    
    void openModelFile(const char *filename);
    
#ifdef __cplusplus
}
#endif

#endif /* defined(__legoview__app_bridge__) */
