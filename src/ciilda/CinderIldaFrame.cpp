//
//  CinderIldaFrame.cpp
//  LaserGateway
//
//  Created by say nono on 27.06.13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "CinderIldaFrame.h"


namespace ciilda {

    Frame::Frame() {
        setDefaultParams();
    }

    void Frame::setDefaultParams() {
        memset(&params, 0, sizeof(params));  // safety catch all default to zero
        memset(&stats, 0, sizeof(stats));  // safety catch all default to zero
        
        params.draw.lines = true;
        params.draw.points = true;
        params.draw.pointNumbers = false;
        
        
        params.output.color = params.output.color.white();//.set(1.0f, 1.0f, 1.0f, 1.0f);
        params.output.blankCount = 10;
        params.output.endCount = 3;
        params.output.doCapX = false;
        params.output.doCapY = false;
        
        params.output.targetPointCount = 400;
        params.output.spacing = 0;
        
        params.output.transform.doFlipX = false;
        params.output.transform.doFlipY = false;
        params.output.transform.offset.set(0, 0);
        params.output.transform.scale.set(1, 1);
        
        mCurrentColor = ColorA::white();
        
    }

    //--------------------------------------------------------------
    string Frame::getString() {
        stringstream s;
//        s << polyProcessor.getString();
        
        s << "params:" << endl;
        s << "draw.lines : " << params.draw.lines << endl;
        s << "draw.point : " << params.draw.points << endl;
        s << "draw.pointNumbers : " << params.draw.pointNumbers << endl;
        
        s << "output.color : " << params.output.color << endl;
        s << "output.blankCount : " << params.output.blankCount << endl;
        s << "output.endCount : " << params.output.endCount << endl;
        s << "output.doCapX : " << params.output.doCapX << endl;
        s << "output.doCapY : " << params.output.doCapY << endl;
        s << "output.transform.doFlipX : " << params.output.transform.doFlipX << endl;
        s << "output.transform.doFlipY : " << params.output.transform.doFlipY << endl;
        s << "output.transform.offset : " << params.output.transform.offset << endl;
        s << "output.transform.scale : " << params.output.transform.scale << endl;
        
        s << endl;
        
        s << "stats:" << endl;
        s << "stats.pointCountOrig : " << stats.pointCountOrig << endl;
        s << "stats.pointCountProcessed : " << stats.pointCountProcessed << endl;
        
        return s.str();
    }
    
    //--------------------------------------------------------------
    void Frame::update() {
        updateFinalPoints();
    }
    
    //--------------------------------------------------------------
    void Frame::draw(float x, float y, float w, float h) {
        if(w==0) w = getWindowWidth();
        if(h==0) h = getWindowHeight();
        
        float sw = w / (float) kIldaDimension;
        float sh = h / (float) kIldaDimension;
        
        float cx = lerp(x, x+w, .5);
        float cy = lerp(y, y+h, .5);

        float scale = 1000.0f;
        
        Shape2d shapeDraw = origShape;
        shapeDraw.scale(Vec2f(scale,scale));
        
        gl::pushMatrices();
        gl::translate(x, y);
        gl::scale(w/scale, h/scale);
        gl::draw( shapeDraw );
        gl::popMatrices();
        
        gl::pushMatrices();
        gl::translate(cx, cy);
        gl::scale(sw, sh);
        
        
        if(params.draw.lines) {
            gl::lineWidth(2);
//            ofSetLineWidth(2);
//            for(int i=0; i<processedPolys.size(); i++) {
//                ofPolyline &poly = processedPolys[i];
//                ColorA &pcolor = processedPolys[i].color;
//                ofSetColor(pcolor.r*255, pcolor.g*255, pcolor.b*255);
//                poly.draw();
//                //            for(int i=0; i<data.size(); i++) {
//                //                ofPoint p0 = data[i];
//                //                if(i < data.size()-1) {
//                //                    ofPoint p1 = data[i+1];
//                ////                    ofSetColor(p1.r * 255, p1.g * 255, p1.b * 255, p1.a * 255);
//                //                    ofLine(p0.x, p0.y, p1.x, p1.y);
//                //                }
//                //            }
//            }
        }
        if(params.draw.points) {
            glPointSize(5);
            gl::begin(GL_POINTS);
            for(int i=0;i<points.size();i++){
                gl::vertex(points[i].x, points[i].y);
            }
            gl::end();
        }
        
        gl::popMatrices();
        
    }
    
    //--------------------------------------------------------------
    void Frame::clear() {
//        origPolys.clear();
//        processedPolys.clear();
    }
    
    //--------------------------------------------------------------
    int Frame::size() {
        return 10;//origPolys.size();
    }
    
    //--------------------------------------------------------------

    void Frame::setShape2d(const Shape2d& shape){
        setShape2d(shape,mCurrentColor);
    }
    
    void Frame::setShape2d(const Shape2d& shape, ColorA clr){
        mColorsContours.clear();
        mColorsSegments.clear();
        origShape = shape;
        for(int i=0;i<shape.getNumContours();i++){
            mColorsContours.push_back(clr);
        }
        addColoursToShape(shape, clr);
        updateFinalPoints();
    }

    //--------------------------------------------------------------
    
    void Frame::addShape2d(const Shape2d& shape){
        addShape2d(shape, mCurrentColor);
    }
    
    void Frame::addShape2d(const Shape2d& shape, ColorA clr){
        origShape.append(shape);
        for(int i=0;i<shape.getNumContours();i++){
            mColorsContours.push_back(clr);
        }
        addColoursToShape(shape, clr);
        updateFinalPoints();
    }

    //--------------------------------------------------------------

    void Frame::addPath2d(const Path2d& path){
        addPath2d(path, mCurrentColor);
    }
    
    void Frame::addPath2d(const Path2d& path, ColorA clr){
        origShape.appendContour(path);
        mColorsContours.push_back(clr);
        addColoursToPath(path, clr);
        updateFinalPoints();
    }

    //--------------------------------------------------------------

    const Shape2d& Frame::getShape2d(){
        return origShape;
    }
    
    //--------------------------------------------------------------
    const vector<Point>& Frame::getPoints() const {
        return points;
    }
    
    //--------------------------------------------------------------
    void Frame::addColoursToShape(const Shape2d& shape, ColorA clr){
        for(int i=0;i<shape.getNumContours();i++){
            addColoursToPath(shape.getContour(i),clr);
        }        
    }
    
    //--------------------------------------------------------------
    void Frame::addColoursToPath(const Path2d& path, ColorA clr){
        for(int i=0;i<path.getNumSegments();i++){
            mColorsSegments.push_back(clr);
        }
    }
    
    
    //--------------------------------------------------------------
    void Frame::drawCalibration() {
        
        Shape2d calib;
        
        Path2d square;
        square.moveTo(0.25, 0.25);
        square.lineTo(0.75, 0.25);
        square.lineTo(0.75, 0.75);
        square.lineTo(0.25, 0.75);
        square.close();
        
        
        Path2d lineVert;
        lineVert.moveTo(0.5,0.25);
        lineVert.moveTo(0.5,0.75);
        
        Path2d lineHori;
        lineHori.moveTo(0.25,0.5);
        lineHori.moveTo(0.75,0.5);
        
        Path2d circle;
        circle.moveTo(0,-0.2);
        circle.arcTo(0, 0.2, 0, 0, M_PI);
        
        calib.appendContour(square);
        calib.appendContour(lineHori);
        calib.appendContour(lineVert);
        calib.appendContour(circle);
        
        setShape2d(calib);
        
    }
    
    //--------------------------------------------------------------
    Point Frame::transformPoint(Vec2f p, ColorA clr) const {
        
        Point pIlda;
        
        // flip
        if(params.output.transform.doFlipX) p.x = 1 - p.x;
        if(params.output.transform.doFlipY) p.y = 1 - p.y;
        
        // scale
        if(params.output.transform.scale.lengthSquared() > 0) {
            p -= Vec2f(0.5, 0.5);
            p *= params.output.transform.scale;
            p += Vec2f(0.5, 0.5);
        }
        
        // offset
        p += params.output.transform.offset;
        
        
        
        
        // cap or wrap
        if(p.x < 0) {
            p.x = params.output.doCapX ? 0 : 1 + p.x - ceil(p.x);
        } else if(p.x > 1) {
            p.x = params.output.doCapX ? 1 : p.x - floor(p.x);
        }
        
        if(p.y < 0) {
            p.y = params.output.doCapY ? 0 : 1 + p.y - ceil(p.y);
        } else if(p.y > 1) {
            p.y = params.output.doCapY ? 1 : p.y - floor(p.y);
        }
        
        pIlda.set(p, clr);        
        return pIlda;
    }
    
    //--------------------------------------------------------------
    void Frame::updateFinalPoints() {
        
        calculateIldaPoints();
        
//        points.clear();
//        for(int i=0; i<processedPolys.size(); i++) {
//            ofPolyline &poly = processedPolys[i];
//            ofFloatColor &pcolor = processedPolys[i].color;
//            
//            if(poly.size() > 0) {
//                
//                ofPoint startPoint = transformPoint(poly.getVertices().front());
//                ofPoint endPoint = transformPoint(poly.getVertices().back());
//                
//                // blanking at start
//                for(int n=0; n<params.output.blankCount; n++) {
//                    points.push_back( Point(startPoint, ofFloatColor(0, 0, 0, 0)));
//                }
//                
//                // repeat at start
//                for(int n=0; n<params.output.endCount; n++) {
//                    points.push_back( Point(startPoint, pcolor) );
//                }
//                
//                // add points
//                for(int j=0; j<poly.size(); j++) {
//                    points.push_back( Point(transformPoint(poly[j]), pcolor) );
//                }
//                
//                // repeat at end
//                for(int n=0; n<params.output.endCount; n++) {
//                    points.push_back( Point(endPoint, pcolor) );
//                }
//                
//                // blanking at end
//                for(int n=0; n<params.output.blankCount; n++) {
//                    points.push_back( Point(endPoint, ofFloatColor(0, 0, 0, 0) ));
//                }
//                
//            }
//        }
    }
    
    
    
    //--------------------------------------------------------------
    
    void Frame::calculateIldaPoints(){
        
        vector<float> segmentLengths;
        
        float totalLength = 0;
        int blankCount = params.output.blankCount;
        int endCount = params.output.endCount;
        
        int totalAmountStripped = params.output.targetPointCount;
        
        points.clear();
        
        for(int i=0; i<origShape.getNumContours(); i++) {
            float len = 0;
            size_t pathType;
            Path2d path = origShape.getContour(i);
            totalAmountStripped -= blankCount;
            totalAmountStripped -= endCount;
            for(int j=0;j<path.getNumSegments();j++){
                pathType = path.getSegmentType(j);
                len = 0;
                if(pathType == Path2d::LINETO || pathType == Path2d::CLOSE ){
                    len = (path.getSegmentPosition(j, 1) - path.getSegmentPosition(j, 0)).length();
                }else if(pathType == Path2d::MOVETO){
                    // Don't do anything
                }else{
                    len = getSegmentLength(path,j);
                }
                segmentLengths.push_back(len);
                totalLength += len;
            }
            totalAmountStripped -= blankCount;
            //        if(pathType!=Path2d::CLOSE)
            totalAmountStripped -= endCount;
            
            totalAmountStripped -= 1; // TODO only if not closed?
        }
        
        
        float step = totalLength/totalAmountStripped;
        
//        console() << std::endl<<std::endl<<"LEN TOT : " << totalLength << std::endl;
//        console() << " step : "<< step<< std::endl;
//        console() << " totalAmountStripped : "<< totalAmountStripped<< std::endl;
        
        int segCounter = 0;
        float steps;
        float percentSeg;
        Vec2f pos;
        Point pIlda;
        ColorA clr;
        for(int i=0; i<origShape.getNumContours(); i++) {
            float len = 0;
            Path2d path = origShape.getContour(i);
            pos = path.getPosition(0);
            pIlda = transformPoint(pos);
//            clr = mColorsSegments[i];
            clr = mColorsSegments[segCounter];
            
            for(int k=0;k<blankCount;k++){ points.push_back(pIlda); }
            pIlda = transformPoint(pos,clr);
            for(int k=0;k<endCount;k++){ points.push_back(pIlda); }
                        
            for(int j=0;j<path.getNumSegments();j++){
                clr = mColorsSegments[segCounter];
                len = segmentLengths[segCounter++];
                steps = round(len / step);
                for(int k=0;k<steps;k++){
                    percentSeg = k/steps;
                    pos = path.getSegmentPosition(j, k/steps);
                    pIlda = transformPoint(pos,clr);
                    points.push_back(pIlda);
                }
                pos = path.getSegmentPosition(j, 1);
            }
            pIlda = transformPoint(pos,clr);
            points.push_back(pIlda);
            
            for(int k=0;k<endCount;k++){ points.push_back(pIlda); }
            pIlda = transformPoint(pos);
            for(int k=0;k<blankCount;k++){ points.push_back(pIlda); }
                        
        }
        
        console() << " segCounter : " << segCounter << "        mColorsSegments: " << mColorsSegments.size() << std::endl;
//
//        for(int i=0;i<points.size();i++){
////            console() << i << " -> " << points[i].x << " _ "  << points[i].x << std::endl;
//        }
        
        stats.pointCountProcessed = points.size();
        
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    ////////////////// little helper
    
    float Frame::getSegmentLength(const Path2d& path, int segment, int detail ){
        float len = 0;
        Vec2f pos1, pos2;
        float step = 1.0/detail;
        pos1 = path.getPosition(0);
        for(float percent=step;percent<=1;percent+=step){
            pos2 = path.getPosition(percent);
            len += (pos2 - pos1).length();
            pos1 = pos2;
        }
        return len;
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    
    float Frame::getPathLength(const Path2d& path, int detail ){
        float len = 0;
        float totalLength = 0;
        for(int j=0;j<path.getNumSegments();j++){
            size_t pathType = path.getSegmentType(j);
            len = 0;
            if(pathType == Path2d::LINETO || pathType == Path2d::CLOSE ){
                len = (path.getSegmentPosition(j, 1) - path.getSegmentPosition(j, 0)).length();
            }else if(pathType == Path2d::MOVETO){
                // Don't do anything
            }else{
                len = getSegmentLength(path,j,detail);
            }
            totalLength += len;
        }
        return totalLength;
    }


    //////////////////////////////////////////////////////////////////////////////////////////
    

} // namespace ciilda