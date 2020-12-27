#include <cmath>
#include <lv2.h>

inline float db_to_gain( float x) 
{
    return pow(10, x/20.0);
}

struct clipping_hard {
    float *ports[5];
};

LV2_Handle instantiate(const LV2_Descriptor *descriptor, double sample_rate, const char *bundle_path, const LV2_Feature *const *features)
{
    return (LV2_Handle)(new clipping_hard);
}

void cleanup(LV2_Handle instance)
{
    delete ((clipping_hard*)instance);
}

void connect_port(LV2_Handle instance, uint32_t port, void *data_location)
{
    ((clipping_hard*)instance)->ports[port] = (float*)data_location;
}

void run(LV2_Handle instance, uint32_t sample_count)
{
    clipping_hard *tinstance = (clipping_hard*)(instance);

    const float pregain = db_to_gain(tinstance->ports[2][0]);
    const float threshold = tinstance->ports[3][0];
    const float bias = tinstance->ports[4][0];
    const float postgain = db_to_gain(tinstance->ports[5][0]);
    
    for(uint32_t sample_index = 0; sample_index < sample_count; ++sample_index)
    {
        const float in = (pregain * tinstance->ports[0][sample_index]) + bias;
        tinstance->ports[1][sample_index] = 
            postgain * (fabs(in) > threshold ? threshold : in);
    }
}

LV2_Descriptor descriptor = {
    "http://fps.io/plugins/clipping.hard",
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
