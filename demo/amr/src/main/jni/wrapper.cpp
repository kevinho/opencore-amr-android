/* ------------------------------------------------------------------
 * Copyright (C) 2009 Martin Storsjo
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 * -------------------------------------------------------------------
 */

#define AMRNB_WRAPPER_INTERNAL
#include <sp_dec.h>
#include <amrdecode.h>
#include <amrencode.h>
#include "interf_dec.h"
#include "interf_enc.h"
#include "opencore/codecs_v2/audio/gsm_amr/amr_nb/enc/src/amrencode.h"
#include "opencore/codecs_v2/audio/gsm_amr/amr_nb/dec/src/sp_dec.h"
#include "oscl/osclconfig_limits_typedefs.h"
#include "opencore/codecs_v2/audio/gsm_amr/amr_nb/dec/src/amrdecode.h"
#include "opencore/codecs_v2/audio/gsm_amr/amr_nb/common/include/gsm_amr_typedefs.h"
#include "opencore/codecs_v2/audio/gsm_amr/common/dec/include/pvgsmamrdecoderinterface.h"
#include <stdlib.h>

#ifndef DISABLE_AMRNB_DECODER
void* Decoder_Interface_init(void) {
	void* ptr = NULL;
	GSMInitDecode(&ptr, (int8*)"Decoder");
	return ptr;
}

void Decoder_Interface_exit(void* state) {
	GSMDecodeFrameExit(&state);
}

void Decoder_Interface_Decode(void* state, const unsigned char* in, short* out) {
	unsigned char type = (in[0] >> 3) & 0x0f;
	in++;
	AMRDecode(state, (enum Frame_Type_3GPP) type, (UWord8*) in, out, MIME_IETF);
}
#endif

#ifndef DISABLE_AMRNB_ENCODER

void* Encoder_Interface_init(int dtx) {
	struct encoder_state* state = (struct encoder_state*) malloc(sizeof(struct encoder_state));
	AMREncodeInit(&state->encCtx, &state->pidSyncCtx, dtx);
	return state;
}

void Encoder_Interface_reset(void* s){
	struct encoder_state* state = (struct encoder_state*) s;
	AMREncodeReset(&state->encCtx,&state->pidSyncCtx);
}

void Encoder_Interface_exit(void* s) {
	struct encoder_state* state = (struct encoder_state*) s;
	AMREncodeExit(&state->encCtx, &state->pidSyncCtx);
	free(state);
}

int Encoder_Interface_Encode(void* s, enum Mode mode, const short* in, unsigned char* out) {
	struct encoder_state* state = (struct encoder_state*) s;
	enum Frame_Type_3GPP frame_type = (enum Frame_Type_3GPP) mode;
	int ret = AMREncode(state->encCtx, state->pidSyncCtx, mode, (Word16*) in, out, &frame_type, AMR_TX_IETF);
	out[0] |= 0x04;
	return ret;
}
#endif

