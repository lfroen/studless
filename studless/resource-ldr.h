
/*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
//
//  resource-ldr.h
//  stelport
//
//  Created by leonid on 19/06/10.
//  Copyright Froenchenko Leonid lfroen@gmail.com All rights reserved.
//

//
// This is interface to Qt-part since qtcompat.h is not included in objc part.
//
#include "qtcompat.h"

QString locateFile(const char *file);

unsigned char *loadImageFromFile(const char *file, 
								 bool &has_alpha, bool &is_gray, int &w, int &h);

bool queryImageFromFile(const char *file, 
								 bool *has_alpha, bool *is_gray, int *w, int *h);

bool loadPlistFromFile(const char *file, QMap< QString, QMap<QString, QString> > &plistData);

QString queryUserDefaults(const QString &section, const QString &key, bool &found);

QString translateString(const QString &s);

void initTexCache();
void discardTexCache();

#ifndef __OBJC__
class UIImage;
class NSString;
#endif
UIImage *makeImageByMask(UIImage *image, NSString *maskName);
