// Copyright (c) 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

/**
 * A simple bypass node demo.
 *
 * @class BypassProcessor
 * @extends AudioWorkletProcessor
 */
class BlockProcessor extends AudioWorkletProcessor {
    constructor() {
        super();

        this.n_period = 32;
        this.cBuffer = new Float32Array(128 * this.n_period);
    }

    process(inputs, outputs) {
        //console.log("in: " + inputs.length + "  - out: " + outputs.length);


        this.cBuffer.set(this.cBuffer.subarray(0, 128 * 31), 0);


        let input = inputs[0];
        let output = outputs[0];

        for (let channel = 0; channel < output.length; ++channel) {
            let inputChannel = input[channel];
            let outputChannel = output[channel];
            //console.log("buffer size: " + inputChannel.length);            

            for (let i = 0; i < inputChannel.length; ++i) {
                outputChannel[i] = inputChannel[i];
                this.cBuffer[this.n_period * 128 + i] = inputChannel[i];
            }

        }

        return true;
    }
}

registerProcessor('block-processor', BlockProcessor);
