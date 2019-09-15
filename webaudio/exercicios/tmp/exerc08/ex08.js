"user strict";


window.addEventListener('load', init_on_load, false);


function init_on_load() {
    var audioObj = new AudioText();
    audioObj.setResumeButton("bt-resume");
    audioObj.configure();
    window.audioObj = audioObj;
    window.audioSampler = new AudioSampler();


    window.bufferLoader = null;


    loadSound("./samples/s.wav");


    var bt = document.getElementById("bt-play");
    bt.addEventListener('click', function () {
        console.log('hit' + window.audioSampler);

        // Desafio (drum machine!!!!)
        playBatera();

    });

    draw();

}

class AudioSampler {
    construtor() {
        this._sample01 = null;
        this._sample02 = null;
        this._sample03 = null;
    }

    set sample01(s) {
        this._sampler01 = s;
    }
    set sample02(s) {
        this._sampler02 = s;
    }
    set sample03(s) {
        this._sampler03 = s;
    }
    get sample01() {
        return this._sampler01;
    }
    get sample02() {
        return this._sampler02;
    }
    get sample03() {
        return this._sampler03;
    }

}


class AudioText {
    constructor() {

        try {
            // Audio context -> 
            this._audioContext = new(window.AudioContext || window.webkitAudioContext)();
        } catch (e) {
            alert('Web Audio API is not supported in this browser');
        }
    }

    configure() {
        console.log("configuring audio...")
            // create nodes for audio generation
        this._sinea = this._audioContext.createOscillator();
        this._sineb = this._audioContext.createOscillator();
        this._sinec = this._audioContext.createOscillator();
        //======================================
        var analyser = this._audioContext.createAnalyser();
        analyser.fftSize = 2048 * 1;
        var bufferLength = analyser.frequencyBinCount;
        var dataArray = new Uint8Array(bufferLength);
        window.dataArray = dataArray;
        window.bufferLength = bufferLength;
        analyser.getByteTimeDomainData(dataArray);

        window.analyser = analyser;
        // Get a canvas defined with ID "oscilloscope"
        var canvas = document.getElementById("oscilloscope");
        var canvasCtx = canvas.getContext("2d");
        window.canvas = canvas;
        window.canvasCtx = canvasCtx;
        //======================================
        this._sinea.frequency.value = 440;
        this._sinea.type = "sine";

        this._sineb.frequency.value = 440 * 2;
        this._sineb.type = "sine";

        this._sinec.frequency.value = 440 * 3;
        //this._sinec.type = "sine";
        this._sinec.type = "square";
        this._sinec.type = "square";

        this._sinea.start();
        this._sineb.start();
        this._sinec.start();


        this._volumea = this._audioContext.createGain();
        this._volumeb = this._audioContext.createGain();
        this._volumec = this._audioContext.createGain();
        this._volumeMix = this._audioContext.createGain();

        this._sinea.connect(this._volumea);
        this._sineb.connect(this._volumeb);
        this._sinec.connect(this._volumec);

        //        this._volumea.connect(this._audioContext.destination);
        //        this._volumeb.connect(this._audioContext.destination);
        //        this._volumec.connect(this._audioContext.destination);
        this._volumea.connect(this._volumeMix);
        this._volumeb.connect(this._volumeMix);
        this._volumec.connect(this._volumeMix);

        this._volumea.gain.value = 0.0;
        this._volumeb.gain.value = 0.0;
        this._volumec.gain.value = 0.0;


        this._volumeMix.gain.value = 0.5;





        // Create the filter
        this._filter = this._audioContext.createBiquadFilter();
        // Create the audio graph.
        this._volumeMix.connect(this._filter);
        this._filter.connect(analyser);
        // Create and specify parameters for the low-pass filter.
        this._filter.type = 'lowpass'; // Low-pass filter. See BiquadFilterNode docs
        this._filter.frequency.value = 440; // Set cutoff to 440 HZ




        //this._volumeMix.connect(analyser);
        analyser.connect(this._audioContext.destination);



    }

    get audioContext() {
        return this._audioContext;
    }

    setVolumeA(v) {
        this._volumea.gain.value = v;
    }
    setVolumeB(v) {
        this._volumeb.gain.value = v;
    }
    setVolumeC(v) {
        this._volumec.gain.value = v;
    }

    setFreqA(v) {
        this._sinea.frequency.value = v;
    }
    setFreqFilter(f) {
        this._filter.frequency.value = f;
    }


    // One-liner to resume playback when user interacted with the page.
    setResumeButton(bt_id) {
        var bt = document.getElementById(bt_id);
        bt.addEventListener('click', function () {

            var audioCtx = window.audioObj.audioContext;

            if (audioCtx.state === 'running') {
                audioCtx.suspend().then(function () {
                    bt.style.backgroundColor = "red";
                });
            } else if (audioCtx.state === 'suspended') {
                audioCtx.resume().then(function () {
                    console.log('Playback resumed successfully');
                    bt.style.backgroundColor = "green";
                });
            }


        });
    }



    playSound(buffer) {
        var source = this._audioContext.createBufferSource(); // creates a sound source
        source.buffer = buffer; // tell the source which sound to play
        source.connect(this._audioContext.destination); // connect the source to the context's destination (the speakers)
        source.start(0); // play the source now
        // note: on older systems, may have to use deprecated noteOn(time);
    }

}

function changeGainA(g) {
    window.audioObj.setVolumeA(g);
}

function changeGainB(g) {
    window.audioObj.setVolumeB(g);
}

function changeGainC(g) {
    window.audioObj.setVolumeC(g);
}

function changeFreqA(g) {
    window.audioObj.setFreqA(g);
}

function changeFreqFilter(f) {
    window.audioObj.setFreqFilter(f);
}



function loadSound(url) {
    var request = new XMLHttpRequest();
    request.open('GET', url, true);
    request.responseType = 'arraybuffer';

    // Decode asynchronously
    request.onload = function () {
        var audioCtx = window.audioObj.audioContext;
        var audioSplr = window.audioSampler;
        audioCtx.decodeAudioData(request.response, function (buffer) {
            console.log('sample01 loaded: ' + buffer);
            audioSplr.sample01 = buffer;
            console.log(audioSplr.sample01);
        }, onSampleLoadError);
    }
    request.send();
}

function onSampleLoadError() {
    alert("Error while loading wav samples.");
}


function draw() {

    requestAnimationFrame(draw);

    window.analyser.getByteTimeDomainData(window.dataArray);

    window.canvasCtx.fillStyle = "rgb(200, 200, 200)";
    window.canvasCtx.fillRect(0, 0, window.canvas.width, window.canvas.height);

    window.canvasCtx.lineWidth = 1;
    window.canvasCtx.strokeStyle = "rgb(0, 0, 0)";

    window.canvasCtx.beginPath();

    var sliceWidth = window.canvas.width * 1.0 / window.bufferLength;
    var x = 0;

    for (var i = 0; i < window.bufferLength; i++) {

        var v = window.dataArray[i] / 128.0;
        var y = v * window.canvas.height / 2;

        if (i === 0) {
            window.canvasCtx.moveTo(x, y);
        } else {
            window.canvasCtx.lineTo(x, y);
        }

        x += sliceWidth;
    }

    window.canvasCtx.lineTo(window.canvas.width, window.canvas.height / 2);
    window.canvasCtx.stroke();
}



function playBatera() {
    var audioCtx = window.audioObj.audioContext;
    // We'll start playing the rhythm 100 milliseconds from "now"
    var startTime = audioCtx.currentTime + 0.100;
    var tempo = 80; // BPM (beats per minute)
    var eighthNoteTime = (60 / tempo) / 2;

    // Play 2 bars of the following:
    for (var bar = 0; bar < 2; bar++) {
        var time = startTime + bar * 8 * eighthNoteTime;
        // Play the bass (kick) drum on beats 1, 5
        playSoundRythm(window.audioSampler.sample01, time);
        playSoundRythm(window.audioSampler.sample01, time + 4 * eighthNoteTime);

        // Play the snare drum on beats 3, 7
        playSoundRythm(window.audioSampler.sample01, time + 2 * eighthNoteTime);
        playSoundRythm(window.audioSampler.sample01, time + 6 * eighthNoteTime);

        // Play the hi-hat every eighthh note.
        for (var i = 0; i < 8; ++i) {
            playSoundRythm(window.audioSampler.sample01, time + i * eighthNoteTime);
        }
    }
};

function playSoundRythm(buffer, time) {
    var audioCtx = window.audioObj.audioContext;
    var source = audioCtx.createBufferSource();
    source.buffer = buffer;
    source.connect(audioCtx.destination);
    source.start(time);
}
