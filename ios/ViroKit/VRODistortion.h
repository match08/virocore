//
//  VRODistortion.h
//  ViroRenderer
//
//  Created by Raj Advani on 10/23/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#ifndef VRODistortion_h
#define VRODistortion_h

#include "VRODefines.h"
#if VRO_METAL

#include <stdio.h>
#include <cmath>

/*
 Parameterizes the barrel distortion to use in the distortion renderer.
 */
class VRODistortion {
    
public:
    
    VRODistortion() {
        _coefficients[0] = 0.441f;
        _coefficients[1] = 0.156f;
    }
    
    void setCoefficients(const float *coefficients) {
        for (int i = 0; i < s_numberOfCoefficients; i++) {
            _coefficients[i] = coefficients[i];
        }
    }
    
    const float *getCoefficients() const {
        return _coefficients;
    };
    
    float getDistortionFactor(float radius) const {
        float result = 1.0f;
        float rFactor = 1.0f;
        float squaredRadius = radius * radius;
        
        for (int i = 0; i < s_numberOfCoefficients; i++) {
            rFactor *= squaredRadius;
            result += _coefficients[i] * rFactor;
        }
        return result;
    }
    
    float distort(float radius) const {
        return radius * getDistortionFactor(radius);
    }
    
    float distortInverse(float radius) const {
        float r0 = radius / 0.9f;
        float r = radius * 0.9f;
        float dr0 = radius - distort(r0);
        
        while (std::abs((float) (r - r0)) > 0.0001f) {
            float dr = radius - distort(r);
            float r2 = r - dr * ((r - r0) / (dr - dr0));
            r0 = r;
            r = r2;
            dr0 = dr;
        }
        
        return r;
    }
    
    bool equals(const VRODistortion *other) const {
        if (other == nullptr) {
            return false;
        }
        else if (other == this) {
            return true;
        }
        
        for (int i = 0; i < s_numberOfCoefficients; i++) {
            if (_coefficients[i] != other->_coefficients[i]) {
                return false;
            }
        }
        
        return true;
    }
    
private:
    
    constexpr static int s_numberOfCoefficients = 2;
    float _coefficients[s_numberOfCoefficients];
    
};

#endif
#endif /* VRODistortion_h */