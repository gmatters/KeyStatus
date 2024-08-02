/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2007 Adobe Systems Incorporated                       */
/* All Rights Reserved.                                            */
/*                                                                 */
/* NOTICE:  All information contained herein is, and remains the   */
/* property of Adobe Systems Incorporated and its suppliers, if    */
/* any.  The intellectual and technical concepts contained         */
/* herein are proprietary to Adobe Systems Incorporated and its    */
/* suppliers and may be covered by U.S. and Foreign Patents,       */
/* patents in process, and are protected by trade secret or        */
/* copyright law.  Dissemination of this information or            */
/* reproduction of this material is strictly forbidden unless      */
/* prior written permission is obtained from Adobe Systems         */
/* Incorporated.                                                   */
/*                                                                 */
/*******************************************************************/

/*	KeyStatus.cpp	

	This is a compiling husk of a project. Fill it in with interesting
	pixel processing code.
	
	Revision History

	Version		Change													Engineer	Date
	=======		======													========	======
	1.0			(seemed like a good idea at the time)					bbb			6/1/2002

	1.0			Okay, I'm leaving the version at 1.0,					bbb			2/15/2006
				for obvious reasons; you're going to 
				copy these files directly! This is the
				first XCode version, though.

	1.0			Let's simplify this barebones sample					zal			11/11/2010

	1.0			Added new entry point									zal			9/18/2017

*/

#include "KeyStatus.h"

static PF_Err 
About (	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	AEGP_SuiteHandler suites(in_data->pica_basicP);
	
	suites.ANSICallbacksSuite1()->sprintf(	out_data->return_msg,
											"%s v%d.%d\r%s",
											STR(StrID_Name), 
											MAJOR_VERSION, 
											MINOR_VERSION, 
											STR(StrID_Description));
	return PF_Err_NONE;
}

static PF_Err 
GlobalSetup (	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	out_data->my_version = PF_VERSION(	MAJOR_VERSION, 
										MINOR_VERSION,
										BUG_VERSION, 
										STAGE_VERSION, 
										BUILD_VERSION);

	out_data->out_flags =  PF_OutFlag_DEEP_COLOR_AWARE;	// just 16bpc, not 32bpc
	
	return PF_Err_NONE;
}

static PF_Err 
ParamsSetup (	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	PF_Err		err		= PF_Err_NONE;
	PF_ParamDef	def;

	AEFX_CLR_STRUCT(def);

	PF_ADD_COLOR(	STR(StrID_Color_Param_Name), 
					PF_HALF_CHAN8,
                    PF_HALF_CHAN8,
                    PF_HALF_CHAN8,
					COLOR_DISK_ID);
	
	out_data->num_params = KEYSTATUS_NUM_PARAMS;

	return err;
}

static PF_Err
KeyStatusFunc16 (
	void		*refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err		err = PF_Err_NONE;

	ColorInfo	*ciP	= reinterpret_cast<ColorInfo*>(refcon);
	PF_FpLong	red	= PF_HALF_CHAN16;
    PF_FpLong   green = PF_HALF_CHAN16;
    PF_FpLong   blue = PF_HALF_CHAN16;
					
	if (ciP){
		red = ciP->red * PF_MAX_CHAN16 / PF_MAX_CHAN8;
        green = ciP->green * PF_MAX_CHAN16 / PF_MAX_CHAN8;
        blue = ciP->blue * PF_MAX_CHAN16 / PF_MAX_CHAN8;
	}

    outP->alpha        =    PF_MAX_CHAN16;
    if (inP->alpha == 0) {
        outP->red = outP->green = outP->blue = 0;
    } else if (inP->alpha == PF_MAX_CHAN16) {
        outP->red = outP->green = outP->blue = PF_MAX_CHAN16;
    } else {
        outP->red = red;
        outP->green = green;
        outP->blue = blue;
    }

	return err;
}

static PF_Err
KeyStatusFunc8 (
	void		*refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err		err = PF_Err_NONE;

    ColorInfo    *ciP    = reinterpret_cast<ColorInfo*>(refcon);
    PF_FpLong    red    = PF_HALF_CHAN8;
    PF_FpLong   green = PF_HALF_CHAN8;
    PF_FpLong   blue = PF_HALF_CHAN8;
					
	if (ciP){
        red = ciP->red;
        green = ciP->green;
        blue = ciP->blue;
	}
    
    outP->alpha        =    PF_MAX_CHAN8;
    if (inP->alpha == 0) {
        outP->red = outP->green = outP->blue = 0;
    } else if (inP->alpha == PF_MAX_CHAN8) {
        outP->red = outP->green = outP->blue = PF_MAX_CHAN8;
    } else {
        outP->red = red;
        outP->green = green;
        outP->blue = blue;
    }

	return err;
}

static PF_Err 
Render (
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	PF_Err				err		= PF_Err_NONE;
	AEGP_SuiteHandler	suites(in_data->pica_basicP);

	/*	Put interesting code here. */
	ColorInfo			ciP;
	AEFX_CLR_STRUCT(ciP);
	A_long				linesL	= 0;

	linesL 		= output->extent_hint.bottom - output->extent_hint.top;
    ciP.red 	= params[KEYSTATUS_COLOR]->u.cd.value.red;
    ciP.green     = params[KEYSTATUS_COLOR]->u.cd.value.green;
    ciP.blue     = params[KEYSTATUS_COLOR]->u.cd.value.blue;
	
	if (PF_WORLD_IS_DEEP(output)){
		ERR(suites.Iterate16Suite1()->iterate(	in_data,
												0,								// progress base
												linesL,							// progress final
												&params[KEYSTATUS_INPUT]->u.ld,	// src 
												NULL,							// area - null for all pixels
												(void*)&ciP,					// refcon - your custom data pointer
												KeyStatusFunc16,				// pixel function pointer
												output));
	} else {
		ERR(suites.Iterate8Suite1()->iterate(	in_data,
												0,								// progress base
												linesL,							// progress final
												&params[KEYSTATUS_INPUT]->u.ld,	// src 
												NULL,							// area - null for all pixels
												(void*)&ciP,					// refcon - your custom data pointer
												KeyStatusFunc8,				// pixel function pointer
												output));	
	}

	return err;
}


extern "C" DllExport
PF_Err PluginDataEntryFunction(
	PF_PluginDataPtr inPtr,
	PF_PluginDataCB inPluginDataCallBackPtr,
	SPBasicSuite* inSPBasicSuitePtr,
	const char* inHostName,
	const char* inHostVersion)
{
	PF_Err result = PF_Err_INVALID_CALLBACK;

	result = PF_REGISTER_EFFECT(
		inPtr,
		inPluginDataCallBackPtr,
		"KeyStatus", // Name
		"ADBE KeyStatus", // Match Name
		"Keying", // Category
		AE_RESERVED_INFO); // Reserved Info

	return result;
}


PF_Err
EffectMain(
	PF_Cmd			cmd,
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output,
	void			*extra)
{
	PF_Err		err = PF_Err_NONE;
	
	try {
		switch (cmd) {
			case PF_Cmd_ABOUT:

				err = About(in_data,
							out_data,
							params,
							output);
				break;
				
			case PF_Cmd_GLOBAL_SETUP:

				err = GlobalSetup(	in_data,
									out_data,
									params,
									output);
				break;
				
			case PF_Cmd_PARAMS_SETUP:

				err = ParamsSetup(	in_data,
									out_data,
									params,
									output);
				break;
				
			case PF_Cmd_RENDER:

				err = Render(	in_data,
								out_data,
								params,
								output);
				break;
		}
	}
	catch(PF_Err &thrown_err){
		err = thrown_err;
	}
	return err;
}

