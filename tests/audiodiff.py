import wave
import os

firstOutputWavPath = os.path.normpath("./tests/outputv1.0.wav")
secondOutputWavPath = os.path.normpath("./tests/outputv2.0.wav")
firstOutputWav = wave.open(firstOutputWavPath, "rb")
secondOutputWav = wave.open(secondOutputWavPath, "rb")

print("\n")
print(firstOutputWavPath)
print("Number of channels", firstOutputWav.getnchannels())
print("Sample width", firstOutputWav.getsampwidth())
print("Frame rate", firstOutputWav.getframerate())
print("Number of frames", firstOutputWav.getnframes())
print("\n")
print(secondOutputWavPath)
print("Number of channels", secondOutputWav.getnchannels())
print("Sample width", secondOutputWav.getsampwidth())
print("Frame rate", secondOutputWav.getframerate())
print("Number of frames", secondOutputWav.getnframes())
print("\n")

firstOutputFrames = firstOutputWav.readframes(firstOutputWav.getnframes())
secondOutputFrames = secondOutputWav.readframes(secondOutputWav.getnframes())

print("Comparing frames...")
test = False
if firstOutputFrames == secondOutputFrames:
    test = True
else:
    test = False

if test:
    print("Output wav files are equal")
    print("--------------------------")
    print("All tests passed ✅")
else:
    print("Output wav files are not equal")
    print("--------------------------")
    print("All tests failed ❌")
