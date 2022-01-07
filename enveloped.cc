#include <cmath>
#include <lv2.h>

#include "common.cc"

struct clipping_enveloped {
    float *ports[6];
    float buffer[128];
    int buffer_position;
    double samplerate;

    clipping_enveloped(double samplerate) 
        : 
            buffer_position(0), 
            samplerate(samplerate) 
        { 
            for (int index = 0; index < 128; ++index)
            {
                buffer[index] = 0;
            }
        }
};

static LV2_Handle instantiate(const LV2_Descriptor *descriptor, double sample_rate, const char *bundle_path, const LV2_Feature *const *features)
{
    return (LV2_Handle)(new clipping_enveloped(sample_rate));
}

static void cleanup(LV2_Handle instance)
{
    delete ((clipping_enveloped*)instance);
}

static void connect_port(LV2_Handle instance, uint32_t port, void *data_location)
{
    ((clipping_enveloped*)instance)->ports[port] = (float*)data_location;
}

static void run(LV2_Handle instance, uint32_t sample_count)
{
    clipping_enveloped *tinstance = (clipping_enveloped*)(instance);

    const float envgain = db_to_gain(tinstance->ports[2][0]);
    const int window_size = (int)(tinstance->ports[3][0]);
    const float postgain = db_to_gain(tinstance->ports[4][0]);
    const float wet = tinstance->ports[5][0];
    const float dry = 1.0 - wet;

    for(uint32_t sample_index = 0; sample_index < sample_count; ++sample_index)
    {
        const float in = tinstance->ports[0][sample_index];
        tinstance->buffer[tinstance->buffer_position] = in;

        float env = 0;
        for (int index = 0; index < window_size; ++index) 
            { env += fabs(tinstance->buffer[index] / (float)window_size); }

        const float in2 = in > 0 ? 1 : -1;
        
        tinstance->ports[1][sample_index] = dry * in +
            wet * postgain * tanhf(envgain * env) * in2;

        ++(tinstance->buffer_position);
        tinstance->buffer_position %= window_size;
    }
}

static const LV2_Descriptor descriptor = {
    "http://fps.io/plugins/clipping.enveloped",
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
