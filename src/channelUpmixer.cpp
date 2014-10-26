#include "template/include/client.h"

#include "channelUpmixer.h"

#include <string>
using namespace std;

//! Array with channel layout as a string.
static char *channellayout_strings[] =
{
	"channellayout_mono",				// FC	(supported from ActiveAE)
	"channellayout_stereo",				// FL, FR	(supported from ActiveAE)
	"channellayout_2_1",				// FL, FR, LFE	(supported from ActiveAE)
	"channellayout_3_0",				// FL, FR, FC	(supported from ActiveAE)
	"channellayout_3_0_back",			// FL, FR, BC
	"channellayout_4_0",				// FL, FR, FC, BC
	"channellayout_quad",				// FL, FR, BL, BR
	"channellayout_quad_side",			// FL, FR, SL, SR	(supported from ActiveAE)
	"channellayout_3_1",				// FL, FR, FC, LFE	(supported from ActiveAE)
	"channellayout_5_0",				// FL, FR, FC, BL, BR
	"channellayout_5_0_side",			// FL, FR, FC, SL, SR	(supported from ActiveAE)
	"channellayout_4_1",				// FL, FR, FC, LFE, BC
	"channellayout_4_1_side",			// FL, FR, LFE, SL, SR	(supported from ActiveAE)
	"channellayout_5_1",				// FL, FR, FC, LFE, BL, BR
	"channellayout_5_1_side",			// FL, FR, FC, LFE, SL, SR	(supported from ActiveAE)
	"channellayout_6_0",				// FL, FR, FC, BC, SL, SR
	//"channellayout_6_0_front",		// FL, FR, FLC, FRC, SL, SR --> not supported from ADSP Addons, it's only here for future implementations
	"channellayout_hexagonal",			// FL, FR, FC, BL, BR, BC
	"channellayout_6_1",				// FL, FR, FC, LFE, BC, SL, SR
	"channellayout_6_1_back",			// FL, FR, FC, LFE, BL, BR, BC
	//"channellayout_6_1_front",		// FL, FR, LFE, FLC, FRC, SL, SR --> not supported from ADSP Addons, it's only here for future implementations
	"channellayout_7_0",				// FL, FR, FC, BL, BR, SL, SR	(supported from ActiveAE)
	//"channellayout_7_0_front",		// FL, FR, FC, FLC, FRC, SL, SR
	"channellayout_7_1",				// FL, FR, FC, LFE, BL, BR, SL, SR	(supported from ActiveAE)
	//"channellayout_7_1_wide",			// FL, FR, FC, LFE, BL, BR, FLC, FRC
	//"channellayout_7_1_wide_side",	// FL, FR, FC, LFE, FLC, FRC, SL, SR
	"channellayout_octagonal",			// FL, FR, FC, BL, BR, BC, SL, SR
	//"channellayout_downmix",			// DL, DR --> not supported from ADSP Addons, it's only here for future implementations
	"channellayout_unknown",
	NULL
};

ADSP_CHANNEL_LAYOUT getChannelLayout(unsigned int ChannelFlags)
{
	switch (ChannelFlags)
	{
	case AE_DSP_PRSNT_CH_FC:
		return channellayout_mono;
		break;

	case AE_DSP_PRSNT_CH_FL | AE_DSP_PRSNT_CH_FR:
		return channellayout_stereo;
		break;

	case AE_DSP_PRSNT_CH_FL | AE_DSP_PRSNT_CH_FR | AE_DSP_PRSNT_CH_LFE:
		return channellayout_2_1;
		break;

	case AE_DSP_PRSNT_CH_FL | AE_DSP_PRSNT_CH_FR | AE_DSP_PRSNT_CH_FC:
		return channellayout_3_0;
		break;

	case AE_DSP_PRSNT_CH_FL | AE_DSP_PRSNT_CH_FR | AE_DSP_PRSNT_CH_BC:
		return channellayout_3_0_back;
		break;

	case AE_DSP_PRSNT_CH_FL | AE_DSP_PRSNT_CH_FR | AE_DSP_PRSNT_CH_FC | AE_DSP_PRSNT_CH_BC:
		return channellayout_4_0;
		break;

	case AE_DSP_PRSNT_CH_FL | AE_DSP_PRSNT_CH_FR | AE_DSP_PRSNT_CH_BL | AE_DSP_PRSNT_CH_BR:
		return channellayout_quad;
		break;

	case AE_DSP_PRSNT_CH_FL | AE_DSP_PRSNT_CH_FR | AE_DSP_PRSNT_CH_SL | AE_DSP_PRSNT_CH_SR:
		return channellayout_quad_side;
		break;

	case AE_DSP_PRSNT_CH_FL | AE_DSP_PRSNT_CH_FR | AE_DSP_PRSNT_CH_FC | AE_DSP_PRSNT_CH_LFE:
		return channellayout_3_1;
		break;

	case AE_DSP_PRSNT_CH_FL | AE_DSP_PRSNT_CH_FR | AE_DSP_PRSNT_CH_FC | AE_DSP_PRSNT_CH_BL | AE_DSP_PRSNT_CH_BR:
		return channellayout_5_0;
		break;

	case AE_DSP_PRSNT_CH_FL | AE_DSP_PRSNT_CH_FR | AE_DSP_PRSNT_CH_FC | AE_DSP_PRSNT_CH_SL | AE_DSP_PRSNT_CH_SR:
		return channellayout_5_0_side;
		break;

	case AE_DSP_PRSNT_CH_FL | AE_DSP_PRSNT_CH_FR | AE_DSP_PRSNT_CH_FC | AE_DSP_PRSNT_CH_LFE | AE_DSP_PRSNT_CH_BC:
		return channellayout_4_1;
		break;

	case AE_DSP_PRSNT_CH_FL | AE_DSP_PRSNT_CH_FR | AE_DSP_PRSNT_CH_FC | AE_DSP_PRSNT_CH_LFE | AE_DSP_PRSNT_CH_BL | AE_DSP_PRSNT_CH_BR:
		return channellayout_5_1;
		break;

	case AE_DSP_PRSNT_CH_FL | AE_DSP_PRSNT_CH_FR | AE_DSP_PRSNT_CH_FC | AE_DSP_PRSNT_CH_LFE | AE_DSP_PRSNT_CH_SL | AE_DSP_PRSNT_CH_SR:
		return channellayout_5_1_side;
		break;

	case AE_DSP_PRSNT_CH_FL | AE_DSP_PRSNT_CH_FR | AE_DSP_PRSNT_CH_FC | AE_DSP_PRSNT_CH_BC | AE_DSP_PRSNT_CH_SL | AE_DSP_PRSNT_CH_SR:
		return channellayout_6_0;
		break;

		// not supported from ADSP Addons, it's only here for future implementations
		//case AE_DSP_PRSNT_CH_FL | AE_DSP_PRSNT_CH_FR | AE_DSP_PRSNT_CH_FLC | AE_DSP_PRSNT_CH_FRC | AE_DSP_PRSNT_CH_SL | AE_DSP_PRSNT_CH_SR:
		//	return channellayout_6_0_front;
		//break;

	case AE_DSP_PRSNT_CH_FL | AE_DSP_PRSNT_CH_FR | AE_DSP_PRSNT_CH_FC | AE_DSP_PRSNT_CH_BL | AE_DSP_PRSNT_CH_BR | AE_DSP_PRSNT_CH_BC:
		return channellayout_hexagonal;
		break;

	case AE_DSP_PRSNT_CH_FL | AE_DSP_PRSNT_CH_FR | AE_DSP_PRSNT_CH_FC | AE_DSP_PRSNT_CH_LFE | AE_DSP_PRSNT_CH_BC | AE_DSP_PRSNT_CH_SL | AE_DSP_PRSNT_CH_SR:
		return channellayout_6_1;
		break;

	case AE_DSP_PRSNT_CH_FL | AE_DSP_PRSNT_CH_FR | AE_DSP_PRSNT_CH_FC | AE_DSP_PRSNT_CH_LFE | AE_DSP_PRSNT_CH_BL | AE_DSP_PRSNT_CH_BR | AE_DSP_PRSNT_CH_BC:
		return channellayout_6_1_back;
		break;

		// not supported from ADSP Addons, it's only here for future implementations
		//case AE_DSP_PRSNT_CH_FL | AE_DSP_PRSNT_CH_FR | AE_DSP_PRSNT_CH_LFE | AE_DSP_PRSNT_CH_FLC | AE_DSP_PRSNT_CH_FRC | AE_DSP_PRSNT_CH_SL | AE_DSP_PRSNT_CH_SR:
		//	return channellayout_6_1_front;
		//break;

	case AE_DSP_PRSNT_CH_FL | AE_DSP_PRSNT_CH_FR | AE_DSP_PRSNT_CH_FC | AE_DSP_PRSNT_CH_BL | AE_DSP_PRSNT_CH_BR | AE_DSP_PRSNT_CH_SL | AE_DSP_PRSNT_CH_SR:
		return channellayout_7_0;
		break;

		//case AE_DSP_PRSNT_CH_FL | AE_DSP_PRSNT_CH_FR | AE_DSP_PRSNT_CH_FC | AE_DSP_PRSNT_CH_FLC | AE_DSP_PRSNT_CH_FRC | AE_DSP_PRSNT_CH_SL | AE_DSP_PRSNT_CH_SR:
		//	return channellayout_7_0_front;
		//break;

	case AE_DSP_PRSNT_CH_FL | AE_DSP_PRSNT_CH_FR | AE_DSP_PRSNT_CH_FC | AE_DSP_PRSNT_CH_LFE | AE_DSP_PRSNT_CH_BL | AE_DSP_PRSNT_CH_BR | AE_DSP_PRSNT_CH_SL | AE_DSP_PRSNT_CH_SR:
		return channellayout_7_1;
		break;

		//case AE_DSP_PRSNT_CH_FL | AE_DSP_PRSNT_CH_FR | AE_DSP_PRSNT_CH_FC | AE_DSP_PRSNT_CH_LFE | AE_DSP_PRSNT_CH_BL | AE_DSP_PRSNT_CH_BR | AE_DSP_PRSNT_CH_FLC | AE_DSP_PRSNT_CH_FRC:
		//	return channellayout_7_1_wide;
		//break;

		//case AE_DSP_PRSNT_CH_FL | AE_DSP_PRSNT_CH_FR | AE_DSP_PRSNT_CH_FC | AE_DSP_PRSNT_CH_LFE | AE_DSP_PRSNT_CH_FLC | AE_DSP_PRSNT_CH_FRC | AE_DSP_PRSNT_CH_SL | AE_DSP_PRSNT_CH_SR:
		//	return channellayout_7_1_wide_side;
		//break;

	case AE_DSP_PRSNT_CH_FL | AE_DSP_PRSNT_CH_FR | AE_DSP_PRSNT_CH_FC | AE_DSP_PRSNT_CH_BL | AE_DSP_PRSNT_CH_BR | AE_DSP_PRSNT_CH_BC | AE_DSP_PRSNT_CH_SL | AE_DSP_PRSNT_CH_SR:
		return channellayout_octagonal;
		break;

		// not supported from ADSP Addons, it's only here for future implementations
		//case AE_DSP_PRSNT_CH_DL | AE_DSP_PRSNT_CH_DR:
		//	return channellayout_downmix; 
		//break;

	default:
		return channellayout_unknown;
		break;
	}
}

std::string getChannelLayoutString(ADSP_CHANNEL_LAYOUT ChannelLayout)
{
	return string(channellayout_strings[ChannelLayout]);
}


void from_X_to_stereo(float **Array_in, float **Array_out, unsigned int Samples, ADSP_CHANNEL_LAYOUT UpmixFrom)
{
	switch (UpmixFrom)
	{
	case channellayout_mono:
		from_mono_to_stereo(Array_in, Array_out, Samples);
		break;
	}
}

void from_X_to_2_1(float **Array_in, float **Array_out, unsigned int Samples, ADSP_CHANNEL_LAYOUT UpmixFrom)
{
	switch (UpmixFrom)
	{
	case channellayout_mono:
		from_mono_to_2_1(Array_in, Array_out, Samples);
		break;

	case channellayout_stereo:
		from_mono_to_2_1(Array_in, Array_out, Samples);
		break;
	}
}

void from_X_to_3_0(float **Array_in, float **Array_out, unsigned int Samples, ADSP_CHANNEL_LAYOUT UpmixFrom)
{
	switch (UpmixFrom)
	{
	case channellayout_mono:
		from_mono_to_3_0(Array_in, Array_out, Samples);
		break;

	case channellayout_stereo:
		from_stereo_to_3_0(Array_in, Array_out, Samples);
		break;

	case channellayout_2_1:
		from_2_1_to_3_0(Array_in, Array_out, Samples);
		break;
	}
}

void from_X_to_quad_side(float **Array_in, float **Array_out, unsigned int Samples, ADSP_CHANNEL_LAYOUT UpmixFrom)
{
	switch (UpmixFrom)
	{
	case channellayout_mono:
		from_mono_to_quad_side(Array_in, Array_out, Samples);
		break;

	case channellayout_stereo:
		from_stereo_to_quad_side(Array_in, Array_out, Samples);
		break;

	case channellayout_2_1:
		from_2_1_to_quad_side(Array_in, Array_out, Samples);
		break;

	case channellayout_3_0:
		from_3_0_to_quad_side(Array_in, Array_out, Samples);
		break;
	}
}

void from_X_to_3_1(float **Array_in, float **Array_out, unsigned int Samples, ADSP_CHANNEL_LAYOUT UpmixFrom)
{
	switch (UpmixFrom)
	{
	case channellayout_mono:
		from_mono_to_3_1(Array_in, Array_out, Samples);
		break;

	case channellayout_stereo:
		from_stereo_to_3_1(Array_in, Array_out, Samples);
		break;

	case channellayout_2_1:
		from_2_1_to_3_1(Array_in, Array_out, Samples);
		break;

	case channellayout_3_0:
		from_3_0_to_3_1(Array_in, Array_out, Samples);
		break;

	case channellayout_quad_side:
		from_quad_side_to_3_1(Array_in, Array_out, Samples);
		break;
	}
}

void from_X_to_5_0_side(float **Array_in, float **Array_out, unsigned int Samples, ADSP_CHANNEL_LAYOUT UpmixFrom)
{
	switch (UpmixFrom)
	{
	case channellayout_mono:
		from_mono_to_5_0_side(Array_in, Array_out, Samples);
		break;

	case channellayout_stereo:
		from_stereo_to_5_0_side(Array_in, Array_out, Samples);
		break;

	case channellayout_2_1:
		from_2_1_to_5_0_side(Array_in, Array_out, Samples);
		break;

	case channellayout_3_0:
		from_3_0_to_5_0_side(Array_in, Array_out, Samples);
		break;

	case channellayout_quad_side:
		from_quad_side_to_5_0_side(Array_in, Array_out, Samples);
		break;

	case channellayout_3_1:
		from_3_1_to_5_0_side(Array_in, Array_out, Samples);
		break;
	}
}

void from_X_to_4_1_side(float **Array_in, float **Array_out, unsigned int Samples, ADSP_CHANNEL_LAYOUT UpmixFrom)
{
	switch (UpmixFrom)
	{
	case channellayout_mono:
		from_mono_to_4_1_side(Array_in, Array_out, Samples);
		break;

	case channellayout_stereo:
		from_stereo_to_4_1_side(Array_in, Array_out, Samples);
		break;

	case channellayout_2_1:
		from_2_1_to_4_1_side(Array_in, Array_out, Samples);
		break;

	case channellayout_3_0:
		from_3_0_to_4_1_side(Array_in, Array_out, Samples);
		break;

	case channellayout_quad_side:
		from_quad_side_to_4_1_side(Array_in, Array_out, Samples);
		break;

	case channellayout_3_1:
		from_3_1_to_4_1_side(Array_in, Array_out, Samples);
		break;

	case channellayout_5_0_side:
		from_5_0_side_to_4_1_side(Array_in, Array_out, Samples);
		break;
	}
}

void from_X_to_5_1_side(float **Array_in, float **Array_out, unsigned int Samples, ADSP_CHANNEL_LAYOUT UpmixFrom)
{
	switch (UpmixFrom)
	{
	case channellayout_mono:
		from_mono_to_5_1_side(Array_in, Array_out, Samples);
		break;

	case channellayout_stereo:
		from_stereo_to_5_1_side(Array_in, Array_out, Samples);
		break;

	case channellayout_2_1:
		from_2_1_to_5_1_side(Array_in, Array_out, Samples);
		break;

	case channellayout_3_0:
		from_3_0_to_5_1_side(Array_in, Array_out, Samples);
		break;

	case channellayout_quad_side:
		from_quad_side_to_5_1_side(Array_in, Array_out, Samples);
		break;

	case channellayout_3_1:
		from_3_1_to_5_1_side(Array_in, Array_out, Samples);
		break;

	case channellayout_5_0_side:
		from_5_0_side_to_5_1_side(Array_in, Array_out, Samples);
		break;

	case channellayout_4_1_side:
		from_4_1_side_to_5_1_side(Array_in, Array_out, Samples);
		break;
	}
}

void from_X_to_7_0(float **Array_in, float **Array_out, unsigned int Samples, ADSP_CHANNEL_LAYOUT UpmixFrom)
{
	switch (UpmixFrom)
	{
	case channellayout_mono:
		from_mono_to_7_0(Array_in, Array_out, Samples);
		break;

	case channellayout_stereo:
		from_stereo_to_7_0(Array_in, Array_out, Samples);
		break;

	case channellayout_2_1:
		from_2_1_to_7_0(Array_in, Array_out, Samples);
		break;

	case channellayout_3_0:
		from_3_0_to_7_0(Array_in, Array_out, Samples);
		break;

	case channellayout_quad_side:
		from_quad_side_to_7_0(Array_in, Array_out, Samples);
		break;

	case channellayout_3_1:
		from_3_1_to_7_0(Array_in, Array_out, Samples);
		break;

	case channellayout_5_0_side:
		from_5_0_side_to_7_0(Array_in, Array_out, Samples);
		break;

	case channellayout_4_1_side:
		from_5_0_side_to_7_0(Array_in, Array_out, Samples);
		break;
	}
}

void from_X_to_7_1(float **Array_in, float **Array_out, unsigned int Samples, ADSP_CHANNEL_LAYOUT UpmixFrom)
{
	switch (UpmixFrom)
	{
	case channellayout_mono:
		from_mono_to_7_1(Array_in, Array_out, Samples);
		break;

	case channellayout_stereo:
		from_stereo_to_7_1(Array_in, Array_out, Samples);
		break;

	case channellayout_2_1:
		from_2_1_to_7_1(Array_in, Array_out, Samples);
		break;

	case channellayout_3_0:
		from_3_0_to_7_1(Array_in, Array_out, Samples);
		break;

	case channellayout_quad_side:
		from_quad_side_to_7_1(Array_in, Array_out, Samples);
		break;

	case channellayout_3_1:
		from_3_1_to_7_1(Array_in, Array_out, Samples);
		break;

	case channellayout_5_0_side:
		from_5_0_side_to_7_1(Array_in, Array_out, Samples);
		break;

	case channellayout_4_1_side:
		from_4_1_side_to_7_1(Array_in, Array_out, Samples);
		break;

	case channellayout_7_0:
		from_7_0_to_7_1(Array_in, Array_out, Samples);
		break;
	}
}

// functions for upmixing to mono stereo (2.0)
void from_mono_to_stereo(float **Array_in, float **Array_out, unsigned int Samples)
{
	memcpy(Array_out[AE_DSP_CH_FL], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FR], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
}

// functions for upmixing to 2.1
void from_mono_to_2_1(float **Array_in, float **Array_out, unsigned int Samples)
{
	memcpy(Array_out[AE_DSP_CH_FL], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FR], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_LFE], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
}

void from_stereo_to_2_1(float **Array_in, float **Array_out, unsigned int Samples)
{
	memcpy(Array_out[AE_DSP_CH_FL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));

	for (unsigned int ii = 0; ii < Samples; ii++)
	{
		Array_out[AE_DSP_CH_LFE][ii] = (Array_out[AE_DSP_CH_FL][ii] + Array_out[AE_DSP_CH_FR][ii]) / 2.0f;
	}
}

// functions for upmixing to 3.0
void from_mono_to_3_0(float **Array_in, float **Array_out, unsigned int Samples)
{
	memcpy(Array_out[AE_DSP_CH_FL], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FR], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FC], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
}

void from_stereo_to_3_0(float **Array_in, float **Array_out, unsigned int Samples)
{
	memcpy(Array_out[AE_DSP_CH_FL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));

	// sum up FR, FL to FC
	for (unsigned int ii = 0; ii < Samples; ii++)
	{
		Array_out[AE_DSP_CH_FC][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_FR][ii]) / 2.0f;
	}
}

void from_2_1_to_3_0(float **Array_in, float **Array_out, unsigned int Samples)
{
	for (unsigned int ii = 0; ii < Samples; ii++)
	{
		Array_out[AE_DSP_CH_FL][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_LFE][ii]) / 2.0f;
		Array_out[AE_DSP_CH_FR][ii] = (Array_in[AE_DSP_CH_FR][ii] + Array_in[AE_DSP_CH_LFE][ii]) / 2.0f;
		Array_out[AE_DSP_CH_FC][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_FR][ii] + Array_in[AE_DSP_CH_LFE][ii]) / 3.0f;
	}
}

// functions for upmixing to quad_side
void from_mono_to_quad_side(float **Array_in, float **Array_out, unsigned int Samples)
{
	memcpy(Array_out[AE_DSP_CH_FL], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FR], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SL], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SR], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
}

void from_stereo_to_quad_side(float **Array_in, float **Array_out, unsigned int Samples)
{
	memcpy(Array_out[AE_DSP_CH_FL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));
}

void from_2_1_to_quad_side(float **Array_in, float **Array_out, unsigned int Samples)
{
	for (unsigned int ii = 0; ii < Samples; ii++)
	{
		Array_out[AE_DSP_CH_FL][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_LFE][ii]) / 2.0f;
		Array_out[AE_DSP_CH_FR][ii] = (Array_in[AE_DSP_CH_FR][ii] + Array_in[AE_DSP_CH_LFE][ii]) / 2.0f;
		Array_out[AE_DSP_CH_SL][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_LFE][ii]) / 2.0f;
		Array_out[AE_DSP_CH_SR][ii] = (Array_in[AE_DSP_CH_FR][ii] + Array_in[AE_DSP_CH_LFE][ii]) / 2.0f;
	}
}

void from_3_0_to_quad_side(float **Array_in, float **Array_out, unsigned int Samples)
{
	for (unsigned int ii = 0; ii < Samples; ii++)
	{
		Array_out[AE_DSP_CH_FL][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_FC][ii]) / 2.0f;
		Array_out[AE_DSP_CH_FR][ii] = (Array_in[AE_DSP_CH_FR][ii] + Array_in[AE_DSP_CH_FC][ii]) / 2.0f;
		Array_out[AE_DSP_CH_SL][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_FC][ii]) / 2.0f;
		Array_out[AE_DSP_CH_SR][ii] = (Array_in[AE_DSP_CH_FR][ii] + Array_in[AE_DSP_CH_FC][ii]) / 2.0f;
	}
}

// functions for upmixing to 3_1
void from_mono_to_3_1(float **Array_in, float **Array_out, unsigned int Samples)
{
	memcpy(Array_out[AE_DSP_CH_FL], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FR], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FC], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_LFE], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
}

void from_stereo_to_3_1(float **Array_in, float **Array_out, unsigned int Samples)
{
	memcpy(Array_out[AE_DSP_CH_FL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));

	for (unsigned int ii = 0; ii < Samples; ii++)
	{
		Array_out[AE_DSP_CH_LFE][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_FR][ii]) / 2.0f;
		Array_out[AE_DSP_CH_FC][ii] = Array_out[AE_DSP_CH_LFE][ii];
	}
}

void from_2_1_to_3_1(float **Array_in, float **Array_out, unsigned int Samples)
{
	memcpy(Array_out[AE_DSP_CH_FL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_LFE], Array_in[AE_DSP_CH_LFE], Samples*sizeof(float));

	for (unsigned int ii = 0; ii < Samples; ii++)
	{
		Array_out[AE_DSP_CH_FC][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_FR][ii]) / 2.0f;
	}
}

void from_3_0_to_3_1(float **Array_in, float **Array_out, unsigned int Samples)
{
	memcpy(Array_out[AE_DSP_CH_FL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FC], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));

	for (unsigned int ii = 0; ii < Samples; ii++)
	{
		Array_out[AE_DSP_CH_LFE][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_FR][ii] + Array_in[AE_DSP_CH_FC][ii]) / 3.0f;
	}
}

void from_quad_side_to_3_1(float **Array_in, float **Array_out, unsigned int Samples)
{
	for (unsigned int ii = 0; ii < Samples; ii++)
	{
		Array_out[AE_DSP_CH_FL][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_SL][ii]) / 2.0f;
		Array_out[AE_DSP_CH_FR][ii] = (Array_in[AE_DSP_CH_FR][ii] + Array_in[AE_DSP_CH_SR][ii]) / 2.0f;
		Array_out[AE_DSP_CH_FC][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_FR][ii] + Array_in[AE_DSP_CH_SL][ii] + Array_in[AE_DSP_CH_SR][ii]) / 4.0f;
		Array_out[AE_DSP_CH_LFE][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_FR][ii] + Array_in[AE_DSP_CH_SL][ii] + Array_in[AE_DSP_CH_SR][ii]) / 4.0f;
	}
}

// functions for upmixing to 5_0_side
void from_mono_to_5_0_side(float **Array_in, float **Array_out, unsigned int Samples)
{
	memcpy(Array_out[AE_DSP_CH_FL], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FR], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FC], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SL], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SR], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
}

void from_stereo_to_5_0_side(float **Array_in, float **Array_out, unsigned int Samples)
{
	memcpy(Array_out[AE_DSP_CH_FL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));

	for (unsigned int ii = 0; ii < Samples; ii++)
	{
		Array_out[AE_DSP_CH_FC][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_FR][ii]) / 2.0f;
	}
}

void from_2_1_to_5_0_side(float **Array_in, float **Array_out, unsigned int Samples)
{
	for (unsigned int ii = 0; ii < Samples; ii++)
	{
		Array_out[AE_DSP_CH_FL][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_LFE][ii]) / 2.0f;
		Array_out[AE_DSP_CH_FR][ii] = (Array_in[AE_DSP_CH_FR][ii] + Array_in[AE_DSP_CH_LFE][ii]) / 2.0f;
		Array_out[AE_DSP_CH_FC][ii] = (Array_in[AE_DSP_CH_FR][ii] + Array_in[AE_DSP_CH_FR][ii] + Array_in[AE_DSP_CH_LFE][ii]) / 3.0f;
		Array_out[AE_DSP_CH_SL][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_LFE][ii]) / 2.0f;
		Array_out[AE_DSP_CH_SR][ii] = (Array_in[AE_DSP_CH_FR][ii] + Array_in[AE_DSP_CH_LFE][ii]) / 2.0f;
	}
}

void from_3_0_to_5_0_side(float **Array_in, float **Array_out, unsigned int Samples)
{
	memcpy(Array_out[AE_DSP_CH_FL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FC], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));
}

void from_quad_side_to_5_0_side(float **Array_in, float **Array_out, unsigned int Samples)
{
	memcpy(Array_out[AE_DSP_CH_FL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SL], Array_in[AE_DSP_CH_SL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SR], Array_in[AE_DSP_CH_SR], Samples*sizeof(float));

	for (unsigned int ii = 0; ii < Samples; ii++)
	{
		Array_out[AE_DSP_CH_FC][ii] = (Array_in[AE_DSP_CH_FR][ii] + Array_in[AE_DSP_CH_FR][ii]) / 2.0f;
	}
}

void from_3_1_to_5_0_side(float **Array_in, float **Array_out, unsigned int Samples)
{
	for (unsigned int ii = 0; ii < Samples; ii++)
	{
		Array_out[AE_DSP_CH_FL][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_LFE][ii]) / 2.0f;
		Array_out[AE_DSP_CH_FR][ii] = (Array_in[AE_DSP_CH_FR][ii] + Array_in[AE_DSP_CH_LFE][ii]) / 2.0f;
		Array_out[AE_DSP_CH_FC][ii] = (Array_in[AE_DSP_CH_FC][ii] + Array_in[AE_DSP_CH_LFE][ii]) / 2.0f;
		Array_out[AE_DSP_CH_SL][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_LFE][ii]) / 2.0f;
		Array_out[AE_DSP_CH_SR][ii] = (Array_in[AE_DSP_CH_FR][ii] + Array_in[AE_DSP_CH_LFE][ii]) / 2.0f;
	}
}

// functions for upmixing to 4_1_side
void from_mono_to_4_1_side(float **Array_in, float **Array_out, unsigned int Samples)
{
	memcpy(Array_out[AE_DSP_CH_FL], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FR], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_LFE], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SL], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SR], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
}

void from_stereo_to_4_1_side(float **Array_in, float **Array_out, unsigned int Samples)
{
	memcpy(Array_out[AE_DSP_CH_FL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));

	for (unsigned int ii = 0; ii < Samples; ii++)
	{
		Array_out[AE_DSP_CH_LFE][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_FR][ii]) / 2.0f;
	}
}

void from_2_1_to_4_1_side(float **Array_in, float **Array_out, unsigned int Samples)
{
	memcpy(Array_out[AE_DSP_CH_FL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_LFE], Array_in[AE_DSP_CH_LFE], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));
}

void from_3_0_to_4_1_side(float **Array_in, float **Array_out, unsigned int Samples)
{
	for (unsigned int ii = 0; ii < Samples; ii++)
	{
		Array_out[AE_DSP_CH_FL][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_FC][ii]) / 2.0f;
		Array_out[AE_DSP_CH_FR][ii] = (Array_in[AE_DSP_CH_FR][ii] + Array_in[AE_DSP_CH_FC][ii]) / 2.0f;
		Array_out[AE_DSP_CH_LFE][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_FR][ii] + Array_in[AE_DSP_CH_FC][ii]) / 3.0f;
		Array_out[AE_DSP_CH_SL][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_FC][ii]) / 2.0f;
		Array_out[AE_DSP_CH_SR][ii] = (Array_in[AE_DSP_CH_FR][ii] + Array_in[AE_DSP_CH_FC][ii]) / 2.0f;
	}
}

void from_quad_side_to_4_1_side(float **Array_in, float **Array_out, unsigned int Samples)
{
	memcpy(Array_out[AE_DSP_CH_FL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SL], Array_in[AE_DSP_CH_SL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SR], Array_in[AE_DSP_CH_SR], Samples*sizeof(float));

	for (unsigned int ii = 0; ii < Samples; ii++)
	{
		Array_out[AE_DSP_CH_LFE][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_FR][ii] + Array_in[AE_DSP_CH_SL][ii] + Array_in[AE_DSP_CH_SR][ii]) / 4.0f;
	}
}

void from_3_1_to_4_1_side(float **Array_in, float **Array_out, unsigned int Samples)
{
	for (unsigned int ii = 0; ii < Samples; ii++)
	{
		Array_out[AE_DSP_CH_FL][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_FC][ii]) / 2.0f;
		Array_out[AE_DSP_CH_FR][ii] = (Array_in[AE_DSP_CH_FR][ii] + Array_in[AE_DSP_CH_FC][ii]) / 2.0f;
		Array_out[AE_DSP_CH_LFE][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_FR][ii] + Array_in[AE_DSP_CH_FC][ii]) / 3.0f;
		Array_out[AE_DSP_CH_SL][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_FC][ii]) / 2.0f;
		Array_out[AE_DSP_CH_SR][ii] = (Array_in[AE_DSP_CH_FR][ii] + Array_in[AE_DSP_CH_FC][ii]) / 2.0f;
	}
}

void from_5_0_side_to_4_1_side(float **Array_in, float **Array_out, unsigned int Samples)
{
	memcpy(Array_out[AE_DSP_CH_SL], Array_in[AE_DSP_CH_SL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SR], Array_in[AE_DSP_CH_SR], Samples*sizeof(float));

	for (unsigned int ii = 0; ii < Samples; ii++)
	{
		Array_out[AE_DSP_CH_FL][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_FC][ii]) / 2.0f;
		Array_out[AE_DSP_CH_FR][ii] = (Array_in[AE_DSP_CH_FR][ii] + Array_in[AE_DSP_CH_FC][ii]) / 2.0f;
		Array_out[AE_DSP_CH_LFE][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_FR][ii]
			+ Array_in[AE_DSP_CH_FC][ii] + Array_in[AE_DSP_CH_SL][ii] + Array_in[AE_DSP_CH_SR][ii]) / 5.0f;
	}
}


// functions for upmixing to 5_1_side
void from_mono_to_5_1_side(float **Array_in, float **Array_out, unsigned int Samples)
{
	memcpy(Array_out[AE_DSP_CH_FL], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FR], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FC], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_LFE], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SL], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SR], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
}

void from_stereo_to_5_1_side(float **Array_in, float **Array_out, unsigned int Samples)
{
	memcpy(Array_out[AE_DSP_CH_FL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));

	for (unsigned int ii = 0; ii < Samples; ii++)
	{
		Array_out[AE_DSP_CH_FC][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_FR][ii]) / 2.0f;
		Array_out[AE_DSP_CH_LFE][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_FR][ii]) / 2.0f;
	}
}

void from_2_1_to_5_1_side(float **Array_in, float **Array_out, unsigned int Samples)
{
	memcpy(Array_out[AE_DSP_CH_FL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_LFE], Array_in[AE_DSP_CH_LFE], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));

	for (unsigned int ii = 0; ii < Samples; ii++)
	{
		Array_out[AE_DSP_CH_FC][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_FR][ii]) / 2.0f;
	}
}

void from_3_0_to_5_1_side(float **Array_in, float **Array_out, unsigned int Samples)
{
	memcpy(Array_out[AE_DSP_CH_FL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FC], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));

	for (unsigned int ii = 0; ii < Samples; ii++)
	{
		Array_out[AE_DSP_CH_LFE][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_FR][ii] + Array_in[AE_DSP_CH_FC][ii]) / 3.0f;
	}
}

void from_quad_side_to_5_1_side(float **Array_in, float **Array_out, unsigned int Samples)
{
	memcpy(Array_out[AE_DSP_CH_FL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SL], Array_in[AE_DSP_CH_SL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SR], Array_in[AE_DSP_CH_SR], Samples*sizeof(float));

	for (unsigned int ii = 0; ii < Samples; ii++)
	{
		Array_out[AE_DSP_CH_FC][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_FR][ii]) / 2.0f;
		Array_out[AE_DSP_CH_LFE][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_FR][ii]
			+ Array_in[AE_DSP_CH_SL][ii] + Array_in[AE_DSP_CH_SL][ii]) / 4.0f;
	}
}

void from_3_1_to_5_1_side(float **Array_in, float **Array_out, unsigned int Samples)
{
	memcpy(Array_out[AE_DSP_CH_FL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FC], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_LFE], Array_in[AE_DSP_CH_LFE], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));
}

void from_5_0_side_to_5_1_side(float **Array_in, float **Array_out, unsigned int Samples)
{
	memcpy(Array_out[AE_DSP_CH_FL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FC], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));

	for (unsigned int ii = 0; ii < Samples; ii++)
	{
		Array_out[AE_DSP_CH_LFE][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_FR][ii]
			+ Array_in[AE_DSP_CH_FC][ii] + Array_in[AE_DSP_CH_SL][ii] + Array_in[AE_DSP_CH_SR][ii]) / 5.0f;
	}
}

void from_4_1_side_to_5_1_side(float **Array_in, float **Array_out, unsigned int Samples)
{
	memcpy(Array_out[AE_DSP_CH_FL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_LFE], Array_in[AE_DSP_CH_LFE], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));

	for (unsigned int ii = 0; ii < Samples; ii++)
	{
		Array_out[AE_DSP_CH_FC][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_FR][ii]) / 2.0f;
	}
}

// functions for upmixing to 7_0_side
void from_mono_to_7_0(float **Array_in, float **Array_out, unsigned int Samples)
{
	memcpy(Array_out[AE_DSP_CH_FL], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FR], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FC], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_BL], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_BR], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SL], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SR], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
}

void from_stereo_to_7_0(float **Array_in, float **Array_out, unsigned int Samples)
{
	memcpy(Array_out[AE_DSP_CH_FL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_BL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_BR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SL], Array_in[AE_DSP_CH_SL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SR], Array_in[AE_DSP_CH_SR], Samples*sizeof(float));

	for (unsigned int ii = 0; ii < Samples; ii++)
	{
		Array_out[AE_DSP_CH_FC][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_FR][ii]) / 2.0f;
	}
}

void from_2_1_to_7_0(float **Array_in, float **Array_out, unsigned int Samples)
{
	for (unsigned int ii = 0; ii < Samples; ii++)
	{
		Array_out[AE_DSP_CH_FL][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_LFE][ii]) / 2.0f;
		Array_out[AE_DSP_CH_FR][ii] = (Array_in[AE_DSP_CH_FR][ii] + Array_in[AE_DSP_CH_LFE][ii]) / 2.0f;
		Array_out[AE_DSP_CH_FC][ii] = (Array_in[AE_DSP_CH_FR][ii] + Array_in[AE_DSP_CH_FR][ii] + Array_in[AE_DSP_CH_LFE][ii]) / 3.0f;
		Array_out[AE_DSP_CH_BL][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_LFE][ii]) / 2.0f;
		Array_out[AE_DSP_CH_BR][ii] = (Array_in[AE_DSP_CH_FR][ii] + Array_in[AE_DSP_CH_LFE][ii]) / 2.0f;
		Array_out[AE_DSP_CH_SL][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_LFE][ii]) / 2.0f;
		Array_out[AE_DSP_CH_SR][ii] = (Array_in[AE_DSP_CH_FR][ii] + Array_in[AE_DSP_CH_LFE][ii]) / 2.0f;
	}
}

void from_3_0_to_7_0(float **Array_in, float **Array_out, unsigned int Samples)
{
	memcpy(Array_out[AE_DSP_CH_FL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FC], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_BL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_BR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));
}

void from_quad_side_to_7_0(float **Array_in, float **Array_out, unsigned int Samples)
{
	memcpy(Array_out[AE_DSP_CH_FL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_BL], Array_in[AE_DSP_CH_SL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_BR], Array_in[AE_DSP_CH_SR], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SL], Array_in[AE_DSP_CH_SL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SR], Array_in[AE_DSP_CH_SR], Samples*sizeof(float));

	for (unsigned int ii = 0; ii < Samples; ii++)
	{
		Array_out[AE_DSP_CH_FC][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_FR][ii]) / 2.0f;
	}
}

void from_3_1_to_7_0(float **Array_in, float **Array_out, unsigned int Samples)
{
	for (unsigned int ii = 0; ii < Samples; ii++)
	{
		Array_out[AE_DSP_CH_FL][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_LFE][ii]) / 2.0f;
		Array_out[AE_DSP_CH_FR][ii] = (Array_in[AE_DSP_CH_FR][ii] + Array_in[AE_DSP_CH_LFE][ii]) / 2.0f;
		Array_out[AE_DSP_CH_FC][ii] = (Array_in[AE_DSP_CH_FR][ii] + Array_in[AE_DSP_CH_FR][ii] + Array_in[AE_DSP_CH_LFE][ii]) / 3.0f;
		Array_out[AE_DSP_CH_BL][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_LFE][ii]) / 2.0f;
		Array_out[AE_DSP_CH_BR][ii] = (Array_in[AE_DSP_CH_FR][ii] + Array_in[AE_DSP_CH_LFE][ii]) / 2.0f;
		Array_out[AE_DSP_CH_SL][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_LFE][ii]) / 2.0f;
		Array_out[AE_DSP_CH_SR][ii] = (Array_in[AE_DSP_CH_FR][ii] + Array_in[AE_DSP_CH_LFE][ii]) / 2.0f;
	}
}

void from_5_0_side_to_7_0(float **Array_in, float **Array_out, unsigned int Samples)
{
	memcpy(Array_out[AE_DSP_CH_FL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FC], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_BL], Array_in[AE_DSP_CH_SL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_BR], Array_in[AE_DSP_CH_SR], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SL], Array_in[AE_DSP_CH_SL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SR], Array_in[AE_DSP_CH_SR], Samples*sizeof(float));
}

void from_4_1_side_to_7_0(float **Array_in, float **Array_out, unsigned int Samples)
{
	for (unsigned int ii = 0; ii < Samples; ii++)
	{
		Array_out[AE_DSP_CH_FL][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_LFE][ii]) / 2.0f;
		Array_out[AE_DSP_CH_FR][ii] = (Array_in[AE_DSP_CH_FR][ii] + Array_in[AE_DSP_CH_LFE][ii]) / 2.0f;
		Array_out[AE_DSP_CH_FC][ii] = (Array_in[AE_DSP_CH_FR][ii] + Array_in[AE_DSP_CH_FR][ii] + Array_in[AE_DSP_CH_LFE][ii]) / 3.0f;
		Array_out[AE_DSP_CH_BL][ii] = (Array_in[AE_DSP_CH_SL][ii] + Array_in[AE_DSP_CH_LFE][ii]) / 2.0f;
		Array_out[AE_DSP_CH_BR][ii] = (Array_in[AE_DSP_CH_SR][ii] + Array_in[AE_DSP_CH_LFE][ii]) / 2.0f;
		Array_out[AE_DSP_CH_SL][ii] = (Array_in[AE_DSP_CH_SL][ii] + Array_in[AE_DSP_CH_LFE][ii]) / 2.0f;
		Array_out[AE_DSP_CH_SR][ii] = (Array_in[AE_DSP_CH_SR][ii] + Array_in[AE_DSP_CH_LFE][ii]) / 2.0f;
	}
}

void from_5_1_to_7_0(float **Array_in, float **Array_out, unsigned int Samples)
{
	for (unsigned int ii = 0; ii < Samples; ii++)
	{
		Array_out[AE_DSP_CH_FL][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_LFE][ii]) / 2.0f;
		Array_out[AE_DSP_CH_FR][ii] = (Array_in[AE_DSP_CH_FR][ii] + Array_in[AE_DSP_CH_LFE][ii]) / 2.0f;
		Array_out[AE_DSP_CH_FC][ii] = (Array_in[AE_DSP_CH_FC][ii] + Array_in[AE_DSP_CH_LFE][ii]) / 2.0f;
		Array_out[AE_DSP_CH_BL][ii] = (Array_in[AE_DSP_CH_SL][ii] + Array_in[AE_DSP_CH_LFE][ii]) / 2.0f;
		Array_out[AE_DSP_CH_BR][ii] = (Array_in[AE_DSP_CH_SR][ii] + Array_in[AE_DSP_CH_LFE][ii]) / 2.0f;
		Array_out[AE_DSP_CH_SL][ii] = (Array_in[AE_DSP_CH_SL][ii] + Array_in[AE_DSP_CH_LFE][ii]) / 2.0f;
		Array_out[AE_DSP_CH_SR][ii] = (Array_in[AE_DSP_CH_SR][ii] + Array_in[AE_DSP_CH_LFE][ii]) / 2.0f;
	}
}

// functions for upmixing to 7_1
void from_mono_to_7_1(float **Array_in, float **Array_out, unsigned int Samples)
{
	memcpy(Array_out[AE_DSP_CH_FL], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FR], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FC], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_LFE], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_BL], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_BR], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SL], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SR], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
}

void from_stereo_to_7_1(float **Array_in, float **Array_out, unsigned int Samples)
{
	memcpy(Array_out[AE_DSP_CH_FL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_BL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_BR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));

	// sum up FR, FL to LFE & FC
	for (unsigned int ii = 0; ii < Samples; ii++)
	{
		Array_out[AE_DSP_CH_FC][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_FR][ii]) / 2.0f;
		Array_out[AE_DSP_CH_LFE][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_FR][ii]) / 2.0f;
	}
}

void from_2_1_to_7_1(float **Array_in, float **Array_out, unsigned int Samples)
{
	memcpy(Array_out[AE_DSP_CH_FL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_LFE], Array_in[AE_DSP_CH_LFE], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_BL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_BR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));

	// sum up FR, FL to LFE & FC
	for (unsigned int ii = 0; ii < Samples; ii++)
	{
		Array_out[AE_DSP_CH_FC][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_FR][ii]) / 2.0f;
	}
}

void from_3_0_to_7_1(float **Array_in, float **Array_out, unsigned int Samples)
{
	memcpy(Array_out[AE_DSP_CH_FL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FC], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_BL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_BR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));

	// sum up FR, FL to LFE & FC
	for (unsigned int ii = 0; ii < Samples; ii++)
	{
		Array_out[AE_DSP_CH_LFE][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_FR][ii] + Array_in[AE_DSP_CH_FC][ii]) / 3.0f;
	}
}

void from_quad_side_to_7_1(float **Array_in, float **Array_out, unsigned int Samples)
{
	memcpy(Array_out[AE_DSP_CH_FL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_BL], Array_in[AE_DSP_CH_SL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_BR], Array_in[AE_DSP_CH_SR], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SL], Array_in[AE_DSP_CH_SL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SR], Array_in[AE_DSP_CH_SR], Samples*sizeof(float));

	// sum up FR, FL to LFE & FC
	for (unsigned int ii = 0; ii < Samples; ii++)
	{
		Array_out[AE_DSP_CH_FC][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_FR][ii]) / 2.0f;
		Array_out[AE_DSP_CH_LFE][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_FR][ii]
			+ Array_in[AE_DSP_CH_SL][ii] + Array_in[AE_DSP_CH_SR][ii]) / 4.0f;
	}
}

void from_3_1_to_7_1(float **Array_in, float **Array_out, unsigned int Samples)
{
	memcpy(Array_out[AE_DSP_CH_FL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FC], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_LFE], Array_in[AE_DSP_CH_LFE], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_BL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_BR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));
}

void from_5_0_side_to_7_1(float **Array_in, float **Array_out, unsigned int Samples)
{
	memcpy(Array_out[AE_DSP_CH_FL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FC], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_BL], Array_in[AE_DSP_CH_SL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_BR], Array_in[AE_DSP_CH_SR], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SL], Array_in[AE_DSP_CH_SL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SR], Array_in[AE_DSP_CH_SR], Samples*sizeof(float));

	for (unsigned int ii = 0; ii < Samples; ii++)
	{
		Array_out[AE_DSP_CH_LFE][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_FR][ii]
			+ Array_in[AE_DSP_CH_FC][ii] + Array_in[AE_DSP_CH_SL][ii] + Array_in[AE_DSP_CH_SR][ii]) / 5.0f;
	}
}

void from_4_1_side_to_7_1(float **Array_in, float **Array_out, unsigned int Samples)
{
	memcpy(Array_out[AE_DSP_CH_FL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_LFE], Array_in[AE_DSP_CH_LFE], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_BL], Array_in[AE_DSP_CH_SL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_BR], Array_in[AE_DSP_CH_SR], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SL], Array_in[AE_DSP_CH_SL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SR], Array_in[AE_DSP_CH_SR], Samples*sizeof(float));

	for (unsigned int ii = 0; ii < Samples; ii++)
	{
		Array_out[AE_DSP_CH_FC][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_FR][ii]) / 2.0f;
	}
}

void from_5_1_to_7_1(float **Array_in, float **Array_out, unsigned int Samples)
{
	memcpy(Array_out[AE_DSP_CH_FL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FC], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_LFE], Array_in[AE_DSP_CH_LFE], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_BL], Array_in[AE_DSP_CH_SL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_BR], Array_in[AE_DSP_CH_SR], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SL], Array_in[AE_DSP_CH_SL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SR], Array_in[AE_DSP_CH_SR], Samples*sizeof(float));
}

void from_7_0_to_7_1(float **Array_in, float **Array_out, unsigned int Samples)
{
	memcpy(Array_out[AE_DSP_CH_FL], Array_in[AE_DSP_CH_FL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FR], Array_in[AE_DSP_CH_FR], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_FC], Array_in[AE_DSP_CH_FC], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_BL], Array_in[AE_DSP_CH_BL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_BR], Array_in[AE_DSP_CH_BR], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SL], Array_in[AE_DSP_CH_SL], Samples*sizeof(float));
	memcpy(Array_out[AE_DSP_CH_SR], Array_in[AE_DSP_CH_SR], Samples*sizeof(float));

	for (unsigned int ii = 0; ii < Samples; ii++)
	{
		Array_out[AE_DSP_CH_LFE][ii] = (Array_in[AE_DSP_CH_FL][ii] + Array_in[AE_DSP_CH_FR][ii]
			+ Array_in[AE_DSP_CH_FC][ii]
			+ Array_in[AE_DSP_CH_BL][ii] + Array_in[AE_DSP_CH_BR][ii]
			+ Array_in[AE_DSP_CH_SL][ii] + Array_in[AE_DSP_CH_SR][ii]) / 7.0f;
	}
}