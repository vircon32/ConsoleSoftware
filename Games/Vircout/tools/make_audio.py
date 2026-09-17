#!/usr/bin/env python3
"""Generate all original WAV assets for V32 Breakout.

Only Python + numpy are required. Outputs mono 16-bit PCM WAV at 44100 Hz,
ready for Vircon32's wav2vircon tool.
"""
from pathlib import Path
import math
import wave
import numpy as np

SR = 44100
ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "audio"
OUT.mkdir(parents=True, exist_ok=True)


def write_wav(name, samples):
    samples = np.asarray(samples, dtype=np.float64)
    peak = float(np.max(np.abs(samples))) if len(samples) else 1.0
    if peak > 0.96:
        samples = samples * (0.96 / peak)
    pcm = np.clip(samples, -1.0, 1.0)
    pcm = (pcm * 32767.0).astype('<i2')
    with wave.open(str(OUT / name), 'wb') as w:
        w.setnchannels(1)
        w.setsampwidth(2)
        w.setframerate(SR)
        w.writeframes(pcm.tobytes())


def silence(seconds):
    return np.zeros(int(SR * seconds), dtype=np.float64)


def osc(freq, seconds, kind='square', phase=0.0):
    n = int(SR * seconds)
    t = np.arange(n, dtype=np.float64) / SR
    p = 2 * np.pi * freq * t + phase
    if kind == 'square':
        return np.where(np.sin(p) >= 0, 1.0, -1.0)
    if kind == 'triangle':
        return (2 / np.pi) * np.arcsin(np.sin(p))
    if kind == 'saw':
        return 2.0 * ((freq * t + phase / (2*np.pi)) % 1.0) - 1.0
    return np.sin(p)


def env_ar(n, attack=0.01, release=0.08):
    e = np.ones(n, dtype=np.float64)
    a = min(n, max(1, int(SR * attack)))
    r = min(n, max(1, int(SR * release)))
    e[:a] *= np.linspace(0, 1, a, endpoint=False)
    e[-r:] *= np.linspace(1, 0, r, endpoint=True)
    return e


def note(freq, seconds, kind='square', volume=0.2, attack=0.006, release=0.04):
    x = osc(freq, seconds, kind)
    return x * env_ar(len(x), attack, release) * volume


def mix_into(dst, src, start_s):
    i = int(start_s * SR)
    j = min(len(dst), i + len(src))
    if i < len(dst) and j > i:
        dst[i:j] += src[:j-i]


def midi(n):
    return 440.0 * 2.0 ** ((n - 69) / 12.0)


def sweep(seconds, f0, f1, kind='square', volume=0.25):
    n = int(SR * seconds)
    t = np.arange(n, dtype=np.float64) / SR
    if f0 > 0 and f1 > 0:
        ratio = f1 / f0
        phase = 2*np.pi * f0 * seconds / math.log(ratio) * (ratio ** (t / seconds) - 1) if ratio != 1 else 2*np.pi*f0*t
    else:
        phase = 2*np.pi * (f0*t + 0.5*(f1-f0)/seconds*t*t)
    if kind == 'square':
        x = np.where(np.sin(phase) >= 0, 1.0, -1.0)
    elif kind == 'triangle':
        x = (2/np.pi) * np.arcsin(np.sin(phase))
    else:
        x = np.sin(phase)
    return x * env_ar(n, 0.003, min(0.09, seconds*0.6)) * volume


def noise(seconds, volume=0.15, seed=1234):
    rng = np.random.default_rng(seed)
    n = int(SR * seconds)
    x = rng.uniform(-1, 1, n)
    return x * env_ar(n, 0.002, seconds*0.85) * volume


def make_title_music():
    # Original 8-bar title theme at 120 BPM, kept deliberately compact.
    # It is rendered slightly quieter than in milestone 7.0 so it sits back
    # behind the title screen instead of competing with the game audio.
    bpm = 120
    beat = 60.0 / bpm
    bars = 8
    length = bars * 4 * beat
    out = silence(length)

    roots = [45, 41, 48, 43]  # A2 F2 C3 G2
    chords = [
        [57, 60, 64],
        [53, 57, 60],
        [60, 64, 67],
        [55, 59, 62],
    ]
    phrase = [69,72,76,72, 67,72,76,79, 69,72,76,81, 79,76,72,67]
    phrase2 = [72,76,79,76, 69,72,76,72, 67,71,74,79, 76,74,71,67]

    for bar in range(bars):
        root = roots[(bar // 2) % 4]
        chord = chords[(bar // 2) % 4]
        tbar = bar * 4 * beat

        for b in range(4):
            n = root + (12 if b == 3 else 0)
            mix_into(out, note(midi(n), beat*0.82, 'triangle', 0.16, release=0.08), tbar + b*beat)

        for e in range(8):
            nn = chord[e % 3] + 12
            mix_into(out, note(midi(nn), beat*0.38, 'square', 0.065, release=0.025), tbar + e*(beat/2))

        for b in range(4):
            mix_into(out, sweep(0.07, 110, 48, 'sine', 0.12), tbar + b*beat)
        mix_into(out, noise(0.045, 0.08, 100+bar), tbar + beat)
        mix_into(out, noise(0.045, 0.08, 200+bar), tbar + 3*beat)

        seq = phrase if bar % 2 == 0 else phrase2
        for step, nn in enumerate(seq):
            if step % 4 != 3:
                mix_into(out, note(midi(nn), beat*0.19, 'square', 0.075, release=0.02), tbar + step*(beat/4))

    # Milestone 7.2: another ~3 dB below the 7.1 title mix
    # (roughly -6 dB relative to the original shared title/gameplay track).
    out *= 0.50
    write_wav('music_title.wav', out)


def pluck(freq, seconds, volume=0.15, bright=0.35):
    """Short electronic pluck with a fast exponential decay."""
    n = int(SR * seconds)
    t = np.arange(n, dtype=np.float64) / SR
    tri = osc(freq, seconds, 'triangle')
    sq = osc(freq * 2.0, seconds, 'square')
    body = tri * (1.0 - bright) + sq * bright
    decay = np.exp(-5.5 * t / max(seconds, 0.001))
    attack_n = max(1, int(0.004 * SR))
    decay[:attack_n] *= np.linspace(0.0, 1.0, attack_n, endpoint=False)
    return body * decay * volume


def soft_bass(freq, seconds, volume=0.12):
    """Warm bass made from triangle plus a quiet sine fundamental."""
    x = osc(freq, seconds, 'triangle') * 0.65 + osc(freq / 2.0, seconds, 'sine') * 0.35
    return x * env_ar(len(x), 0.018, min(0.20, seconds * 0.35)) * volume




def soft_keys(freq, seconds, volume=0.08):
    """Mellow electric-key voice: rounded fundamental plus a soft overtone."""
    x = (osc(freq, seconds, 'sine') * 0.62 +
         osc(freq * 2.0, seconds, 'sine') * 0.18 +
         osc(freq, seconds, 'triangle') * 0.20)
    return x * env_ar(len(x), 0.035, min(0.28, seconds * 0.45)) * volume

def make_gameplay_music():
    """Downtempo electronic gameplay theme, intentionally distinct from title.

    16 bars at 84 BPM are roughly 45.7 seconds. Instead of the previous
    sustained-pad arrangement this version is built around warm bass pulses,
    broken-chord plucks, syncopated arpeggios and a sparse lead. It keeps an
    ambient pace while changing texture and rhythmic density between sections.
    """
    bpm = 84
    beat = 60.0 / bpm
    bars = 16
    length = bars * 4 * beat
    out = silence(length)

    # Harmonic route: A minor -> F -> C -> G, with two contrasting middle
    # sections before resolving back to A minor. MIDI values are deliberately
    # low/mid to keep the track relaxed rather than bright like the title.
    roots = [45, 41, 48, 43,
             45, 40, 41, 43,
             48, 43, 45, 41,
             45, 43, 41, 45]
    chords = [
        [57,60,64], [53,57,60], [60,64,67], [55,59,62],
        [57,60,64], [52,55,59], [53,57,60], [55,59,62],
        [60,64,67], [55,59,62], [57,60,64], [53,57,60],
        [57,60,64], [55,59,62], [53,57,60], [57,60,64],
    ]

    # Flute-like lead: coherent 4-bar phrases in A natural minor. Strong beats
    # land mostly on chord tones; passing notes move stepwise between them.
    # The four sections reuse recognizable motifs with small variations so the
    # melody feels related to the harmony instead of sounding random.
    lead = [
        # A: call phrase over Am - F - C - G
        [(0.5,69,0.55),(1.5,72,0.55),(2.5,76,0.70),(3.5,72,0.35)],
        [(0.0,69,0.70),(1.25,67,0.35),(2.0,65,0.70),(3.0,69,0.55)],
        [(0.5,67,0.45),(1.25,72,0.55),(2.25,76,0.55),(3.25,74,0.40)],
        [(0.0,71,0.65),(1.25,69,0.35),(2.0,67,0.60),(3.0,74,0.70)],

        # B: same contour, slightly higher answer and longer rests
        [(0.25,69,0.45),(1.0,72,0.45),(2.0,76,0.75),(3.25,81,0.45)],
        [(0.0,71,0.65),(1.5,67,0.50),(2.5,64,0.70)],
        [(0.5,69,0.45),(1.25,72,0.70),(2.75,69,0.60)],
        [(0.0,71,0.45),(0.75,74,0.55),(2.0,71,0.45),(3.0,67,0.75)],

        # C: brighter register over C - G - Am - F, still chord-led
        [(0.5,72,0.45),(1.25,76,0.55),(2.25,79,0.70),(3.25,76,0.35)],
        [(0.0,74,0.55),(1.25,71,0.55),(2.5,67,0.75)],
        [(0.5,69,0.40),(1.25,72,0.45),(2.0,76,0.55),(3.0,81,0.65)],
        [(0.0,72,0.60),(1.5,69,0.45),(2.5,65,0.80)],

        # D: resolving variation that recalls the opening motif
        [(0.25,76,0.45),(1.0,72,0.45),(1.75,69,0.55),(3.0,72,0.70)],
        [(0.0,74,0.55),(1.0,71,0.45),(2.0,69,0.45),(3.0,67,0.75)],
        [(0.5,69,0.45),(1.25,67,0.35),(2.0,65,0.55),(3.0,69,0.75)],
        [(0.0,69,0.55),(1.0,72,0.55),(2.0,76,0.75),(3.25,69,0.95)],
    ]

    # Broken-chord patterns change every four bars instead of repeating a
    # single ostinato. Values are chord indices, None means rest.
    arp_patterns = [
        [0,2,1,None, 2,1,0,None],
        [0,None,1,2, 1,None,2,1],
        [2,1,None,0, 1,2,None,1],
        [0,1,2,1, None,2,1,None],
    ]

    for bar in range(bars):
        tbar = bar * 4 * beat
        root = roots[bar]
        chord = chords[bar]
        section = bar // 4

        # Warm bass: alternating long pulse and shorter response. Every second
        # bar changes the response to a fifth/octave, giving movement.
        mix_into(out, soft_bass(midi(root), beat*1.55, 0.115), tbar)
        response = root + (7 if bar % 2 else 12)
        mix_into(out, soft_bass(midi(response), beat*0.85, 0.082), tbar + 2.5*beat)

        # Main electronic pluck/arpeggio. Sections deliberately alter density
        # and octave so the texture evolves over the 45-second loop.
        pattern = arp_patterns[section]
        octave = 12 if section in (0,3) else 0
        for step, ci in enumerate(pattern):
            if ci is not None:
                nn = chord[ci] + octave
                pos = tbar + step * (beat/2)
                vol = 0.060 if section != 2 else 0.050
                mix_into(out, pluck(midi(nn), beat*0.38, vol, 0.28), pos)

        # A second, quieter syncopated pluck appears only in the middle half.
        # This makes sections B/C feel instrumentally different without adding
        # a continuous pad underneath everything.
        if 4 <= bar < 12:
            for k, posb in enumerate((0.75, 1.75, 3.25)):
                nn = chord[(k + bar) % 3] + 12
                mix_into(out, pluck(midi(nn), beat*0.26, 0.034, 0.12),
                         tbar + posb*beat)

        # Flute-like lead uses a soft sine/triangle blend.
        for posb, nn, durb in lead[bar]:
            dur = durb * beat
            tone = (osc(midi(nn), dur, 'sine') * 0.72 +
                    osc(midi(nn), dur, 'triangle') * 0.28)
            tone *= env_ar(len(tone), 0.025, min(0.14, dur*0.35)) * 0.070
            mix_into(out, tone, tbar + posb*beat)

        # Additional electric-key layer. It answers the flute rather than
        # playing continuously, filling the sparser spaces in the arrangement.
        # The voicing follows the current chord, so it reinforces harmony while
        # giving the track a warmer, less empty midrange.
        key_events = ((0.0, 0, 0.82), (2.0, 1, 0.72))
        if section == 1:
            key_events = ((0.5, 1, 0.65), (2.5, 2, 0.92))
        elif section == 2:
            key_events = ((0.0, 2, 0.72), (1.75, 0, 0.62), (3.0, 1, 0.72))
        elif section == 3:
            key_events = ((0.5, 0, 0.82), (2.75, 2, 0.82))

        for posb, ci, durb in key_events:
            # Alternate octave every bar to keep the layer from becoming a
            # fixed ostinato; quieter than both flute and primary arpeggio.
            nn = chord[ci] + (12 if bar % 2 == 0 else 0)
            mix_into(out, soft_keys(midi(nn), durb*beat, 0.032),
                     tbar + posb*beat)

        # Downtempo percussion: soft kick on 1, lighter kick on 3, tiny hats.
        mix_into(out, sweep(0.095, 78, 42, 'sine', 0.060), tbar)
        mix_into(out, sweep(0.075, 70, 40, 'sine', 0.044), tbar + 2*beat)
        if bar % 2 == 0:
            mix_into(out, noise(0.028, 0.020, 3000+bar), tbar + 1.5*beat)
            mix_into(out, noise(0.025, 0.017, 4000+bar), tbar + 3.5*beat)
        else:
            mix_into(out, noise(0.024, 0.018, 5000+bar), tbar + beat)
            mix_into(out, noise(0.024, 0.016, 6000+bar), tbar + 3*beat)

        # Short transition sweep only at section boundaries.
        if bar in (3, 7, 11):
            mix_into(out, sweep(0.30, 320, 720, 'sine', 0.025),
                     tbar + 3.45*beat)

    # Keep approximately the same perceived gameplay level requested in 7.2.
    # write_wav protects against accidental clipping if later edits add energy.
    out *= 1.75
    write_wav('music_gameplay.wav', out)

def make_ending_music():
    beat = 0.42
    length = 6.0
    out = silence(length)
    melody = [60,64,67,72, 67,69,72,76, 72,76,79,84]
    for i, nn in enumerate(melody):
        dur = beat * (1.7 if i in (3,7,11) else 0.85)
        mix_into(out, note(midi(nn), dur, 'square', 0.12, release=0.10), i*beat)
        if i % 4 == 0:
            root = [48,53,55][min(i//4,2)]
            mix_into(out, note(midi(root), beat*3.6, 'triangle', 0.12, release=0.18), i*beat)
    # Final bright chord.
    t = 12*beat
    for nn in [60,64,67,72]:
        mix_into(out, note(midi(nn), 0.85, 'triangle', 0.08, release=0.25), t)
    out *= 10 ** (-5 / 20)
    write_wav('music_ending.wav', out)


def make_sfx():
    # Paddle: bright rubber/plastic bounce.
    x = sweep(0.10, 260, 520, 'triangle', 0.34)
    write_wav('sfx_paddle.wav', x)

    # Wall: shorter, harder click.
    x = sweep(0.075, 430, 310, 'square', 0.22) + noise(0.075, 0.06, 1)
    x *= 10 ** (-5 / 20)
    write_wav('sfx_wall.wav', x)

    # Normal block: crisp pop with blue-explosion character.
    x = sweep(0.13, 520, 180, 'square', 0.20) + noise(0.13, 0.10, 2)
    x *= 10 ** (-7 / 20)
    write_wav('sfx_normal.wav', x)

    # Hard block damaged: metallic ping, no destruction.
    out = silence(0.16)
    mix_into(out, note(880, 0.14, 'sine', 0.24, release=0.12), 0)
    mix_into(out, note(1320, 0.10, 'sine', 0.12, release=0.08), 0.01)
    write_wav('sfx_hard_hit.wav', out)

    # Hard block destroyed: heavier orange burst.
    x = sweep(0.19, 400, 95, 'square', 0.24) + noise(0.19, 0.15, 3)
    x *= 10 ** (-10 / 20)
    write_wav('sfx_hard_break.wav', x)

    # Unbreakable: short metallic spark/chime.
    out = silence(0.14)
    mix_into(out, note(1568, 0.12, 'sine', 0.22, release=0.10), 0)
    mix_into(out, note(2093, 0.09, 'sine', 0.10, release=0.07), 0.008)
    write_wav('sfx_metal.wav', out)

    # Item pickup: rising 3-note chirp.
    out = silence(0.25)
    for i, nn in enumerate([72, 76, 81]):
        mix_into(out, note(midi(nn), 0.085, 'square', 0.16, release=0.04), i*0.065)
    out *= 10 ** (-5 / 20)
    write_wav('sfx_item.wav', out)

    # Twin laser burst (the game emits two projectiles but only plays one SFX).
    out = silence(0.12)
    mix_into(out, sweep(0.10, 1250, 620, 'square', 0.18), 0)
    mix_into(out, sweep(0.08, 1550, 820, 'square', 0.12), 0.012)
    out *= 10 ** (-5 / 20)
    write_wav('sfx_laser.wav', out)

    # Ball lost: descending arcade tone.
    out = silence(0.55)
    mix_into(out, sweep(0.52, 520, 90, 'triangle', 0.23), 0)
    write_wav('sfx_ball_lost.wav', out)

    # Level clear: ascending fanfare.
    out = silence(0.85)
    for i, nn in enumerate([60,64,67,72,76]):
        mix_into(out, note(midi(nn), 0.20 if i < 4 else 0.40, 'square', 0.15, release=0.10), i*0.11)
    out *= 10 ** (-10 / 20)
    write_wav('sfx_level_clear.wav', out)

    # Game over: compact descending phrase.
    out = silence(0.9)
    for i, nn in enumerate([60,57,53,48]):
        mix_into(out, note(midi(nn), 0.28 if i < 3 else 0.48, 'triangle', 0.17, release=0.14), i*0.16)
    out *= 10 ** (10 / 20)
    write_wav('sfx_game_over.wav', out)


    # Konami-code confirmation: short bright arcade flourish.
    out = silence(0.42)
    for i, nn in enumerate([72, 76, 79, 84]):
        duration = 0.10 if i < 3 else 0.18
        mix_into(out, note(midi(nn), duration, 'square', 0.12, release=0.055), i * 0.07)
    mix_into(out, note(midi(60), 0.24, 'triangle', 0.055, release=0.10), 0.18)
    write_wav('sfx_cheat.wav', out)


if __name__ == '__main__':
    make_title_music()
    make_gameplay_music()
    make_ending_music()
    make_sfx()
    print(f"Generated WAV assets in {OUT}")
