//
//  ColouredShape2d.h
//  QLT_GenomeLaser
//
//  Created by say nono on 28.07.13.
//  Copyright (c) 2013 http://say-nono.com. All rights reserved.
//

#pragma once


#include "cinder/app/App.h"
#include "cinder/Shape2d.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class ColouredShape2d: public Shape2d{

public:
    
    
    ColouredShape2d();
    
    void    clear();
    
	void    color( ColorAf clr );
    const ColorAf getCurrentColor();
    vector<ColorAf> getColors() const;
    const ColorAf getSegmentColor(int seg);
    
    void    appendColouredShape2d(const ColouredShape2d& path);
    void    appendShape2d(const Shape2d& path);
    void    appendPath2d(const Path2d& path);
    void    addColoursToPath(const Path2d& path, ColorA clr);

    
    void	moveTo( const vec2 &p );
	void	moveTo( float x, float y );
	void	lineTo( const vec2 &p );
	void	lineTo( float x, float y );
	void	quadTo( const vec2 &p1, const vec2 &p2 );
	void	quadTo( float x1, float y1, float x2, float y2 );
	void	curveTo( const vec2 &p1, const vec2 &p2, const vec2 &p3 );
	void	curveTo( float x1, float y1, float x2, float y2, float x3, float y3 );
	void	arc( const vec2 &center, float radius, float startRadians, float endRadians, bool forward = true );
	void	arc( float centerX, float centerY, float radius, float startRadians, float endRadians, bool forward = true );
	void	arcTo( const vec2 &p, const vec2 &t, float radius );
	void	arcTo( float x, float y, float tanX, float tanY, float radius);

    
    
    void                draw();

    vector<ColorAf>    mSegmentColours;

    
private:
	
//    vector<ColorAf>    mSegmentColours;
    ColorAf            mCurrentColour;
    bool               bColorSet;
	
};



