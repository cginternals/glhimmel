#include </data/shader/bruneton/include/uniforms.glsl>

#ifndef LIMIT
#define LIMIT

float limit(float r, float mu) {
    float RL = u_radiusUpToEndOfAtmosphere + 1.0;
    float dout = -r * mu + sqrt(r * r * (mu * mu - 1.0) + RL * RL);
    float delta2 = r * r * (mu * mu - 1.0) + u_apparentAngularRadius * u_apparentAngularRadius;
    if (delta2 >= 0.0) {
        float din = -r * mu - sqrt(delta2);
        if (din >= 0.0) {
            dout = min(dout, din);
        }
    }
    return dout;
}

#endif // LIMIT
