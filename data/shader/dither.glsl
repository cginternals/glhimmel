float pseudo_rand(
    const vec2 i
,   const int seed)
{
    int i1 = int(i.x + i.y * 1733);
    i1 = (i1 << 7) ^ i1 + seed;  // seed

    int i2 = int(i.y + i.x * 1103);
    i2 = (i2 << 7) ^ i2 + seed;  // seed

    i1 ^= i2;
    return 1.0 - float((i1 * (i1 * i1 * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0;
}

vec4 dither(
    float multiplier
,   int seed)
{
    float r = pseudo_rand(gl_FragCoord.xy, seed);
    uvec4 v = uint(r * 3571) * uvec4(67, 89, 23, 71);

    // A ditheringMultiplier of 1 will add frame to frame coherent noise for each pixel of about +-1.
    // The average brightness of the rendering will roughly remain unchanged.
    return (vec4(v % uvec4(853)) - 241 - multiplier * 1.41) * 0.00001 * multiplier;
}