/*
  ==============================================================================

    BGM_FFT.h
    Created: 5/30/25 5:29:50
    Author:  brand

  ==============================================================================
*/

#pragma once

namespace BGM
{
    class FFT
    {
    public:
        FFT();
        void forwardTransform(float *const data, size_t size);
        void inverseTransform(float *const data, size_t size);
    private:
        void bitReversal(float *const re, float *const im, size_t size);
        void spaceOut(float *const data, size_t size);
    };
}