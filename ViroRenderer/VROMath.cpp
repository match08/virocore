//
//  VROMath.cpp
//  ViroRenderer
//
//  Created by Raj Advani on 10/15/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#include "VROMath.h"
#include <algorithm>

matrix_float4x4 matrix_from_scale(float sx, float sy, float sz) {
    matrix_float4x4 m = matrix_identity_float4x4;
    m.columns[0][0] = sx;
    m.columns[1][1] = sy;
    m.columns[2][2] = sz;
    
    return m;
}

matrix_float4x4 matrix_from_translation(float x, float y, float z) {
    matrix_float4x4 m = matrix_identity_float4x4;
    m.columns[3] = (vector_float4) { x, y, z, 1.0 };
    
    return m;
}

matrix_float4x4 matrix_from_rotation(float radians, float x, float y, float z) {
    vector_float3 v = vector_normalize(((vector_float3){x, y, z}));
    float cos = cosf(radians);
    float cosp = 1.0f - cos;
    float sin = sinf(radians);
    
    matrix_float4x4 m = {
        .columns[0] = {
            cos + cosp * v.x * v.x,
            cosp * v.x * v.y + v.z * sin,
            cosp * v.x * v.z - v.y * sin,
            0.0f,
        },
        
        .columns[1] = {
            cosp * v.x * v.y - v.z * sin,
            cos + cosp * v.y * v.y,
            cosp * v.y * v.z + v.x * sin,
            0.0f,
        },
        
        .columns[2] = {
            cosp * v.x * v.z + v.y * sin,
            cosp * v.y * v.z - v.x * sin,
            cos + cosp * v.z * v.z,
            0.0f,
        },
        
        .columns[3] = { 0.0f, 0.0f, 0.0f, 1.0f
        }
    };
    return m;
}

matrix_float4x4 matrix_from_perspective_fov_aspectLH(const float fovY, const float aspect, const float nearZ, const float farZ) {
    float yscale = 1.0f / tanf(fovY * 0.5f); // 1 / tan == cot
    float xscale = yscale / aspect;
    float q = farZ / (farZ - nearZ);
    
    matrix_float4x4 m = {
        .columns[0] = { xscale, 0.0f, 0.0f, 0.0f },
        .columns[1] = { 0.0f, yscale, 0.0f, 0.0f },
        .columns[2] = { 0.0f, 0.0f, q, 1.0f },
        .columns[3] = { 0.0f, 0.0f, q * -nearZ, 0.0f }
    };
    
    return m;
}

matrix_float4x4 matrix_for_frustum(const float left, const float right,
                                   const float bottom, const float top,
                                   const float znear, const float zfar) {
    const float x_2n = znear + znear;
    const float x_2nf = 2 * znear * zfar;
    
    const float p_fn = zfar + znear;
    const float m_nf = znear - zfar;
    
    const float p_rl = right + left;
    const float m_rl = right - left;
    const float p_tb = top + bottom;
    const float m_tb = top - bottom;
    
    matrix_float4x4 m = {
        .columns[0] = { x_2n / m_rl, 0, 0, 0 },
        .columns[1] = { 0, x_2n / m_tb, 0, 0 },
        .columns[2] = { p_rl / m_rl, p_tb / m_tb, p_fn / m_nf, -1 },
        .columns[3] = { 0, 0, x_2nf / m_nf, 0}
    };

    return m;
}

matrix_float4x4 matrix_float4x4_from_GL(GLKMatrix4 glm) {
    matrix_float4x4 m = {
        .columns[0] = { glm.m[0],  glm.m[1],  glm.m[2],  glm.m[3]  },
        .columns[1] = { glm.m[4],  glm.m[5],  glm.m[6],  glm.m[7]  },
        .columns[2] = { glm.m[8],  glm.m[9],  glm.m[10], glm.m[11] },
        .columns[3] = { glm.m[12], glm.m[13], glm.m[14], glm.m[15] }
    };
    
    return m;
}

double degrees_to_radians(double degrees) {
    return degrees * M_PI / 180.0;
}

double radians_to_degrees(double radians) {
    return radians * 180.0 / M_PI;
}

float clamp(float val, float min, float max) {
    return std::max(min, std::min(max, val));
}
