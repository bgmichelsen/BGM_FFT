/*
  ==============================================================================

    BGM_FFT.cpp
    Created: 5/30/25 5:29:50
    Author:  brand

  ==============================================================================
*/

#include "BGM_FFT.h"

#include <cmath>
#include <math.h>
#include <algorithm>

BGM::FFT::FFT()
{

}

// Based on BASIC code implementation in the Scientist's Guide to DSP by Steven W. Smith
// https://www.dspguide.com/ch12/3.htm
void BGM::FFT::forwardTransform(float *const data, size_t size)
{
    constexpr double PI = 3.14159265;
    size_t N = (size);
    int M = (std::log(N) / std::log(2));

    float *re = new float[N];
    float *im = new float[N];
    std::memcpy(re, data, (N * sizeof(float)));
    std::memset(im, 0.0f, (N * sizeof(float)));

    bitReversal(re, im, N);

    // Loop each FFT stage
    for (int l = 1; l < M; l++)
    {
        int le = std::pow(2, l);
        int le2 = (le / 2);
        float ur = 1.0f;
        float ui = 0.0f;
        float sr = std::cos(PI / le2);
        float si = -1.0f * std::sin(PI / le2);

        // Loop for each sub DFT
        for (int j = 1; j < le2; j++)
        {
            // Loop for each butterfly calculation
            for (int i = (j - 1); i < (N - 1); i += le)
            {
                int ip = i + le2;

                // Butterfly calculation
                float tr = re[ip] * ur - im[ip] * ui;
                float ti = re[ip] * ui + im[ip] * ur;
                re[ip] = re[i] - tr;
                im[ip] = im[i] - ti;
                re[i] = re[i] + tr;
                im[i] = im[i] + ti;
            }

            float tr = ur;
            ur = tr * sr - ui * si;
            ui = tr * si + ui * sr;
        }
    }

    // Set the new data
    for (int i = 0; i < (N / 2); i++)
    {
        data[i * 2] = re[i];
        data[i * 2 + 1] = im[i];
    }

    delete[] re;
    delete[] im;
}

// Based on BASIC code implementation in the Scientist's Guide to DSP by Steven W. Smith
// https://www.dspguide.com/ch12/3.htm
void BGM::FFT::inverseTransform(float* const data, size_t size)
{
    // TODO: Implement inverse FFT
}

// data = in and out
void BGM::FFT::bitReversal(float *const re, float *const im, size_t size)
{
    size_t N = size;//(size / 2);
    int j = (N / 2);
    for (int i = 1; i < (N - 2); i++)
    {
        if (i < j)
        {
            float tr = re[j];       // Real part
            float ti = im[j];       // Imaginary part
            re[j] = re[i];
            im[j] = im[i];
            re[i] = tr;
            im[i] = ti;
        }
        int k = (N / 2);
        while (k <= j)
        {
            j = j - k;
            k = k / 2;
        }
        j = j + k;
    }
}

void BGM::FFT::spaceOut(float* const data, size_t size)
{
    size_t N = (size / 2);
    float* temp_arr = new float[N];
    std::copy_n(data, N, temp_arr);
    std::memset(data, 0.0f, size);
    for (int i = 0; i < N; i++)
    {
        data[i * 2] = temp_arr[i];
    }
    delete[] temp_arr;
}