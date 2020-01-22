//
//  PdAudioController.h
//  libpd
//
//  Created on 11/10/11.
//
//  For information on usage and redistribution, and for a DISCLAIMER OF ALL
//  WARRANTIES, see the file, "LICENSE.txt," in this distribution.
//
//  Updated 2018, 2020 Dan Wilcox <danomatika@gmail.com>
//

#import "PdAudioUnit.h"
#import <AVFoundation/AVFoundation.h>

/// PdAudioStatus is used to indicate success, failure, or that parameters had
/// to be adjusted in order to work
typedef enum PdAudioStatus {
	PdAudioOK = 0,             // success
	PdAudioError = -1,         // unrecoverable error
	PdAudioPropertyChanged = 1 // some properties have changed to run correctly
} PdAudioStatus;

/// PdAudioController: a class for managing a PdAudioUnit instance within iOS
/// by using the AVFoundation and Audio Services APIs, handles phone
/// interruptions and provides high level configuration methods by wrapping
/// relevant AVAudioSession methods
@interface PdAudioController : NSObject

/// check or set the active status of the audio unit
@property (nonatomic, getter=isActive) BOOL active;

#pragma mark Read Only Configuration Properties

// read only properties set by the configure methods

/// desired sample rate, may be different from session rate
@property (nonatomic, readonly) int sampleRate;

/// number of input channels, may not match number of session inputs
@property (nonatomic, readonly) int inputChannels;

/// number of output channels, may not match number of session inputs
@property (nonatomic, readonly) int outputChannels;

/// is the audio input stream enabled?
@property (nonatomic, readonly) BOOL inputEnabled;

/// number of pd ticks per buffer size, computed from session buffer duration
@property (nonatomic, readonly) int ticksPerBuffer;

/// read only access to the underlying pd audio unit
@property (nonatomic, strong, readonly) PdAudioUnit *audioUnit;

#pragma mark Audio Session Category Options

// audio session category options applied by the configure methods and while
// active, see Apple docs for category and category options info

/// is audio mixing with other apps enabled? (default YES)
/// applied to categories: PlayAndRecord, Playback, MultiRoute
@property (nonatomic, assign) BOOL mixWithOthers;

/// duck (ie. lower) audio output from other apps while active?
/// applied to categories: Ambient, PlayAndRecord, Playback, MultiRoute
@property (nonatomic, assign) BOOL duckOthers;

/// interrupt another app in AVAudioSessionModeSpokenAudio mode while active?
/// applied to categories: PlayAndRecord, Playback, MultiRoute
@property (nonatomic, assign) BOOL interruptSpokenAudioAndMixWithOthers;

/// output to speaker instead of receiver (earpiece)?  (default YES)
/// applied to categories: PlayAndRecord
@property (nonatomic, assign) BOOL defaultToSpeaker;

/// use Bluetooth HFP (Hands-Free Profile)?
/// note: this is traditional 1 channel IO, ie. headset/earpiece
/// note: this may override allowBluetoothA2DP if both are set
/// applied to categories: Record, PlayAndRecord
@property (nonatomic, assign) BOOL allowBluetooth;

/// use Bluetooth A2DP (Advanced Audio Distribution Profile)?
/// note: this is stereo, ie. jambox/headphones/earbuds
/// note: this may be overridden by allowBluetooth if both are set
/// applied to categories: PlayAndRecord,
/// always supported for output-only categories: Playback, Ambient, SoloAmbient
@property (nonatomic, assign) BOOL allowBluetoothA2DP;

/// use AirPlay?
/// applied to categories: PlayAndRecord,
/// always supported for output-only categories: Playback, Ambient, SoloAmbient
@property (nonatomic, assign) BOOL allowAirPlay;

#pragma mark Other Configuration Properties

// other options applied only during configuration

/// prefer stereo over mono input/output (default YES)
///
/// ensures a minimum of stereo IO as some routes (mono mic -> built-in
/// speaker) don't seem to like mismatched sessions (ie. 1 input and 2 outputs),
/// this also seems to enable automatic mixdown to mono for some outputs
@property (nonatomic, assign) BOOL preferStereo;

/// ignore audio session route changes (default NO)
///
/// by default, the audio controller will reconfigure the audio unit whenever
/// an IO device is changes, use this to override if you have your own custom
/// route change handling
@property (nonatomic, assign) BOOL ignoreRouteChanges;

#pragma mark Initialization

/// init with default pd audio unit
- (instancetype)init;

/// init with a custom pd audio unit
///
/// derive PdAudioUnit when you need to access to the raw samples when using,
/// for instance, AudioBus, and call this method after init
- (instancetype)initWithAudioUnit:(PdAudioUnit *)audioUnit;

#pragma mark Configuration

/// configure audio with the specified samplerate, as well as number of input
/// and output channels
///
/// inputChannels 0 uses the Playback AVAudioSession category and disables
/// the input, while > 0 uses the PlaybackAndRecord category
///
/// a channel value of -1 will use the current channel number(s) from the audio
/// session and automatically change them when the active session route changes
- (PdAudioStatus)configurePlaybackWithSampleRate:(int)sampleRate
                                   inputChannels:(int)inputChannels
                                  outputChannels:(int)outputChannels;

/// configure audio for recording, without output channels
///
/// uses the Record AVAudioSession category
///
/// a channel value of -1 will use the current channel number(s) from the audio
/// session and automatically change them when the active session route changes
- (PdAudioStatus)configureRecordWithSampleRate:(int)sampleRate
                                 inputChannels:(int)inputChannels;

/// configure audio for ambient use, without input channels.
///
/// mixWithOthers YES uses the Ambient AVAudioSession category, while setting NO
/// uses the SoloAmbient category
///
/// a channel value of -1 will use the current channel number(s) from the audio
/// session and automatically change them when the active session route changes
- (PdAudioStatus)configureAmbientWithSampleRate:(int)sampleRate
                                 outputChannels:(int)outputChannels;

/// configure audio for more advanced multi route port configuration
///
/// uses the MultiRoute AVAudioSession category, see Apple docs
/// note: does not allow Bluetooth or AirPlay
///
/// a channel value of -1 will use the current channel number(s) from the audio
/// session and automatically change them when the active session route changes
- (PdAudioStatus)configureMultiRouteWithSampleRate:(int)sampleRate
                                     inputChannels:(int)inputChannels
                                    outputChannels:(int)outputChannels;

/// note: legacy method kept for compatibility
///
/// configure the audio with the specified samplerate, as well as number of
/// output channels (which will also be the number of input channels if input is
/// enabled)
///
/// a channel value of -1 will use the current channel number(s) from the audio
/// session and automatically change them when the active session route changes
///
/// inputEnabled YES uses the PlayAndRecord AVAudioSession category while
/// setting NO uses the Playback category
///
/// mixingEnabled YES will allow the app to continue playing audio along with
/// other apps (such as Music), also sets the mixWithOthers property
- (PdAudioStatus)configurePlaybackWithSampleRate:(int)sampleRate
                                  numberChannels:(int)numChannels
                                    inputEnabled:(BOOL)inputEnabled
                                   mixingEnabled:(BOOL)mixingEnabled;

/// note: legacy method kept for compatibility
///
/// configure audio for ambient use, without input channels
///
/// a channel value of -1 will use the current channel number(s) from the audio
/// session and automatically change them when the active session route changes
///
/// mixingEnabled YES will allow the app to continue playing audio along with
/// other apps (such as Music) and uses the Ambient AVAudioSession
/// category while setting NO uses the SoloAmbient category, also sets the
/// mixWithOthers property
- (PdAudioStatus)configureAmbientWithSampleRate:(int)sampleRate
                                 numberChannels:(int)numberChannels
                                  mixingEnabled:(BOOL)mixingEnabled;

/// configure the ticksPerBuffer parameter which will change the audio session
/// IO buffer size, this can be done on the fly while audio is running.
///
/// note: the audio session only accepts values that correspond to a number
/// of frames that are a power of 2 and sometimes this value is ignored by the
/// audio unit, which works with whatever number of frames it is provided
- (PdAudioStatus)configureTicksPerBuffer:(int)ticksPerBuffer;

#pragma mark Subclass Overrides

/// returns combined audio session options when configuring for playback:
/// audio output only, no input
/// Playback is chosen when inputEnabled is NO and/or inputChannels = 0
/// override if you want to customize option handling
- (AVAudioSessionCategoryOptions)playbackOptions;

/// returns combined audio session options when configuring for playback:
/// audio input and output
/// PlayAndRecord is chosen when inputEnabled is YES and/or inputChannels > 0
/// override if you want to customize option handling
- (AVAudioSessionCategoryOptions)playAndRecordOptions;

/// returns combined audio session options when configuring for playback:
/// audio input only, no output
/// record is chosen when inputEnabled is YES and outputChannels > 0
/// override if you want to customize option handling
- (AVAudioSessionCategoryOptions)recordOptions;

/// returns  combined audio session options when configuring for ambient use,
/// doesn't mix with other apps
/// SoloAmbient is chosen with mixingEnabled is NO and/or mixWithOthers = NO
/// override if you want to customize option handling
- (AVAudioSessionCategoryOptions)soloAmbientOptions;

/// returns combined audio session options when configuring for ambient use,
/// mixes with other apps
/// Ambient is chosen with mixingEnabled is YES and/or mixWithOthers = YES
/// override if you want to customize option handling
- (AVAudioSessionCategoryOptions)ambientOptions;

/// returns combined audio session options when configuring for multi route
/// use, can mix with other apps, allows more advanced port configuration
/// override if you want to customize option handling
- (AVAudioSessionCategoryOptions)multiRouteOptions;

#pragma mark Util

/// print info on the audio session and audio unit to the console
- (void)print;

/// helper to add options to the current audio session category options
/// returns YES on success
+ (BOOL)addSessionOptions:(AVAudioSessionCategoryOptions)options;

/// helper to replace the current audio session category options
/// returns YES on success
+ (BOOL)setSessionOptions:(AVAudioSessionCategoryOptions)options;

@end
