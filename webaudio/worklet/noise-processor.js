
class NoiseProcessor extends AudioWorkletProcessor {

  // Custom AudioParams can be defined with this static getter.
  static get parameterDescriptors() {
    return [{ name: 'noise', defaultValue: 0 }]; //in samples
  }

  constructor() {
    // The super constructor call is required.
    super();
  }

  process(inputs, outputs, parameters) {
    const input = inputs[0];
    const output = outputs[0];
    const noise = parameters.noise;

    for (let channel = 0; channel < input.length; ++channel) {
      const inputChannel = input[channel];
      const outputChannel = output[channel];

      if (noise.length === 1) {
        for (let i = 0; i < inputChannel.length; ++i)
          {
              
             outputChannel[i] = inputChannel[i] + Math.rand()*noise[0];;
          }
      } else {
        for (let i = 0; i < inputChannel.length; ++i)
          {
             outputChannel[i] = inputChannel[i] + Math.rand()*noise[i];;
          }
      }
    }

    return true;
  }
}

registerProcessor('noise-processor', NoiseProcessor);


/*
//  https://developer.mozilla.org/en-US/docs/Web/API/AudioWorkletProcessor/parameterDescriptors 
//  
 // A separate script file, like "my-worklet-processor.js" 
class MyWorkletProcessor extends AudioWorkletProcessor {

  // Static getter to define AudioParam objects in this custom processor.
  static get parameterDescriptors() {
    return [{
      name: 'myParam',
      defaultValue: 0.707
    }];
  }

  constructor() { super(); }

  process(inputs, outputs, parameters) {
    // |myParamValues| is a Float32Array of either 1 or 128 audio samples
    // calculated by WebAudio engine from regular AudioParam operations.
    // (automation methods, setter) Without any AudioParam change, this array
    // would be a single value of 0.707.
    const myParamValues = parameters.myParam;

    if (myParamValues.length === 1) {
      // |myParam| has been a constant value for the current render quantum,
      // which can be accessed by |myParamValues[0]|.
    } else {
      // |myParam| has been changed and |myParamValues| has 128 values.
    }
  }
}
*/
