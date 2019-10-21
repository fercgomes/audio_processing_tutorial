/* Initialize WebAudio */
var AudioContext = window.AudioContext || window.webkitAudioContext;
var audioCtx = new AudioContext({
  latencyHint: 'interactive',
  sampleRate: 44100,
});

/* Set the frequency table */
const freqTable = createFreqTable(440.0, 100);

const osc = new SoundSynth(audioCtx);

function playNote(note) {
  const idx = note;
  const frequency = freqTable[idx];
  adsr_t = [0.1, 0.1, 0.05, 0.2]; // in seconds
  adsr_a = [0.5, 0.4, 0.4, 0.0]; // [0 ~1];
  osc.play(frequency, 'sine', adsr_t, adsr_a, 1.0);
}

// Setup keyboard
var keyboard = document.getElementById('keyboard');
keyboard.addEventListener('change', (e) => {
  if(e.note[0]) { // note[0] == 1 -> noteOn
      console.log("Note-On:"+e.note[1]); 
      playNote(e.note[1]);
  }
	else
		console.log("Note-Off:"+e.note[1]);
});

function getRandomInt(min, max) {
  min = Math.ceil(min);
  max = Math.floor(max);
  return Math.floor(Math.random() * (max - min )) + min;
}

const times = ['whole', 'half', 'quarter',];

// Auto play
function getRandomNoteFromScale() {
  let major = [0, 2, 4, 5, 7, 9, 11, 12];
  let minor = [0, 2, 3, 5, 7, 9, 10, 12]
  let scale = minor;
  let noteIdx = getRandomInt(0, major.length);
  let durIdx = getRandomInt(0, times.length);
  let duration = times[durIdx];
  let note = scale[noteIdx];
  return [note, duration];
}

function sleep(ms) {
  return new Promise(resolve => setTimeout(resolve, ms));
}

async function playRandom() {
  recorder.record();
  let numNotes = 25;
  for(let i = 0; i < numNotes; i++) {
    let wholeDuration = 500.0; // MS
    let random = getRandomNoteFromScale(0);
    let interval = random[0];
    let duration = random[1];
    console.log(duration);
    
    let baseNote = 0;
    let note = baseNote + interval;
    let wait;

    switch(duration) {
      case 'whole':
        wait = wholeDuration;
        break;
      case 'half':
        wait = wholeDuration / 2;
        break;
      case 'quarter':
        wait = wholeDuration / 4;
        break;
      case 'eigth':
        wait = wholeDuration / 8;
        break;
    }

    console.log(wait);
    playNote(note);
    await sleep(wait);
  }

  recorder.stop();
  createDownloadLink();
}

let outNode = osc.outNode();
let recorder = new Recorder(outNode)

function createDownloadLink() {
  recorder && recorder.exportWAV(function(blob) {
    var url = URL.createObjectURL(blob);
    var li = document.createElement('li');
    var au = document.createElement('audio');
    var hf = document.createElement('a');
    
    au.controls = true;
    au.src = url;
    hf.href = url;
    hf.download = new Date().toISOString() + '.wav';
    hf.innerHTML = hf.download;
    li.appendChild(au);
    li.appendChild(hf);
    document.body.appendChild(li);
  });
}


document.getElementById("play-random").addEventListener('click', () => {
  playRandom();
});



