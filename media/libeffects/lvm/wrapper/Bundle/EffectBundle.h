/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ANDROID_EFFECTBUNDLE_H_
#define ANDROID_EFFECTBUNDLE_H_

#include <media/EffectApi.h>
#include <LVM.h>

#if __cplusplus
extern "C" {
#endif

#define FIVEBAND_NUMBANDS       5
#define MAX_NUM_BANDS           5
#define MAX_CALL_SIZE           256

//TODO: this should be included from each effect API include
static const effect_uuid_t SL_IID_BASSBOOST_ = { 0x0634f220, 0xddd4, 0x11db, 0xa0fc, { 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b } };
const effect_uuid_t * const SL_IID_BASSBOOST = &SL_IID_BASSBOOST_;

static const effect_uuid_t SL_IID_EQUALIZER_ = { 0x0bed4300, 0xddd6, 0x11db, 0x8f34, { 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b } };
const effect_uuid_t * const SL_IID_EQUALIZER = &SL_IID_EQUALIZER_;

static const effect_uuid_t SL_IID_VIRTUALIZER_ = { 0x37cc2c00, 0xdddd, 0x11db, 0x8577, { 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b } }; // updated mon 28th june 2010
const effect_uuid_t * const SL_IID_VIRTUALIZER = &SL_IID_VIRTUALIZER_;

static const effect_uuid_t SL_IID_VOLUME_ = { 0x09e8ede0, 0xddde, 0x11db, 0xb4f6, { 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b } }; // updated mon 28th june 2010
const effect_uuid_t * const SL_IID_VOLUME = &SL_IID_VOLUME_;

typedef enum
{
    LVM_BASS_BOOST,
    LVM_VIRTUALIZER,
    LVM_EQUALIZER,    
    LVM_VOLUME
} lvm_effect_en;

// Preset configuration.
struct PresetConfig {
    // Human-readable name.
    const char * name;
    // An array of size nBands where each element is a configuration for the
    // corresponding band.
    //const BandConfig * bandConfigs;
};

/* BundledEffectContext : One per session */
struct BundledEffectContext{
    LVM_Handle_t                    hInstance;                      /* Instance handle */
    int                             SessionNo;                      /* Current session number */
    bool                            bVolumeEnabled;                 /* Flag for Volume */
    bool                            bEqualizerEnabled;              /* Flag for EQ */
    bool                            bBassEnabled;                   /* Flag for Bass */
    bool                            bVirtualizerEnabled;            /* Flag for Virtualizer */
    int                             NumberEffectsEnabled;           /* Effects in this session */
    int                             NumberEffectsCalled;            /* Effects called so far */    
    // Saved parameters for each effect */
    // Bass Boost
    int                             BassStrengthSaved;             /* Conversion between Get/Set */ 
    // Equalizer
    int                             CurPreset;                      /* Current preset being used */
    // Virtualzer
    int                             VirtStrengthSaved;              /* Conversion between Get/Set */
    // Volume
    int                             levelSaved;     /* for when mute is set, level must be saved */
    bool                            bMuteEnabled;   /* Must store as mute = -96dB level */       
};

/* SessionContext : One session */
struct SessionContext{
    bool                            bBundledEffectsEnabled;
    bool                            bVolumeInstantiated; 
    bool                            bEqualizerInstantiated;
    bool                            bBassInstantiated;
    bool                            bVirtualizerInstantiated; 
    BundledEffectContext            *pBundledContext;             
};

struct EffectContext{
    const struct effect_interface_s *itfe;
    effect_config_t                 config;
    lvm_effect_en                   EffectType;
    BundledEffectContext            *pBundledContext; 
};

//TODO: this should be included from each effect API include
/* enumerated parameter settings for BassBoost effect */
typedef enum
{
    BASSBOOST_PARAM_STRENGTH_SUP,        // type SLboolean  = typedef SLuint32
    BASSBOOST_PARAM_STRENGTH             // type SLpermille = typedef SLuint16
} t_bassboost_params;

/* enumerated parameter settings for Virtualizer effect */
typedef enum
{
    VIRTUALIZER_PARAM_STRENGTH_SUP,        // type SLboolean  = typedef SLuint32
    VIRTUALIZER_PARAM_STRENGTH             // type SLpermille = typedef SLuint16
} t_virtualizer_params;

/* enumerated parameter settings for Equalizer effect */
typedef enum
{
    EQ_PARAM_NUM_BANDS,             // Gets the number of frequency bands that the equalizer supports.
    EQ_PARAM_LEVEL_RANGE,           // Returns the minimum and maximum band levels supported.
    EQ_PARAM_BAND_LEVEL,            // Gets/Sets the gain set for the given equalizer band.
    EQ_PARAM_CENTER_FREQ,           // Gets the center frequency of the given band.
    EQ_PARAM_BAND_FREQ_RANGE,       // Gets the frequency range of the given frequency band.
    EQ_PARAM_GET_BAND,              // Gets the band that has the most effect on the given frequency.
    EQ_PARAM_CUR_PRESET,            // Gets/Sets the current preset.
    EQ_PARAM_GET_NUM_OF_PRESETS,    // Gets the total number of presets the equalizer supports.
    EQ_PARAM_GET_PRESET_NAME        // Gets the preset name based on the index.
} t_equalizer_params;

/* enumerated parameter settings for Volume effect */
typedef enum
{
    VOLUME_PARAM_LEVEL,                       // type SLmillibel = typedef SLuint16 (set & get)
    VOLUME_PARAM_MAXLEVEL,                    // type SLmillibel = typedef SLuint16 (get)
    VOLUME_PARAM_MUTE,                        // type SLboolean  = typedef SLuint32 (set & get)
    VOLUME_PARAM_ENABLESTEREOPOSITION,        // type SLboolean  = typedef SLuint32 (set & get)
    VOLUME_PARAM_STEREOPOSITION,              // type SLpermille = typedef SLuint16 (set & get)
} t_volume_params;

static const int PRESET_CUSTOM = -1;

static const uint32_t bandFreqRange[FIVEBAND_NUMBANDS][2] = {
	{30000, 120000},
	{12000, 460000},
	{46000, 1800000},
	{180000, 7000000},
	{700000, 1}
	};

static const LVM_UINT16  EQNB_5BandPresetsFrequencies[] = {
															60,           /* Frequencies in Hz */
															230,
															910,
															3600,
															14000};

static const LVM_UINT16 EQNB_5BandPresetsQFactors[] = {
		                                  96,               /* Q factor multiplied by 100 */
                                          96,
                                          96,
                                          96,
                                          96};

static const LVM_INT16 EQNB_5BandNormalPresets[] = {
									   3, 0, 0, 0, 3,       /* Normal Preset */
                                       8, 5, -3, 5, 6,      /* Classical Preset */
                                       15, -6, 7, 13, 10,   /* Dance Preset */
                                       0, 0, 0, 0, 0,       /* Flat Preset */
                                       6, -2, -2, 6, -3,    /* Folk Preset */
                                       8, -8, 13, -1, -4,   /* Heavy Metal Preset */
                                       10, 6, -4, 5, 8,     /* Hip Hop Preset */
                                       8, 5, -4, 5, 9,      /* Jazz Preset */
                                      -6, 4, 9, 4, -5,      /* Pop Preset */
                                       10, 6, -1, 8, 10};   /* Rock Preset */

static const PresetConfig gEqualizerPresets[] = {
    		{"Normal"},
    		{"Classical"},
    		{"Dance"},
    		{"Flat"},
    		{"Folk"},
    		{"Heavy Metal"},
    		{"Hip Hop"},
    		{"Jazz"},
    		{"Pop"},
    		{"Rock"}
    };

#if __cplusplus
}  // extern "C"
#endif


#endif /*ANDROID_EFFECTBUNDLE_H_*/
