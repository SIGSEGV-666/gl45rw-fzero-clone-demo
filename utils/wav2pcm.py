import wave
from struct import pack, unpack
from io import BytesIO
import sys, os, traceback

PCM_IDENT = b"gl45_pcmfile_hdr"

if __name__ == "__main__":
    infn = sys.argv[1]
    outfn = sys.argv[2]
    with wave.open(infn, 'rb') as inwav:
        with open(outfn, 'wb') as outpcm:
            outpcm.write(PCM_IDENT)
            nch = inwav.getnchannels()
            sampw = inwav.getsampwidth()
            rate = inwav.getframerate()
            nsamples = inwav.getnframes()
            sampt = ({1: 0, 2: 1, 4: 5}).get(sampw, 0)
            outpcm.write(pack("<5I", 0, nch, rate, sampt, nsamples))
            framesraw = inwav.readframes(nsamples)
            outpcm.write(framesraw)
    print("Conversion complete!")


