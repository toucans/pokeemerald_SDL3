/* m4a-worklet.js — thin AudioWorkletProcessor wrapping the shared M4AEngine.
 *
 * Used only on secure contexts (https / localhost), where AudioWorklet is
 * exposed. On plain http (e.g. the VPN dashboard) player.js falls back to a
 * ScriptProcessorNode driving the same M4AEngine — see player.js / m4a-engine.js.
 *
 * This is a *module* worklet (it imports); addModule loads it accordingly.
 */
import { M4AEngine } from './m4a-engine.js';

class M4AProcessor extends AudioWorkletProcessor {
  constructor() {
    super();
    this.eng = new M4AEngine(sampleRate);
    this.eng.onended = () => this.port.postMessage({ type: 'ended' });
    this.port.onmessage = (e) => {
      const m = e.data;
      if (m.type === 'bank') this.eng.loadBank(m.bank);
      else if (m.type === 'play') this.eng.play(m.song);
      else if (m.type === 'stop') this.eng.stop();
    };
  }

  process(_inputs, outputs) {
    const out = outputs[0];
    this.eng.render(out[0], out[1] || out[0], out[0].length);
    return true;
  }
}

registerProcessor('m4a-processor', M4AProcessor);
