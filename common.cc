/*
designed in matlab by: 

oversample = 4;

pass = 0.75;
stop = 1;

attenuation = 18;

ai_lp = designfilt('lowpassiir', 'PassbandFrequency', pass/oversample, 'StopbandFrequency', stop/oversample, 'StopbandAttenuation', attenuation);

ai_lp.Coefficients
*/

const float aa_lp_coeffs[5][6] = {
    { 0.0887,    0.1774,    0.0887,    1.0000,   -1.4582,    0.8129 },
    { 0.0754,    0.1509,    0.0754,    1.0000,   -1.2402,    0.5419 },
    { 0.0672,    0.1345,    0.0672,    1.0000,   -1.1055,    0.3744 },
    { 0.0628,    0.1256,    0.0628,    1.0000,   -1.0323,    0.2834 },
    { 0.2477,    0.2477,         0,    1.0000,   -0.5045,         0 }
};

static inline float db_to_gain( float x)
{
    return expf(0.05f * x * logf(10.0));
}

