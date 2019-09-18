/* Initialize WebAudio */
var AudioContext = window.AudioContext || window.webkitAudioContext;
var audioCtx = new AudioContext({
  latencyHint: 'interactive',
  sampleRate: 44100,
});

/* Set the frequency table */
const freqTable = createFreqTable(440.0, 100);

const osc = new SoundSynth(audioCtx);

// Setup keyboard
var keyboard = document.getElementById('keyboard');
keyboard.addEventListener('change', (e) => {
    if(e.note[0]) { // note[0] == 1 -> noteOn
        console.log("Note-On:"+e.note[1]); 
        const idx = e.note[1];
        const frequency = freqTable[idx];
        adsr_t = [0.1, 0.2, 0.2, 0.2]; // in seconds
        adsr_a = [0.5, 0.4, 0.4, 0.0]; // [0 ~1];
        osc.play(frequency, 'sine', adsr_t, adsr_a, 1.0);
    }
	else
		console.log("Note-Off:"+e.note[1]);
});