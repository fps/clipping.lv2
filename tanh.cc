#include <cmath>
#include <lv2.h>

#include "common.cc"
#include "oversampling.h"
#include "cascaded_biquad/cascaded_biquad.h"

struct clipping_tanh {
    float *ports[6];
    float *in_filter_state;
    float *out_filter_state;
};

static LV2_Handle instantiate(const LV2_Descriptor *descriptor, double sample_rate, const char *bundle_path, const LV2_Feature *const *features)
{
    clipping_tanh *instance = new clipping_tanh;
    instance->in_filter_state = cascaded_biquad::state<6, float>();
    instance->out_filter_state = cascaded_biquad::state<6, float>();
    return (LV2_Handle)(instance);
}

static void cleanup(LV2_Handle instance)
{
    delete ((clipping_tanh*)instance);
}

static void connect_port(LV2_Handle instance, uint32_t port, void *data_location)
{
    ((clipping_tanh*)instance)->ports[port] = (float*)data_location;
}

static void run(LV2_Handle instance, uint32_t sample_count)
{
    clipping_tanh *tinstance = (clipping_tanh*)(instance);

    const float pregain = db_to_gain(tinstance->ports[2][0]);
    const float bias = tinstance->ports[3][0];
    const float postgain = db_to_gain(tinstance->ports[4][0]);
    const float wet = tinstance->ports[5][0];
    const float dry = 1.0 - wet;

    float in_buffer[4] = { 0, 0, 0, 0};
    float out_buffer[4] = { 0, 0, 0, 0 };
    
    for(uint32_t sample_index = 0; sample_index < sample_count; ++sample_index)
    {
        const float in = tinstance->ports[0][sample_index];
        const float in2 = bias + pregain * in;
        
        float out = 0;
        for (int index = 0; index < 4; ++index) 
        {
          const float intermediate = index == 0 ? in2 : 0;

          const float in_filtered = cascaded_biquad::process<6, float, float, float>(clipping_oversampling_filter_coefficients, tinstance->in_filter_state, clipping_oversampling_filter_gain, intermediate);
          const float clipped = tanhf(in_filtered);
          const float out_filtered = cascaded_biquad::process<6, float, float, float>(clipping_oversampling_filter_coefficients, tinstance->out_filter_state, clipping_oversampling_filter_gain, clipped);

          if (index == 0) out = out_filtered;
        }

        tinstance->ports[1][sample_index] = dry * in +
            wet * postgain * out;
    }
}

static const LV2_Descriptor descriptor = {
    "http://fps.io/plugins/clipping.tanh",
    instantiate,
    connect_port,
    nullptr, // activate
    run,
    nullptr, // deactivate
    cleanup,
    nullptr // extension_data
};

LV2_SYMBOL_EXPORT const LV2_Descriptor* lv2_descriptor (uint32_t index)
{
    if (0 == index) return &descriptor;
    else return nullptr;
}
