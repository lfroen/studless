
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
//  resource-ldr.mm
//  stelport
//
//  Created by leonid on 19/06/10.
//  Copyright Froenchenko Leonid lfroen@gmail.com All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#include "resource-ldr.h"


#include "qtcompat.h"

QString translateString(const QString &s)
{
    NSString *ns = [NSString stringWithCString:s.c_str() encoding:NSUTF8StringEncoding];
    NSString *ls = NSLocalizedString(ns, @"no comment");
    if ( ls.length != 0 ) {
        return QString([ls cStringUsingEncoding:NSUTF8StringEncoding]);
    } else {
        return s;
    }
}

UIImage *makeImageByMask(UIImage *image, NSString *maskName)
{
	UIImage *red = [UIImage imageNamed:maskName];
	
	UIGraphicsBeginImageContext(image.size);
	CGContextRef ctx = UIGraphicsGetCurrentContext();
	CGRect imgRect = CGRectMake(0, 0, image.size.width, image.size.height);
	CGBlendMode bm = kCGBlendModeColor;
	CGContextTranslateCTM(ctx, 0.0, image.size.height);
	CGContextScaleCTM(ctx, 1.0, -1.0);
	CGContextSetBlendMode(ctx, bm);
	CGContextClipToMask(ctx, imgRect, image.CGImage);
	[red drawInRect:imgRect blendMode:bm alpha:1.0];
	
	UIImage *red_im = UIGraphicsGetImageFromCurrentImageContext();
	UIGraphicsEndImageContext();
	return red_im;
}


bool loadPlistFromFile(const char *file, QMap< QString, QMap<QString, QString> > &outData)
{
	NSString *finalPath = [[NSString alloc] initWithUTF8String:file];
	NSDictionary *plistData = [NSDictionary dictionaryWithContentsOfFile:finalPath];
	
	if ( plistData == nil ) {
		NSLog(@"Error loading plist [ %@ ]", finalPath);
		return false;
	}
	
	for (NSString *sectKey in [plistData allKeys]) {
		id sectData = [plistData objectForKey:sectKey];
		QString sskey([sectKey UTF8String]);
		
		QMap<QString, QString> sect_map;
		for (NSString *valKey in [sectData allKeys]) {
			std::string vkey([valKey UTF8String]);
			id valData = [sectData objectForKey:valKey];
			if ( [valData isKindOfClass:[NSString class]] ) {
				QString vdat([valData UTF8String]);
				sect_map[vkey] = vdat;
			} else {
				sect_map[vkey] = "";
			}
		}
		outData[sskey] = sect_map;
	}

    return true;
}



QString locateFile(const char *file)
{
//	NSLog(@"locateFile: requested [%s]", file);
	NSString* resourceFilename;
	if ( file[0] == '/' ) {
		resourceFilename = [[NSString alloc] initWithUTF8String:file];
	} else {
		// Qt resource files
		if ( (file[0] == ':') && (file[1] == '/') ) {
			file += 2;
		}
		resourceFilename = [[NSString alloc] initWithFormat:@"%@/%@",
								  [[NSBundle mainBundle] resourcePath], [[NSString alloc] initWithUTF8String:file] ];
	}
	const char *cptr = [resourceFilename UTF8String];
	QString result(cptr);
	return result;
}

bool queryImageFromFile(const char *file, 
						bool *has_alpha, bool *is_gray, int *w, int *h)
{
	NSString *filename = [[NSString alloc] initWithUTF8String:file];
	
	UIImage* image = [[UIImage alloc] initWithContentsOfFile:filename];
	
	if ( image == nil ) {
		return false;
	}
	CGImage *cgimage = image.CGImage;
	if ( w ) { *w = (int)CGImageGetWidth(cgimage); }
	if ( h ) { *h = (int)CGImageGetHeight(cgimage); }

	if ( is_gray ) {
		CGColorSpaceRef imcs = CGImageGetColorSpace(cgimage);
		size_t imcn = CGColorSpaceGetNumberOfComponents(imcs);
		*is_gray = (imcn == 1);
	}
	
	if ( has_alpha ) {
		CGImageAlphaInfo ai = CGImageGetAlphaInfo(cgimage);
		*has_alpha = (ai != kCGImageAlphaNone);
	}
	return true;
}

QFile *g_cache_file = 0;
typedef struct {
	int size;
	unsigned char *ptr;
} TEX_INFO_ENTRY;
std::map<std::string, TEX_INFO_ENTRY> g_tex_cache;

void discardTexCache()
{
	delete g_cache_file;
	g_tex_cache.clear();
}

void initTexCache()
{
	NSString *infoPath = [[NSString alloc] initWithFormat:@"%@/app_data/texcache.plist",
						[[NSBundle mainBundle] resourcePath] ];
	NSArray *cacheinfo = [NSArray arrayWithContentsOfFile:infoPath];
	
	NSString *dataPath = [[NSString alloc] initWithFormat:@"%@/app_data/texcache.bin",
						  [[NSBundle mainBundle] resourcePath] ];
	g_cache_file = new QFile([dataPath UTF8String]);
	g_cache_file->open(QFile::ReadOnly);
	unsigned char *data = g_cache_file->map(0, 0);
	int total_offset = 0;
	for (NSDictionary *di in cacheinfo) {
		NSString *name = [di objectForKey:@"name"];
		NSNumber *size = [di objectForKey:@"size"];
		//printf("TexCache: adding '%s' size %d\n", [name UTF8String], [size intValue]);
		std::string skey([name UTF8String]);
		TEX_INFO_ENTRY e = { [size intValue], data+total_offset };
		g_tex_cache[skey] = e;
		assert ( g_tex_cache.count(skey) );
		total_offset += [size intValue];
	}
}

NSData *texCacheLU(const char *name)
{
	//printf("TexLU: searching for '%s' - ", name);
	if ( g_tex_cache.count(name) ) {
		TEX_INFO_ENTRY e = g_tex_cache[name];
		NSData *data = [NSData dataWithBytes:e.ptr length:e.size];
		//printf(" found %d bytes\n", e.size);
		return data;
	} else {
//		printf("not found\n");
//		for(std::map<std::string, TEX_INFO_ENTRY>::iterator i = g_tex_cache.begin(); i != g_tex_cache.end(); i++) {
//			printf("\t'%s' %d bytes\n", i->first.c_str(), i->second.size);
//		}
		return 0;
	}
}

unsigned char *loadImageFromFile(const char *file, 
								 bool &has_alpha, bool &is_gray, int &w, int &h)
{
	const char *p = file + strlen(file);
	while (*p != '/') {
		p--;
	}
	NSData *cacheData = texCacheLU(p+1);
	UIImage *image = 0;
	if ( cacheData ) {
		image = [UIImage imageWithData:cacheData];
        //[cacheData release];
	} else {
		NSString *filename = [[NSString alloc] initWithUTF8String:file];
		
		image = [[UIImage alloc] initWithContentsOfFile:filename];
        //[filename release];
	}

	
	if ( image == nil ) {
		NSLog(@"failed to load image [%s]", file);
	}
	
	assert(image);
	
	CGImage *cgimage = image.CGImage;
	
	unsigned char *data = 0;
	if ( cgimage ) {
		w = (int)CGImageGetWidth(cgimage);
		h = (int)CGImageGetHeight(cgimage);
		
		CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
		data = new GLubyte[w * h * 4];
		CGBitmapInfo bi = kCGImageAlphaPremultipliedFirst | kCGBitmapByteOrder32Host;
		
		CGContextRef context = CGBitmapContextCreate(data, w, h, 8, 4 * w, colorSpace, bi);
		CGColorSpaceRelease(colorSpace);
		CGContextTranslateCTM(context, 0.0, h);
		CGContextScaleCTM(context, 1.0, -1.0);
		CGContextSetBlendMode(context, kCGBlendModeCopy);
		CGContextDrawImage(context, CGRectMake(0, 0, w, h), cgimage);
		
		CGColorSpaceRef imcs = CGImageGetColorSpace(cgimage);
		size_t imcn = CGColorSpaceGetNumberOfComponents(imcs);
		is_gray = (imcn == 1);
		CGImageAlphaInfo ai = CGImageGetAlphaInfo(cgimage);
		has_alpha = (ai != kCGImageAlphaNone);
        
        CGContextRelease(context);
	}
//	NSLog(@"resource_ldr: loading image %s ", file);
	return data;
}

QString queryUserDefaults(const QString &section, const QString &key, bool &found)
{
	NSUserDefaults *ud = [NSUserDefaults standardUserDefaults];
	NSString *secname = [NSString stringWithUTF8String:section.c_str()];
	NSDictionary *rd = [ud dictionaryForKey:secname];
	NSString *nskey = [NSString stringWithUTF8String:key.c_str()];
	NSString *vs = [rd valueForKey:nskey];
	if ( vs == nil ) {
//		printf("\nqueryUserDefaults: key [%s] not found in section [%s]\n", key.c_str(), section.c_str());
//		printf("dictionary contents=\n");
//		for(NSString *i in [rd allKeys]) {
//			printf("\t[%s]\t=> [%s]\n", [i UTF8String], [[rd objectForKey:i] UTF8String]);
//		}
		found = false;
		return QString();
	}
	found = true;
	QString resVal([vs UTF8String]);
	return resVal;
}
