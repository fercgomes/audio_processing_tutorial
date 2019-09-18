// Create frequency table
/*
    Starting from C?
    Respects ordering from webaudio-keyboard
*/
function createFreqTable(baseFrequency, size) {
    let freqTable = []
    for(i = 0; i < size; i++) {
        freqTable.push(baseFrequency * Math.pow(2, (i - 9)/12));
    }
    return freqTable
}