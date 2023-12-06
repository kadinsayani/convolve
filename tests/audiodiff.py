import wave
import os

output_paths = [
    "./tests/outputv1.0.wav",
    "./tests/outputv2.0.wav",
    "./tests/outputv3.0.wav",
    "./tests/outputv4.0.wav",
    "./tests/outputv4.1.wav",
    "./tests/outputv4.2.wav",
    "./tests/outputv4.3.wav",
    "./tests/outputv4.4.wav"
]

output_wavs = [wave.open(os.path.normpath(path), "rb")
               for path in output_paths]

for i, output_wav in enumerate(output_wavs):
    print("\n")
    print(output_paths[i])
    print("Number of channels", output_wav.getnchannels())
    print("Sample width", output_wav.getsampwidth())
    print("Frame rate", output_wav.getframerate())
    print("Number of frames", output_wav.getnframes())


def compare_frames(frames_list):
    return all(frame == frames_list[0] for frame in frames_list)


output_frames = [output_wav.readframes(
    output_wav.getnframes()) for output_wav in output_wavs]

print("\n")
if compare_frames(output_frames):
    print("Output wav files are equal")
    print("--------------------------")
    print("All tests passed ✅")
else:
    print("Output wav files are not equal")
    print("--------------------------")
    print("All tests failed ❌")
