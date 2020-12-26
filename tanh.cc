#include <cmath>
#include <lv2.h>

struct clipping_tanh {
    float *ports[5];
};

LV2_Handle instantiate(const LV2_Descriptor *descriptor, double sample_rate, const char *bundle_path, const LV2_Feature *const *features)
{
    return (LV2_Handle)(new clipping_tanh);
}

void cleanup(LV2_Handle instance)
{
    delete ((clipping_tanh*)instance);
}

void connect_port(LV2_Handle instance, uint32_t port, void *data_location)
{
    ((clipping_tanh*)instance)->ports[port] = (float*)data_location;
}

void run(LV2_Handle instance, uint32_t sample_count)
{
    clipping_tanh *tinstance = (clipping_tanh*)(instance);
    for(uint32_t sample_index = 0; sample_index < sample_count; ++sample_index)
    {
        tinstance->ports[3][sample_index] = 
            tinstance->ports[4][0] * 
            tanh((tinstance->ports[0][0] * tinstance->ports[2][sample_index]) + tinstance->ports[1][0]);
    }
}

LV2_Descriptor descriptor = {
    "https://fps.io/plugins/clipping.tanh",
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
