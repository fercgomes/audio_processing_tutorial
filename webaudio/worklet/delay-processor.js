
class DelayProcessor extends AudioWorkletProcessor {

  // Custom AudioParams can be defined with this static getter.
  static get parameterDescriptors() {
    return [{ name: 'delay', defaultValue: 0 }]; //in samples
  }

  constructor() {
    // The super constructor call is required.
    super();
  }

  process(inputs, outputs, parameters) {
    const input = inputs[0];
    const output = outputs[0];
    const delay = parameters.delay;
    float decay = 0.8;

    for (let channel = 0; channel < input.length; ++channel) {
      const inputChannel = input[channel];
      const outputChannel = output[channel];

      if (delay.length === 1) {
        for (let i = 0; i < inputChannel.length; ++i)
          {
                outputChannel[i + delay[0]] += inputChannel[i] * decay;
          }
      } else {
        for (let i = 0; i < inputChannel.length; ++i)
          {
                outputChannel[i + delay[i]] += inputChannel[i] * decay;
          }
      }
    }

    return true;
  }
}

registerProcessor('delay-processor', DelayProcessor);


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
