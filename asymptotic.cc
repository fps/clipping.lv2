#include <cmath>
#include <lv2.h>

#include "common.cc"

struct clipping_asymptotic {
    float *ports[7];
};

static LV2_Handle instantiate(const LV2_Descriptor *descriptor, double sample_rate, const char *bundle_path, const LV2_Feature *const *features)
{
    return (LV2_Handle)(new clipping_asymptotic);
}

static void cleanup(LV2_Handle instance)
{
    delete ((clipping_asymptotic*)instance);
}

static void connect_port(LV2_Handle instance, uint32_t port, void *data_location)
{
    ((clipping_asymptotic*)instance)->ports[port] = (float*)data_location;
}

static void run(LV2_Handle instance, uint32_t sample_count)
{
    clipping_asymptotic *tinstance = (clipping_asymptotic*)(instance);

    const float pregain = db_to_gain(tinstance->ports[2][0]);
    const float normalizing_constant = tinstance->ports[3][0];
    const float bias = tinstance->ports[4][0];
    const float postgain = db_to_gain(tinstance->ports[5][0]);
    const float wet = tinstance->ports[6][0];
    const float dry = 1.0 - wet;
    
    for(uint32_t sample_index = 0; sample_index < sample_count; ++sample_index)
    {
        const float in = tinstance->ports[0][sample_index];
        const float in2 = bias + pregain * in;
        
        tinstance->ports[1][sample_index] = dry * in + 
            wet * postgain * in2 / (fabs(in2) + normalizing_constant);
    }
}

static const LV2_Descriptor descriptor = {
    "http://fps.io/plugins/clipping.asymptotic",
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
