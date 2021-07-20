//-----------------------------------------------
//
//	This file is part of the Siv3D Engine.
//
//	Copyright (c) 2008-2021 Ryo Suzuki
//	Copyright (c) 2016-2021 OpenSiv3D Project
//
//	Licensed under the MIT License.
//
//-----------------------------------------------

# version 410

//
//	Textures
//
uniform sampler2D Texture0;

//
//	PSInput
//
layout(location = 0) in vec4 Color;
layout(location = 1) in vec2 UV;

//
//	PSOutput
//
layout(location = 0) out vec4 FragColor;

//
//	Functions
//
vec3 Gamma(vec3 color, float g)
{
	return pow(color, vec3(g));
}

void main()
{
	vec3 texColor = texture(Texture0, UV).rgb;

	FragColor = vec4(Gamma(abs(texColor), (1.0f / 2.2f)), 1.0f);
}
